#include <systems/scm.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

scm_t * scm_construct(const unsigned int num_channels, const unsigned int num_bins, const float alpha) {

    scm_t * obj = (scm_t *) malloc(sizeof(scm_t));

    obj->num_channels = num_channels;
    obj->num_pairs = num_channels * (num_channels-1) / 2;
    obj->num_bins = num_bins;
    obj->alpha = alpha;

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

void scm_destroy(scm_t * obj) {

    for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
        free(obj->xcorrs[index_pair]);
    }
    free(obj->xcorrs);

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        free(obj->acorrs[index_channel]);
    }
    free(obj->acorrs);

    free(obj);

}

int scm_process(scm_t * obj, const freqs_t * freqs, const masks_t * masks, covs_t * covs) {

    unsigned int index_pair = 0;

    for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {

        for (unsigned int index_channel2 = (index_channel1 + 1); index_channel2 < obj->num_channels; index_channel2++) {

            for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

                cplx_t xcorr = obj->xcorrs[index_pair][index_bin];
                cplx_t xspec = cplx_mul(freqs->bins[index_channel1][index_bin], cplx_conj(freqs->bins[index_channel2][index_bin]));
                
                float gain = masks->gains[index_channel1][index_bin] * masks->gains[index_channel2][index_bin];
                float alpha = obj->alpha * gain;

                xcorr = cplx_add(cplx_mul(cplx_cst(1.0f - alpha, 0.0f), xcorr), cplx_mul(cplx_cst(alpha, 0.0f), xspec));
                obj->xcorrs[index_pair][index_bin] = xcorr;

            }

            memcpy(covs->xcorrs[index_pair], obj->xcorrs[index_pair], sizeof(cplx_t) * obj->num_bins);

        index_pair++;

        }

    }

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

        for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

            float acorr = obj->acorrs[index_channel][index_bin];
            float aspec = cplx_l2(freqs->bins[index_channel][index_bin]);

            float gain = masks->gains[index_channel][index_bin] * masks->gains[index_channel][index_bin];
            float alpha = obj->alpha * gain;

            acorr = (1.0f - alpha) * acorr + alpha * aspec;
            obj->acorrs[index_channel][index_bin] = acorr;

        }

        memcpy(covs->acorrs[index_channel], obj->acorrs[index_channel], sizeof(float) * obj->num_bins);

    }

    return 0;

}

iscm_t * iscm_construct(const unsigned int num_channels, const unsigned int num_bins, const float alpha) {

    iscm_t * obj = (iscm_t *) malloc(sizeof(iscm_t));

    obj->num_channels = num_channels;
    obj->num_pairs = num_channels * (num_channels-1) / 2;
    obj->num_bins = num_bins;
    obj->alpha = alpha;

    obj->xcorrs = (cplx_t **) malloc(sizeof(cplx_t *) * obj->num_pairs);
    for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
        obj->xcorrs[index_pair] = (cplx_t *) calloc(sizeof(cplx_t), num_bins);
    }

    obj->acorrs = (float **) malloc(sizeof(float *) * obj->num_channels);
    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        obj->acorrs[index_channel] = (float *) calloc(sizeof(float), num_bins);
        for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
            obj->acorrs[index_channel][index_bin] = 1.0f;
        }
    }

    obj->phiInv = cmat_construct(num_channels, num_channels);
    obj->phiInvSym = hmat_construct(num_channels);
    obj->X = cmat_construct(num_channels, 1);
    obj->XH = cmat_construct(1, num_channels);
    obj->phiInv_X = cmat_construct(num_channels, 1);
    obj->phiInv_X_XH_phiInvH = hmat_construct(num_channels);
    obj->XH_phiInv_X = cmat_construct(1, 1);
    obj->beta_phiInv = hmat_construct(num_channels);
    obj->gamma_phiInv_X_XH_phiInvH = hmat_construct(num_channels);
    obj->phiInvNew = hmat_construct(num_channels);

    return obj;

}

void iscm_destroy(iscm_t * obj) {

    for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {
        free(obj->xcorrs[index_pair]);
    }
    free(obj->xcorrs);

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        free(obj->acorrs[index_channel]);
    }
    free(obj->acorrs);

    cmat_destroy(obj->phiInv);
    hmat_destroy(obj->phiInvSym);
    cmat_destroy(obj->X);
    cmat_destroy(obj->XH);
    cmat_destroy(obj->phiInv_X);
    hmat_destroy(obj->phiInv_X_XH_phiInvH);
    cmat_destroy(obj->XH_phiInv_X);
    hmat_destroy(obj->beta_phiInv);
    hmat_destroy(obj->gamma_phiInv_X_XH_phiInvH);
    hmat_destroy(obj->phiInvNew);

    free(obj);

}

int iscm_process(iscm_t * obj, const freqs_t * freqs, const masks_t * masks, covs_t * covs) {

    //
    // For each frequency bin, we want to compute the following equation (based on 
    // the Woodburry identify):
    //
    // beta * phiInv - gamma * (phiInv @ X) @ (phiInv @ X)^H
    //
    // where beta and gamma are scalars:
    //
    // beta = 1 / (1 - alpha)
    // 
    // gamma = beta^2 * [1 / (1/alpha + X^H @ phiInv @ X)]
    //

    for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

        // phiInv (MxM)
        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
            obj->phiInv->elements[index_channel * obj->num_channels + index_channel] = cplx_cst(obj->acorrs[index_channel][index_bin], 0.0f);
        }
        {
            unsigned int index_pair = 0;
            for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {
                for (unsigned int index_channel2 = (index_channel1+1); index_channel2 < obj->num_channels; index_channel2++) {
                    obj->phiInv->elements[index_channel1 * obj->num_channels + index_channel2] = obj->xcorrs[index_pair][index_bin];
                    obj->phiInv->elements[index_channel2 * obj->num_channels + index_channel1] = cplx_conj(obj->xcorrs[index_pair][index_bin]);
                    index_pair++;
                }
            }
        }
        linalg_cmat2hmat(obj->phiInvSym, obj->phiInv);

        //cmat_printf(obj->phiInv);
        hmat_printf(obj->phiInvSym);

        // X (Mx1)
        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
            obj->X->elements[index_channel] = freqs->bins[index_channel][index_bin];
            obj->X->elements[index_channel].real *= masks->gains[index_channel][index_bin];
            obj->X->elements[index_channel].imag *= masks->gains[index_channel][index_bin];
        }

        cmat_printf(obj->X);
                
        // X^H (1xM)
        cmat_htranspose(obj->XH, obj->X);
        //cmat_printf(obj->XH);

        // phiInv_X (Mx1)
        cmat_mul(obj->phiInv_X, obj->phiInv, obj->X);
        //cmat_printf(obj->phiInv_X);
        
        // phiInv_X_XH_phiInvH (MxM)
        hmat_rank1(obj->phiInv_X_XH_phiInvH, obj->phiInv_X);
        //hmat_printf(obj->phiInv_X_XH_phiInvH);

        // XH_phiInv_X (1x1)
        cmat_mul(obj->XH_phiInv_X, obj->XH, obj->phiInv_X);
        cmat_printf(obj->XH_phiInv_X);

        // beta
        float beta = 1.0f / (1.0f - obj->alpha);
        printf("beta = %f\n", beta);

        // gamma
        float gamma = beta * beta * 1.0f / (1.0f / obj->alpha + beta * obj->XH_phiInv_X->elements[0].real);
        printf("gamma = %f\n", gamma);

        exit(0);

        // beta * phiInv (MxM)
        hmat_scale(obj->beta_phiInv, obj->phiInvSym, beta);
        hmat_printf(obj->beta_phiInv);

        // gamma * (phiInv @ X) @ (phiInv @ X)^H (MxM)
        hmat_scale(obj->gamma_phiInv_X_XH_phiInvH, obj->phiInv_X_XH_phiInvH, gamma);
        hmat_printf(obj->gamma_phiInv_X_XH_phiInvH);

        // beta * phiInv - gamma * (phiInv @ X) @ (phiInv @ X)^H (MxM)
        hmat_sub(obj->phiInvNew, obj->beta_phiInv, obj->gamma_phiInv_X_XH_phiInvH);
        hmat_printf(obj->phiInvNew);
        exit(0);

        // Copy result
        for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
            obj->acorrs[index_channel][index_bin] = obj->phiInvNew->diag[index_channel];
        }
        {
            unsigned int index_pair = 0;
            for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {
                for (unsigned int index_channel2 = (index_channel1+1); index_channel2 < obj->num_channels; index_channel2++) {
                    obj->xcorrs[index_pair][index_bin] = obj->phiInvNew->utri[index_pair];
                    index_pair++;
                }
            }
        }

    }

    return 0;

}

