#include <systems/delaysum.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

delaysum_t * delaysum_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins) {

    delaysum_t * obj = (delaysum_t *) malloc(sizeof(delaysum_t));

    obj->num_sources = num_sources;
    obj->num_channels = num_channels;
    obj->num_bins = num_bins;

    return obj;

}

void delaysum_destroy(delaysum_t * obj) {

    free(obj);

}

int delaysum_process(delaysum_t * obj, const tdoas_t * tdoas, weights_t * coeffs) {

    //
    // For each source, compute the weights like this:
    //
    // W(k) = exp(1j * 2 * pi * k * tau / N)
    //
    // where tau is the delay between a channel and channel 0 (used as reference)
    //

    float gain = 1.0f / obj->num_channels;
    unsigned int frame_size = (obj->num_bins - 1) * 2;

    for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {
        
        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
            
            float delay = tdoas->taus[index_source][index_channel].delay - tdoas->taus[index_source][0].delay;

            for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
                coeffs->bins[index_source][index_channel][index_bin].real = gain * cosf(2.0f * M_PI * index_bin * delay / frame_size);
                coeffs->bins[index_source][index_channel][index_bin].imag = gain * sinf(2.0f * M_PI * index_bin * delay / frame_size);
            }

        }
    }

    return 0;

}