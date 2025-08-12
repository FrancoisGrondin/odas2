#include <systems/beamformer.h>
#include <utils/error.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

beamformer_t * beamformer_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins) {
    if (num_sources < 1) {
        odas2_set_error_number(ODAS2_ERROR_BEAMFORMER_CONSTRUCT_NUM_SOURCES);
        return NULL;
    }
    if (num_channels < 1) {
        odas2_set_error_number(ODAS2_ERROR_BEAMFORMER_CONSTRUCT_NUM_CHANNELS);
        return NULL;
    }
    if (num_bins < 1) {
        odas2_set_error_number(ODAS2_ERROR_BEAMFORMER_CONSTRUCT_NUM_BINS);
        return NULL;
    }

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
    if (obj->num_sources != weights->num_sources) {
        odas2_set_error_number(ODAS2_ERROR_BEAMFORMER_PROCESS_WEIGHTS_NUM_SOURCES);
        return -1;
    }
    if (obj->num_sources != out->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_BEAMFORMER_PROCESS_OUT_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_channels != in->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_BEAMFORMER_PROCESS_IN_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_channels != weights->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_BEAMFORMER_PROCESS_WEIGHTS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_bins != in->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_BEAMFORMER_PROCESS_IN_NUM_BINS);
        return -1;
    }
    if (obj->num_bins != weights->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_BEAMFORMER_PROCESS_WEIGHTS_NUM_BINS);
        return -1;
    }
    if (obj->num_bins != out->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_BEAMFORMER_PROCESS_OUT_NUM_BINS);
        return -1;
    }


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
