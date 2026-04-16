#include <systems/spw.h>
#include <utils/error.h>

#include <stdlib.h>
#include <string.h>

spw_t * spw_construct(const unsigned int num_channels, const unsigned int num_bins) {

    spw_t * obj = (spw_t *) malloc(sizeof(spw_t));

    obj->num_channels = num_channels;
    obj->num_pairs = (unsigned int) (num_channels * (num_channels - 1)) / 2;
    obj->num_bins = num_bins;

    obj->NNinv = (cplx_t *) calloc(obj->num_channels * obj->num_channels, sizeof(cplx_t));
    obj->SS = (cplx_t *) calloc(obj->num_channels * obj->num_channels, sizeof(cplx_t));
    obj->C = (cplx_t *) calloc(obj->num_channels * obj->num_channels, sizeof(cplx_t));

    return obj;

}

void spw_destroy(spw_t * obj) {

    free(obj->NNinv);
    free(obj->SS);
    free(obj->C);

    free((void *) obj);

}

int spw_process(spw_t * obj, const covs_t * target, const covs_t * invinterf, covs_t * whitened) {

    for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

        //
        // Copy values
        //

        {

            unsigned int index_pair = 0;

            for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {

                for (unsigned int index_channel2 = (index_channel1 + 1); index_channel2 < obj->num_channels; index_channel2++) {

                    obj->NNinv[index_channel1 * obj->num_channels + index_channel2] = invinterf->xcorrs[index_pair][index_bin];
                    obj->NNinv[index_channel2 * obj->num_channels + index_channel1] = cplx_conj(invinterf->xcorrs[index_pair][index_bin]);

                    obj->SS[index_channel1 * obj->num_channels + index_channel2] = target->xcorrs[index_pair][index_bin];
                    obj->SS[index_channel2 * obj->num_channels + index_channel1] = cplx_conj(target->xcorrs[index_pair][index_bin]);

                    index_pair++;

                }

            }

            for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

                obj->NNinv[index_channel * obj->num_channels + index_channel] = cplx_cst(invinterf->acorrs[index_channel][index_bin], 0.0f);
                obj->SS[index_channel * obj->num_channels + index_channel] = cplx_cst(target->acorrs[index_channel][index_bin], 0.0f);

            }

        }

        //
        // Matrix multiplication: C = NNinv @ SS
        //

        for (unsigned int index_row = 0; index_row < obj->num_channels; index_row++) {

            for (unsigned int index_col = 0; index_col < obj->num_channels; index_col++) {

                cplx_t c = cplx_cst(0.0f, 0.0f);

                for (unsigned int index_element = 0; index_element < obj->num_channels; index_element++) {

                    cplx_t nninv = obj->NNinv[index_row * obj->num_channels + index_element];
                    cplx_t ss = obj->SS[index_element * obj->num_channels + index_col];
                    c = cplx_add(c, cplx_mul(nninv, ss));
                    
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
