#include <systems/phat.h>
#include <utils/error.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

phat_t * phat_construct(const unsigned int num_channels, const unsigned int num_bins) {

    if (num_channels < 2) {
        odas2_set_error_number(ODAS2_ERROR_PHAT_CONSTRUCT_NUM_CHANNELS);
        return NULL;
    }
    if (num_bins < 1) {
        odas2_set_error_number(ODAS2_ERROR_PHAT_CONSTRUCT_NUM_BINS);
        return NULL;
    }

    phat_t * obj = (phat_t *) malloc(sizeof(phat_t));

    obj->num_channels = num_channels;
    obj->num_pairs = num_channels * (num_channels - 1) / 2;
    obj->num_bins = num_bins;

    return obj;

}

void phat_destroy(phat_t * obj) {

    free((void *) obj);

}

int phat_process(phat_t * obj, const covs_t * covs_in, covs_t * covs_out) {

    if (obj->num_channels != covs_in->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_PHAT_PROCESS_COVS_IN_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_channels != covs_out->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_PHAT_PROCESS_COVS_OUT_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_bins != covs_in->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_PHAT_PROCESS_COVS_IN_NUM_BINS);
        return -1;
    }
    if (obj->num_bins != covs_out->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_PHAT_PROCESS_COVS_OUT_NUM_BINS);
        return -1;
    }

    #pragma omp parallel
    {
        #pragma omp for collapse(2)
        for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
            for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
                covs_out->xcorrs[index_pair][index_bin] = cplx_unit(covs_in->xcorrs[index_pair][index_bin]);
            }
        }

        #pragma omp for collapse(2)
        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
            for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
                covs_out->acorrs[index_channel][index_bin] = 1.0f;
            }
        }
    }

    return 0;

}
