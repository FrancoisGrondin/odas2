#include <signals/covs.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

covs_t * covs_construct(const char * label, const unsigned int num_channels, const unsigned int num_bins) {

    covs_t * obj = (covs_t *) malloc(sizeof(covs_t));

    memset(obj->label, 0x00, 64);
    strcpy(obj->label, label);

    obj->num_channels = num_channels;
    obj->num_pairs = num_channels * (num_channels-1) / 2;
    obj->num_bins = num_bins;

    obj->xcorrs = (cplx_t **) malloc(sizeof(cplx_t *) * obj->num_pairs);
    for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
        obj->xcorrs[index_pair] = (cplx_t *) calloc(sizeof(cplx_t), num_bins);
    }

    obj->acorrs = (float **) malloc(sizeof(float *) * obj->num_channels);
    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        obj->acorrs[index_channel] = (float *) calloc(sizeof(float), num_bins);
    }

    return obj;

}

void covs_destroy(covs_t * obj) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        free(obj->acorrs[index_channel]);
    }
    free(obj->acorrs);

    for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
        free(obj->xcorrs[index_pair]);
    }
    free(obj->xcorrs);

    free(obj);

}

void covs_fprintf(const covs_t * obj, FILE * fp) {

    for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

        fprintf(fp, "Bin index = %u\n", index_bin);

        for (unsigned int index_row = 0; index_row < obj->num_channels; index_row++) {
            
            for (unsigned int index_col = 0; index_col < obj->num_channels; index_col++) {

                cplx_t value;

                if (index_row == index_col) {

                    unsigned int index_acorr = index_row;
                    value = cplx_cst(obj->acorrs[index_acorr][index_bin], 0.0f);

                }
                if (index_row < index_col) {

                    unsigned int index_xcorr = index_row * obj->num_channels - (index_row * (index_row + 1)) / 2 + (index_col - index_row - 1);
                    value = obj->xcorrs[index_xcorr][index_bin];

                }
                if (index_row > index_col) {

                    unsigned int index_xcorr = index_col * obj->num_channels - (index_col * (index_col + 1)) / 2 + (index_row - index_col - 1);
                    value = cplx_conj(obj->xcorrs[index_xcorr][index_bin]);

                }

                fprintf(fp, "(%+8.2f + j %+8.2f) ", value.real, value.imag);

            }

            fprintf(fp, "\n");

        }

        fprintf(fp, "\n");

    }

}