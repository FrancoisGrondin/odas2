#include "test_delaysum.h"

int test_delaysum(void) {

    const float eps = 0.001f;

    {

        const unsigned int num_channels = 4;
        const unsigned int num_pairs = 6;
        const unsigned int num_sources = 3;
        const unsigned int num_bins = 9;

        const float delays[3][6] = { { -1.4927f, -2.9854f, -1.4927f, -1.4927f, +0.0000f, +1.4927f },
                                     { +1.4927f, +0.0000f, -1.4927f, -1.4927f, -2.9854f, -1.4927f },
                                     { +0.0000f, +2.1107f, +2.1107f, +2.1107f, +2.1107f, +0.0000f } };

        tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);
        weights_t * weights = weights_construct("Ws", num_sources, num_channels, num_bins);
        delaysum_t * delaysum = delaysum_construct(num_sources, num_channels, num_bins);

        for (unsigned int index_source = 0; index_source < num_sources; index_source++) {
            for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
                tdoas->taus[index_source][index_pair] = (tau_t) { .delay = delays[index_source][index_pair], .amplitude = 1.0f };
            }
        }

        delaysum_process(delaysum, tdoas, weights);

        for (unsigned int index_source = 0; index_source < num_sources; index_source++) {
            for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
                
                float delay = delays[index_source][index_channel] - delays[index_source][0];
                
                for (unsigned int index_bin = 0; index_bin < num_bins; index_bin++) {
                
                    float target_real = (1.0f / num_channels) * cosf(2.0f * M_PI * index_bin * delay / ((num_bins - 1) * 2));
                    float target_imag = (1.0f / num_channels) * sinf(2.0f * M_PI * index_bin * delay / ((num_bins - 1) * 2));
                
                    if (!(fabsf(weights->bins[index_source][index_channel][index_bin].real - target_real) < eps)) {
                        return -1;
                    }
                    if (!(fabsf(weights->bins[index_source][index_channel][index_bin].imag - target_imag) < eps)) {
                        return -1;
                    }                

                }
            }
        }

        tdoas_destroy(tdoas);
        weights_destroy(weights);
        delaysum_destroy(delaysum);

    }

    return 0;

}