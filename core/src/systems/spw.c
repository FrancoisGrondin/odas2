#include <systems/spw.h>
#include <utils/error.h>

#include <stdlib.h>
#include <string.h>

spw_t * spw_construct(const unsigned int num_channels, const unsigned int num_bins, const float epsilon, const float gamma) {

    spw_t * obj = (spw_t *) malloc(sizeof(spw_t));

    obj->num_channels = num_channels;
    obj->num_pairs = (unsigned int) (num_channels * (num_channels - 1)) / 2;
    obj->num_bins = num_bins;

    obj->epsilon = epsilon;
    obj->gamma = gamma;

    obj->inv = choleskycplx_construct(obj->num_channels);

    obj->target_acorr = (float *) calloc(obj->num_channels, sizeof(float));
    obj->target_xcorr = (cplx_t *) calloc(obj->num_pairs, sizeof(cplx_t));
    obj->interf_acorr = (float *) calloc(obj->num_channels, sizeof(float));
    obj->interf_xcorr = (cplx_t *) calloc(obj->num_pairs, sizeof(cplx_t));
    obj->interfinv_acorr = (float *) calloc(obj->num_channels, sizeof(float));
    obj->interfinv_xcorr = (cplx_t *) calloc(obj->num_pairs, sizeof(cplx_t));

    obj->A = (cplx_t *) calloc(obj->num_channels * obj->num_channels, sizeof(cplx_t));
    obj->B = (cplx_t *) calloc(obj->num_channels * obj->num_channels, sizeof(cplx_t));
    obj->C = (cplx_t *) calloc(obj->num_channels * obj->num_channels, sizeof(cplx_t));

    return obj;

}

void spw_destroy(spw_t * obj) {

    free(obj->A);
    free(obj->B);
    free(obj->C);

    free(obj->target_acorr);
    free(obj->target_xcorr);
    free(obj->interf_acorr);
    free(obj->interf_xcorr);
    free(obj->interfinv_acorr);
    free(obj->interfinv_xcorr);

    choleskycplx_destroy(obj->inv);

    free((void *) obj);

}

int spw_process(spw_t * obj, const covs_t * target, const covs_t * interf, covs_t * whitened) {

    for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

        //
        // Copy values
        //

        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
            obj->target_acorr[index_channel] = target->acorrs[index_channel][index_bin];
            obj->interf_acorr[index_channel] = interf->acorrs[index_channel][index_bin];
        }

        for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
            obj->target_xcorr[index_pair] = target->xcorrs[index_pair][index_bin];
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
        // Copy values (A = inv(interf), B = target)
        //

        {

            unsigned int index_pair = 0;

            for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {

                for (unsigned int index_channel2 = (index_channel1 + 1); index_channel2 < obj->num_channels; index_channel2++) {

                    obj->A[index_channel1 * obj->num_channels + index_channel2] = obj->interfinv_xcorr[index_pair];
                    obj->A[index_channel2 * obj->num_channels + index_channel1] = cplx_conj(obj->interfinv_xcorr[index_pair]);

                    obj->B[index_channel1 * obj->num_channels + index_channel2] = obj->target_xcorr[index_pair];
                    obj->B[index_channel2 * obj->num_channels + index_channel1] = cplx_conj(obj->target_xcorr[index_pair]);

                    index_pair++;

                }

            }

            for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

                obj->A[index_channel * obj->num_channels + index_channel] = cplx_cst(obj->interfinv_acorr[index_channel], 0.0f);
                obj->B[index_channel * obj->num_channels + index_channel] = cplx_cst(obj->target_acorr[index_channel], 0.0f);

            }

        }

        //
        // Matrix multiplication: C = A @ B
        //

        for (unsigned int index_row = 0; index_row < obj->num_channels; index_row++) {

            for (unsigned int index_col = 0; index_col < obj->num_channels; index_col++) {

                cplx_t c = cplx_cst(0.0f, 0.0f);

                for (unsigned int index_element = 0; index_element < obj->num_channels; index_element++) {

                    cplx_t a = obj->A[index_row * obj->num_channels + index_element];
                    cplx_t b = obj->B[index_element * obj->num_channels + index_col];
                    c = cplx_add(c, cplx_mul(a, b));
                    
                }

                obj->C[index_row * obj->num_channels + index_col] = c;

            }

        }

        //
        // Extract values and compute simultaneously Phi = (1/2) * (C + C^H)
        //

        {

            unsigned int index_pair = 0;

            for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {

                for (unsigned int index_channel2 = (index_channel1 + 1); index_channel2 < obj->num_channels; index_channel2++) {

                    cplx_t c1 = obj->C[index_channel1 * obj->num_channels + index_channel2];
                    cplx_t c2 = obj->C[index_channel2 * obj->num_channels + index_channel1];
                    whitened->xcorrs[index_pair][index_bin] = cplx_mul(cplx_cst(0.5f, 0.0f), cplx_add(c1, cplx_conj(c2)));

                    index_pair++;

                }

            }

            for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

                whitened->acorrs[index_channel][index_bin] = obj->C[index_channel * obj->num_channels + index_channel].real;

            }

        }

    }

    return 0;

}
