#include <systems/gcc.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

gcc_t * gcc_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins) {

    gcc_t * obj = (gcc_t *) malloc(sizeof(gcc_t));

    obj->num_sources = num_sources;
    obj->num_channels = num_channels;
    obj->num_pairs = num_channels * (num_channels - 1) / 2;
    obj->num_bins = num_bins;
    obj->num_samples = (num_bins - 1) * 2;
    obj->interpolation_factor = 2;

    obj->fft = fft_construct(obj->num_samples * obj->interpolation_factor);

    obj->XX = (cplx_t *) calloc(sizeof(cplx_t), (obj->num_bins - 1) * obj->interpolation_factor + 1);
    obj->xx = (float *) calloc(sizeof(float), obj->num_samples * obj->interpolation_factor);
    
    return obj;

}

void gcc_destroy(gcc_t * obj) {

    free(obj->xx);
    free(obj->XX);

    fft_destroy(obj->fft);

    free(obj);

}

int gcc_process(gcc_t * obj, const covs_t * covs, tdoas_t * tdoas) {

    //
    // The number of samples for the iFFT corresponds to kN, where N is the 
    // frame size and k is the interpolation rate.
    //
    unsigned int num_samples_interp = obj->num_samples * obj->interpolation_factor;
    
    //
    // Due to interpolation, we have fractional sample spaced by (1/k).
    // For instance, if k=2, we get 0.0, 0.5, 1.0, 1.5, ... such thate delta_tau = 0.5.
    //
    float delta_tau = 1.0f / ((float) obj->interpolation_factor);
    
    //
    // This is the scale factor to ensure the maximum value is 1.0 after iFFT.
    // This is required because the extra bins after interpolation are set to 0, and this
    // needs to be accounted for.
    //
    // Given the number of bins F = (N/2)+1, we get scale = ((F-1) * k + 1) / ((F-1) * k * 2).
    // For instance, if N = 512 and k = 2, we get F = 257, and 
    // scale = ((257-1) * 2 + 1) / ((257-1) * 2 * 2) = 513 / 1024.
    // In this case, the max value would be 513/1024, and thus dividing by scale brings this to 1.
    // 
    float scale = ((float) (obj->num_bins - 1) * obj->interpolation_factor + 1) / ((float) (obj->num_bins - 1) * obj->interpolation_factor * 2);

    //
    // Compute GCC for each pair
    //
    for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
        
        // 
        // Set all values to 0. For instance, with F = 257 and k = 2:
        //
        // [  0  |  0  |  0  |  0  | ... |  0  ]
        //   (0)   (1)   (2)   (3)   ...  (512)
        //
        memset(obj->XX, 0x00, sizeof(cplx_t) * ((obj->num_bins - 1) * obj->interpolation_factor + 1));
        
        //
        // Then load the coefficients:
        //
        // [  X  |  X  |  X  |  X  | ... |  X  |  0  | ... |  0  ]
        //   (0)   (1)   (2)   (3)   ...  (256) (257)  ...  (512)
        //
        memcpy(obj->XX, covs->xcorrs[index_pair], sizeof(cplx_t) * obj->num_bins);

        //
        // Perform iFFT
        //
        // [  x  |  x  |  x  | ... |  x  ]
        //   (0)   (1)   (2)        (1023)
        //
        fft_irfft(obj->fft, obj->XX, obj->xx);

        // 
        // Scan for multiple peaks in this cross-correlation result
        //
        for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {

            // 
            // Find maximum value
            //

            float max_value = 0.0f;
            unsigned int max_index = 0;

            for (unsigned int index_sample = 0; index_sample < num_samples_interp; index_sample++) {
                if (obj->xx[index_sample] > max_value) {
                    max_value = obj->xx[index_sample];
                    max_index = index_sample;
                }
            }

            //
            // Convert index to tau value in samples.
            // For instance, if max occurs at index 11 with interpolation of k = 2, then
            // the real tau value is 5.5 (11/2).
            // If there are 1024 elements and the max occurs at index 1021 with interpolation
            // k = 2, then the real tau value is -1.5 (1021-1024)/2.
            //
            float tau_max;

            if (max_index >= (num_samples_interp / 2)) {
                tau_max = (((float) max_index) - ((float) num_samples_interp)) / ((float) obj->interpolation_factor);
            }
            else {
                tau_max = ((float) max_index) / ((float) obj->interpolation_factor);
            }

            // 
            // Get the left and right values next to the maximum to perform quadratic interpolation.
            //
            unsigned int max_index_right = (unsigned int) (((signed int) max_index + 1) % num_samples_interp);
            unsigned int max_index_left = (unsigned int) (((signed int) max_index - 1) % num_samples_interp);

            //
            // Quadratic interpolation:
            //
            // a (-delta_tau)^2 + b (-delta_tau) + c = y_prev
            // a (0)^2          + b (0)          + c = y_max
            // a (+delta_tau)^2 + b (+delta_tau) + c = y_next
            // 
            // Solve for a, b, and c, and then get the maximum tau value = tau_max + -b/(2*a), and the
            // maximum amplitude value = c - (b^2)/(4a). The equations used here have been simplified
            // by substituting values of a, b, and c using variables delta_tau, y_prev, y_max and y_next.
            // Note the scaling by (1/scale) to make sure the maximum value is 1.0. 
            //
            float y_prev = obj->xx[max_index_left];
            float y_max = obj->xx[max_index];
            float y_next = obj->xx[max_index_right];

            // Avoid division by zero
            float denominator = y_prev - 2.0f * y_max + y_next;
            if (fabsf(denominator) < 1e-20f) {
                denominator = 1e-20f;
            }

            float tau_hat = tau_max + (delta_tau / 2.0f) * (y_prev - y_next) / denominator;
            float amp_hat = (1/scale) * (y_max - (1.0f / 8.0f) * (y_next * y_next + y_prev * y_prev - 2.0f * y_prev * y_next) / denominator);

            tdoas->taus[index_source][index_pair].delay = tau_hat;
            tdoas->taus[index_source][index_pair].amplitude = amp_hat;

            // 
            // If there are other maximum to find, we first need to remove the previously found
            // maximum value. To do that, scan each side of the curve, and set samples to zeros
            // as long as the slope is negative. This allows to reset peaks with large lobes, which
            // can happen when most of the energy lies in low frequencies.
            //
            if ((index_source + 1) < obj->num_sources) {

                {
                    float y = INFINITY;
                    for (unsigned int shift_left = 1; shift_left < (num_samples_interp / 2 - 1); shift_left++) {
                        unsigned int index_left = (unsigned int) (((signed int) max_index - (signed int) shift_left) % num_samples_interp);
                        if (obj->xx[index_left] < y) {
                            y = obj->xx[index_left];
                            obj->xx[index_left] = 0.0f;
                        }
                        else {
                            break;
                        }
                    }
                }

                {
                    float y = INFINITY;
                    for (unsigned int shift_right = 1; shift_right < (num_samples_interp / 2 - 1); shift_right++) {
                        unsigned int index_right = (unsigned int) (((signed int) max_index + (signed int) shift_right) % num_samples_interp);
                        if (obj->xx[index_right] < y) {
                            y = obj->xx[index_right];
                            obj->xx[index_right] = 0.0f;
                        }
                        else {
                            break;
                        }
                    }
                }

                obj->xx[max_index] = 0.0f;

            }

        }

    }

    return 0;

}