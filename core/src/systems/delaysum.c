#include <systems/delaysum.h>
#include <utils/pi.h>
#include <utils/error.h>

#include <math.h>
#include <stdlib.h>


delaysum_t * delaysum_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins) {
    if (num_sources < 1) {
        odas2_set_error_number(ODAS2_ERROR_DELAYSUM_CONSTRUCT_NUM_SOURCES);
        return NULL;
    }
    if (num_channels < 2) {
        odas2_set_error_number(ODAS2_ERROR_DELAYSUM_CONSTRUCT_NUM_CHANNELS);
        return NULL;
    }
    if (num_bins < 1) {
        odas2_set_error_number(ODAS2_ERROR_DELAYSUM_CONSTRUCT_NUM_BINS);
        return NULL;
    }

    delaysum_t * obj = (delaysum_t *) malloc(sizeof(delaysum_t));

    obj->num_sources = num_sources;
    obj->num_channels = num_channels;
    obj->num_bins = num_bins;

    return obj;

}

void delaysum_destroy(delaysum_t * obj) {

    free(obj);

}

int delaysum_process(delaysum_t * obj, const tdoas_t * tdoas, weights_t * weights) {
    if (obj->num_sources != tdoas->num_sources) {
        odas2_set_error_number(ODAS2_ERROR_DELAYSUM_PROCESS_TDOAS_NUM_SOURCES);
        return -1;
    }
    if (obj->num_sources != weights->num_sources) {
        odas2_set_error_number(ODAS2_ERROR_DELAYSUM_PROCESS_WEIGHTS_NUM_SOURCES);
        return -1;
    }
    if (obj->num_channels != tdoas->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_DELAYSUM_PROCESS_TDOAS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_channels != weights->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_DELAYSUM_PROCESS_WEIGHTS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_bins != weights->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_DELAYSUM_PROCESS_WEIGHTS_NUM_BINS);
        return -1;
    }

    //
    // For each source, compute the weights like this:
    //
    // W(k) = exp(1j * 2 * pi * k * tau / N)
    //
    // where tau is the delay between a channel and channel 0 (used as reference)
    //

    float gain = 1.0f / obj->num_channels;
    unsigned int frame_size = (obj->num_bins - 1) * 2;

    #pragma omp parallel for collapse(2)
    for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {

        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

            float delay = tdoas->taus[index_source][index_channel].delay - tdoas->taus[index_source][0].delay;

            #pragma omp simd
            for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
                weights->bins[index_source][index_channel][index_bin].real = gain * cosf(2.0f * PI_F * index_bin * delay / frame_size);
                weights->bins[index_source][index_channel][index_bin].imag = gain * sinf(2.0f * PI_F * index_bin * delay / frame_size);
            }

        }
    }

    return 0;

}
