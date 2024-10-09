#include <systems/ssl.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ssl_t * ssl_construct(const mics_t * mics, const points_t * points, const float sample_rate, const float sound_speed, const unsigned int num_sources, const unsigned int num_directions) {

	ssl_t * obj = (ssl_t *) malloc(sizeof(ssl_t));

	obj->num_channels = mics->num_mics;
    obj->num_pairs = (mics->num_mics-1) * mics->num_mics / 2;
    obj->num_sources = num_sources;
    obj->num_directions = num_directions;
    obj->num_points = points->num_points;

    obj->sample_rate = sample_rate;
    obj->sound_speed = sound_speed;

    //
    // Compute the distance between each pair of microphones (to speed computations later, because
    // the square root can be expensive).
    //
    // The distance corresponds to {(mic1_x - mic2_x)^2 + (mic1_y - mic2_y)^2 + (mic1_z - mic2_z)^2}^(1/2)
    //

    obj->distances = (float *) malloc(sizeof(float) * obj->num_pairs);
    unsigned int index_pair = 0;
    for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {
        for (unsigned int index_channel2 = (index_channel1 + 1); index_channel2 < obj->num_channels; index_channel2++) {
            obj->distances[index_pair] = xyz_mag(xyz_sub(mics->mics[index_channel1].position, mics->mics[index_channel2].position));
            index_pair++;
        }
    }

    //
    // This will hold the TDoAs converted to AoAs
    //

    obj->aoas = (aoa_t **) malloc(sizeof(aoa_t *) * obj->num_sources);
    for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {
        obj->aoas[index_source] = (aoa_t *) malloc(sizeof(aoa_t) * obj->num_pairs);
    }    

    //
    // Pointers to the mics and points object for further reference (avoid copy to save space)
    //

    obj->mics = mics;
    obj->points = points;
    
    //
    // Create the table that will link each point to the AoAs
    //
    // Index 0 is used when the gain is below threshold 0.1
    // Otherwise the angles are mapped to {0, 1, ..., 180}, on interval [1, 181]
    //
    // For each point, the norms value will normalize to ensure a total of 1 based on the
    // number of microphones used (those not removed when gain is too small)
    //

    obj->table = (unsigned char *) calloc(obj->num_points * obj->num_pairs, sizeof(unsigned char));
    obj->norms = (float *) calloc(obj->num_points, sizeof(float));

    for (unsigned int index_point = 0; index_point < obj->num_points; index_point++) {

        xyz_t point = obj->points->points[index_point];
        unsigned int index_pair = 0;
        unsigned int num_hits = 0;

        for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {

            mic_t mic1 = mics->mics[index_channel1];

            for (unsigned int index_channel2 = (index_channel1 + 1); index_channel2 < obj->num_channels; index_channel2++) {

                mic_t mic2 = mics->mics[index_channel2];

                float gain = mic_gain(mic1, point) * mic_gain(mic2, point);
                float angle = roundf((180.0f / M_PI) * acosf(xyz_dot(xyz_unit(xyz_sub(mic2.position, mic1.position)), point)));

                unsigned char index_table;

                if (gain > 0.1f) {
                    index_table = ((unsigned char) angle) + 1;
                    num_hits++;
                }
                else {
                    index_table = 0;
                }

                obj->table[index_point * obj->num_pairs + index_pair] = index_table;
                if (num_hits > 0) {
                    obj->norms[index_point] = 1.0f / ((float) num_hits);
                }
                else {
                    obj->norms[index_point] = 0.0f;
                }

                index_pair++;

            }

        }

    }

    //
    // Kernel (only one half because symmetrical)
    //

    obj->kernel_size = sizeof(kernel_gaussian) / sizeof(float);
    obj->kernel = kernel_gaussian;

    //
    // Will hold the synthesized signals with kernels. One signal per pair.
    //

    obj->synthesis = (float **) malloc(sizeof(float *) * obj->num_pairs);
    for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
        obj->synthesis[index_pair] = (float *) calloc(182, sizeof(float));
    }

    //
    // Will hold the energy for each point scanned
    //

    obj->projections = (float *) calloc(obj->num_points, sizeof(float));

    return obj;

}

void ssl_destroy(ssl_t * obj) {

    free(obj->distances);

    for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {
        free(obj->aoas[index_source]);
    }
    free(obj->aoas);

    free(obj->table);
    free(obj->norms);

    for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
        free(obj->synthesis[index_pair]);
    }
    free(obj->synthesis);

    free(obj->projections);

    free(obj);

}

int ssl_process(ssl_t * obj, const tdoas_t * tdoas, doas_t * doas) {

    //
    // Convert TDoAs to AoAs
    //
    // Each TDoA is given in samples. This will be normalized to an angle in degree
    // between 0 and 180. The equation is:
    //
    // tau = (fS/c) * d * cos(theta), where    fS is the sample rate (samples/sec)
    //                                          c is the speed of sound (m/sec)
    //                                          d is the distance between mics (m)
    //                                      theta is the angle
    //
    // The angle then corresponds to:
    //
    // theta = acos((c/fS) * tau / d), in radians (need x 180/pi to convert in degrees)
    //
    // The amplitude associated to each TDoA is the same for each AoA.
    //

    for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {
        
        for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
        
            obj->aoas[index_source][index_pair].degree = (180.0f / M_PI) * acosf((obj->sound_speed/obj->sample_rate) * tdoas->taus[index_source][index_pair].delay / obj->distances[index_pair]);
            obj->aoas[index_source][index_pair].amplitude = tdoas->taus[index_source][index_pair].amplitude;
        
        }

    }

    //
    // Scan for each direction
    //
    // The energy of each point is computed. When the maximum value is found, the AoAs that
    // contributed to this maximum value are removed, and a new scan is performed. This goes
    // on until the desired number of directions is found.
    //

    for (unsigned int index_direction = 0; index_direction < obj->num_directions; index_direction++) {

        //
        // Create the synthesized signals
        //
        // For each AoA, a gaussian is scaled and added to the synthesized signal.
        // For instance, if we have two AoA, one at 43 degrees with amplitude of 0.5, and
        // the second one at 110 degrees with amplitude of 0.2, we have the following:
        //
        //                  kernel
        //                <------->
        // |----- ... -----xxxxxxx----- ... ---------------------------- ... -----| x 0.5
        //                    ^
        //                  ( 43)
        //                                      +
        //                                              kernel
        //                                            <------->
        // |----- ... ----------------- ... -----------xxxxxxx---------- ... -----| x 0.2
        //                                                ^
        //                                              (110)
        //
        //                                      =
        //
        // |----- ... -----xxxxxxx----- ... -----------xxxxxxx---------- ... -----|
        //
        // The kernel provides some tolerance for mismatch between the measured AoA and the
        // reference AoA for the matrix geometry. This mismatch can be due to numerous things,
        // including:
        // 
        //  - Different speed of sound (speed varies with temperature and humidity)
        //  - Error in the measured positions of each microphone
        //  - Error in the estimated TDoAs using GCC-PHAT/FCC-PHAT
        //  - Reverberation
        //  - ...
        //

        for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {

            //
            // The synthesized signals hold 182 elements. Elements 1 to 181 represent
            // the energy using the kernels and the AoAs. The element 0 is special, it
            // has a value of 0.0f, and is used to allow the projection to grab a value of
            // 0 when a pair of microphone needs to be ignored. This special case could be
            // implemented using a condition, but this would break vectorization during the
            // search, so we prefer to do it this way instead.
            //

            memset(&(obj->synthesis[index_pair][0]), 0x00, sizeof(float) * 182);

            for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {

                aoa_t aoa = obj->aoas[index_source][index_pair];
                unsigned char index_table = ((unsigned char) roundf(aoa.degree)) + 1;

                obj->synthesis[index_pair][index_table] += aoa.amplitude * obj->kernel[0];

                for (unsigned char index_kernel = 1; index_kernel < obj->kernel_size; index_kernel++) {
                    
                    signed int index_left = ((signed int) index_table) - ((signed int) index_kernel);
                    signed int index_right = ((signed int) index_table) + ((signed int) index_kernel);
                    
                    if (index_left > 0) {
                        obj->synthesis[index_pair][index_left] += aoa.amplitude * obj->kernel[index_kernel];
                    }
                    if (index_right <= 181) {
                        obj->synthesis[index_pair][index_right] += aoa.amplitude * obj->kernel[index_kernel];
                    }

                }

            }            

        }

        //
        // Create the projection (energy for each point)
        //

        for (unsigned int index_point = 0; index_point < obj->num_points; index_point++) {
            
            //
            // Each point is associated to a set of indexes. For instance, for a given
            // point when we have 6 pairs of microphones, we could have something like this:
            //
            // [ 112 |  0  |  4  | 178 | 181 | 100 ]
            //
            // This implies that we add the following values:
            //
            // 1) The sample of the synthesized signal for angle at 111 degrees for pair 1
            // 2) A value of 0 for pair 2 (recall 0 stands for a pair that is ignored)
            // 3) The sample of the synthesized signal for angle at 3 degrees for pair 3
            // 4) The sample of the synthesized signal for angle at 177 degrees for pair 4
            // 5) The sample of the synthesized signal for angle at 180 degrees for pair 5
            // 6) The sample of the synthesized signal for angle at 99 degrees for pair 6
            //
            // In this case, the normalisation should be equal to 0.2 (because there are
            // 5 active elements).
            //

            float energy = 0.0f;
            
            for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
            
                energy += obj->synthesis[index_pair][obj->table[index_point * obj->num_pairs + index_pair]];
            }

            obj->projections[index_point] = energy * obj->norms[index_point];

        }

        //
        // Find the maximum value, and keep the corresponding index
        //

        unsigned int max_index = 0;

        for (unsigned int index_point = 0; index_point < obj->num_points; index_point++) {
            
            if (obj->projections[index_point] > obj->projections[max_index]) {
            
                max_index = index_point;
            
            }

        }

        //
        // Copy the potential source: the point that corresponds to this value is copied as the
        // potential source, and the energy corresponds to the maximum value. Energy is saved
        // as it can later provide useful insights to determine if a source is a true source
        // or a false alarm.
        //

        doas->pots[index_direction] = (pot_t) { .direction = obj->points->points[max_index], .energy = obj->projections[max_index] };

        //
        // Remove this source for next scan
        //
        // The goal here is to find the AoAs that contributed to the maximum value.
        // When an angle is within the kernel interval, its corresponding amplitude is reduced by
        // the energy level. The amplitude cannot be negative, and is set to 0 if this is
        // the case. 
        //
        // For instance, if there are 6 pairs and we found at source which AoAs correspond to:
        // 
        // [ 112 |  0  |  4  | 178 | 181 | 100 ] with a energy level of 0.4, 
        //
        // and the measured AoAs are the following:
        //
        // Source 1: AoA:       [ 110 |  9  |  2  | 140 | 181 | 103 ]
        //           Amplitude:   0.3   0.2   0.2   0.5   0.3   0.4
        //
        // Source 2: AoA:       [ 140 | 100 | 100 | 180 | 101 | 133 ]
        //           Amplitude:   0.2   0.3   0.4   0.5   0.3   0.2
        //
        // This becomes (with a kernel of size 5):
        //
        // Source 1: AoA:       [ 110 |  9  |  2  | 140 | 181 | 103 ]
        //           Amplitude:   0.0   0.2   0.0   0.5   0.0   0.0
        //
        // Source 2: AoA:       [ 140 | 100 | 100 | 180 | 101 | 133 ]
        //           Amplitude:   0.2   0.3   0.4   0.1   0.3   0.2        
        // 

        for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {

            if (obj->table[max_index * obj->num_pairs + index_pair] != 0) {

                float degree = (float) (obj->table[max_index * obj->num_pairs + index_pair] - 1);

                for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {

                    aoa_t aoa = obj->aoas[index_source][index_pair];

                    if (fabsf(aoa.degree - degree) < obj->kernel_size) {

                        float amplitude = aoa.amplitude - obj->projections[max_index];
                        if (amplitude < 0.0f) {
                            amplitude = 0.0f;
                        }

                        obj->aoas[index_source][index_pair].amplitude = amplitude;

                    }

                }

            }

        }

    }

    return 0;

}

void ssl_printf(const ssl_t * obj) {

    for (unsigned int index_point = 0; index_point < obj->num_points; index_point++) {
        
        printf("[%04u]: ", index_point);
        xyz_t point = obj->points->points[index_point];

        printf("(%+1.3f, %+1.3f, %+1.3f) ", point.x, point.y, point.z);
        for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
            printf("%03u ", obj->table[index_point * obj->num_pairs + index_pair]);
        }
        
        printf("> %+1.4f\n", obj->norms[index_point]);
    }

}
