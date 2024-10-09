#include <systems/beamformer.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

beamformer_t * beamformer_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins) {

    beamformer_t * obj = (beamformer_t *) malloc(sizeof(beamformer_t));

    obj->num_sources = num_sources;
    obj->num_channels = num_channels;
    obj->num_bins = num_bins;

    return obj;

}

void beamformer_destroy(beamformer_t * obj) {

    free(obj);

}

int beamformer_process(beamformer_t * obj, const freqs_t * in, const weights_t * weights, freqs_t * out) {

    for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {

        memset(out->bins[index_source], 0x00, sizeof(cplx_t) * obj->num_bins);

        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

            for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

                cplx_t coeff = cplx_conj(weights->bins[index_source][index_channel][index_bin]);
                cplx_t bin_in = in->bins[index_channel][index_bin];
                cplx_t bin_out = out->bins[index_source][index_bin];

                out->bins[index_source][index_bin] = cplx_add(cplx_mul(coeff, bin_in), bin_out);

            }

        }

    }

    return 0;

}