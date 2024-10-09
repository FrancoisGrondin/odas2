#include <systems/phat.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

phat_t * phat_construct(const unsigned int num_channels, const unsigned int num_bins) {

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

    for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
        for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
            covs_out->xcorrs[index_pair][index_bin] = cplx_unit(covs_in->xcorrs[index_pair][index_bin]);
        }
    }

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
            covs_out->acorrs[index_channel][index_bin] = 1.0f;
        }
    }

    return 0;

}