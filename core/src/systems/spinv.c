#include <systems/spinv.h>
#include <utils/error.h>

#include <stdlib.h>
#include <string.h>

spinv_t * spinv_construct(const unsigned int num_channels, const unsigned int num_bins, const float epsilon, const float gamma) {

    spinv_t * obj = (spinv_t *) malloc(sizeof(spinv_t));

    obj->num_channels = num_channels;
    obj->num_pairs = (unsigned int) (num_channels * (num_channels - 1)) / 2;
    obj->num_bins = num_bins;

    obj->epsilon = epsilon;
    obj->gamma = gamma;

    obj->inv = choleskycplx_construct(obj->num_channels);

    obj->interf_acorr = (float *) calloc(obj->num_channels, sizeof(float));
    obj->interf_xcorr = (cplx_t *) calloc(obj->num_pairs, sizeof(cplx_t));
    obj->interfinv_acorr = (float *) calloc(obj->num_channels, sizeof(float));
    obj->interfinv_xcorr = (cplx_t *) calloc(obj->num_pairs, sizeof(cplx_t));

    return obj;

}

void spinv_destroy(spinv_t * obj) {

    free(obj->interf_acorr);
    free(obj->interf_xcorr);
    free(obj->interfinv_acorr);
    free(obj->interfinv_xcorr);

    choleskycplx_destroy(obj->inv);

    free((void *) obj);

}

int spinv_process(spinv_t * obj, const covs_t * interf, covs_t * invinterf) {

    for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

        //
        // Copy values
        //

        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
            obj->interf_acorr[index_channel] = interf->acorrs[index_channel][index_bin];
        }

        for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
            obj->interf_xcorr[index_pair] = interf->xcorrs[index_pair][index_bin];
        }

        //
        // Diagonal loading on interference matrix
        //

        float power = 0.0f;
        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
            power += obj->interf_acorr[index_channel];
        }
        power /= (float) obj->num_channels;
        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
            obj->interf_acorr[index_channel] += obj->gamma * power + obj->epsilon;
        }

        //
        // Invert interference matrix
        //

        choleskycplx_process(obj->inv, obj->interf_acorr, obj->interf_xcorr, obj->interfinv_acorr, obj->interfinv_xcorr);

        //
        // Copy values
        //

        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        	invinterf->acorrs[index_channel][index_bin] = obj->interfinv_acorr[index_channel];
        }

        for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
        	invinterf->xcorrs[index_pair][index_bin] = obj->interfinv_xcorr[index_pair];
        }

    }

    return 0;

}