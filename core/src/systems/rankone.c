#include <systems/rankone.h>
#include <utils/error.h>

rankone_t * rankone_construct(const unsigned int num_channels, const unsigned int num_bins, const unsigned int num_iterations) {

    rankone_t * obj = (rankone_t *) malloc(sizeof(rankone_t));

    obj->num_channels = num_channels;
    obj->num_pairs = num_channels * (num_channels-1) / 2;
    obj->num_bins = num_bins;
    obj->num_iterations = num_iterations;

    obj->A = (cplx_t *) calloc(obj->num_channels * obj->num_channels, sizeof(cplx_t));
    obj->bk = (cplx_t *) calloc(obj->num_channels, sizeof(cplx_t));
    obj->bkp1 = (cplx_t *) calloc(obj->num_channels, sizeof(cplx_t));
    obj->bs = (cplx_t *) calloc(obj->num_channels * obj->num_bins, sizeof(cplx_t));

    for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
        obj->bs[index_bin * num_channels] = cplx_cst(1.0f, 0.0f);
    }

    return obj;

}

void rankone_destroy(rankone_t * obj) {

    free((void *) obj->A);
    free((void *) obj->bk);
    free((void *) obj->bkp1);
    free((void *) obj->bs);

    free((void *) obj);

}

int rankone_process(rankone_t * obj, const covs_t * full, const covs_t * proj) {

    const float eps = 1e-20f;

    for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

        //
        // Load values in matrix A
        //

        {

            for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
                obj->A[index_channel * obj->num_channels + index_channel] = cplx_cst(full->acorrs[index_channel][index_bin], 0.0f);
            }

            unsigned int index_pair = 0;

            for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {
                
                for (unsigned int index_channel2 = (index_channel1+1); index_channel2 < obj->num_channels; index_channel2++) {
                
                    obj->A[index_channel1 * obj->num_channels + index_channel2] = full->xcorrs[index_pair][index_bin];
                    index_pair++;
                
                }

            }

        }

        //
        // Load values in vector b
        //

        {

            for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
                obj->bk[index_channel] = obj->bs[index_bin * obj->num_channels + index_channel];
            }

        }

        //
        // Perform b_k+1 = A @ b_k / || A @ b_k ||, then b_k = b_k+1
        //

        {

            for (unsigned int index_iteration = 0; index_iteration < obj->num_iterations; index_iteration++) {

                float norm = 0.0f;

                for (unsigned int index_row = 0; index_row < obj->num_channels; index_row++) {

                    cplx_t sum = cplx_cst(0.0f, 0.0f);

                    for (unsigned int index_col = 0; index_col < obj->num_channels; index_col++) {

                        sum = cplx_add(sum, cplx_mul(obj->A[index_row * obj->num_channels + index_col], obj->bk[index_col]));

                    }

                    obj->bkp1[index_row] = sum;

                    norm += cplx_l2(sum);

                }

                norm = sqrtf(norm);

                for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
                    obj->bk[index_channel] = cplx_mul(cplx_cst(1.0f/(norm + eps), 0.0f), obj->bkp1[index_channel]);
                }

            }

        }

        //
        // Compute lambda = b_k^H @ A @ b_k
        //

        float lambda = 0.0f;

        {

            for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {

                cplx_t sum = cplx_cst(0.0f, 0.0f);

                for (unsigned int index_channel2 = 0; index_channel2 < obj->num_channels; index_channel2++) {

                    sum = cplx_add(sum, cplx_mul(cplx_conj(obj->bk[index_channel2]), obj->A[index_channel2 * obj->num_channels + index_channel1]));

                }

                lambda += cplx_mul(sum, obj->bk[index_channel1]).real;

            }

        }

        //
        // Save b_k for later
        //

        {

            for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
                obj->bs[index_bin * obj->num_channels + index_channel] = obj->bk[index_channel];
            }

        }

        //
        // Generate projection matrix: proj = lambda * b @ b^H
        //

        {

            for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
                proj->acorrs[index_channel][index_bin] = cplx_l2(obj->bk[index_channel]);
            }

            unsigned int index_pair = 0;

            for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {

                for (unsigned int index_channel2 = 0; index_channel2 < obj->num_channels; index_channel2++) {

                    proj->xcorrs[index_pair][index_pair] = cplx_mul(cplx_cst(lambda, 0.0f), cplx_mul(obj->bk[index_channel1], cplx_conj(obj->bk[index_channel2])));

                }

            }

        }

    }

    return 0;

}