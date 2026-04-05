#include <systems/scm.h>
#include <utils/error.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

scm_t * scm_construct(const unsigned int num_channels, const unsigned int num_bins, const float alpha) {

    if (num_channels < 2) {
        odas2_set_error_number(ODAS2_ERROR_SCM_CONSTRUCT_NUM_CHANNELS);
        return NULL;
    }
    if (num_bins < 1) {
        odas2_set_error_number(ODAS2_ERROR_SCM_CONSTRUCT_NUM_BINS);
        return NULL;
    }
    if (alpha < 0.0f || alpha > 1.0f) {
        odas2_set_error_number(ODAS2_ERROR_SCM_CONSTRUCT_ALPHA);
        return NULL;
    }

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

    if (obj->num_channels != freqs->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_SCM_PROCESS_FREQS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_channels != masks->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_SCM_PROCESS_MASKS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_channels != covs->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_SCM_PROCESS_COVS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_bins != freqs->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_SCM_PROCESS_FREQS_NUM_BINS);
        return -1;
    }
    if (obj->num_bins != masks->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_SCM_PROCESS_MASKS_NUM_BINS);
        return -1;
    }
    if (obj->num_bins != covs->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_SCM_PROCESS_COVS_NUM_BINS);
        return -1;
    }

    #pragma omp parallel for collapse(2) num_threads(get_thread_count())
    for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {

        for (unsigned int index_channel2 = (index_channel1 + 1); index_channel2 < obj->num_channels; index_channel2++) {

            //
            // k = i(N-1) - i(i-1)/2 + (j-i-1)
            //
            // Ex:    
            //         0     1     2     3        
            //      +-----+-----+-----+-----+     
            //   0  |  -  |  0  |  1  |  2  |     i=0, j=1 : k = 0 * (4-1) - 0*(0-1)/2 + (1-0-1) = 0
            //      +-----+-----+-----+-----+     i=0, j=2 : k = 0 * (4-1) - 0*(0-1)/2 + (2-0-1) = 1
            //   1  |  -  |  -  |  3  |  4  |     i=0, j=3 : k = 0 * (4-1) - 0*(0-1)/2 + (3-0-1) = 2
            //      +-----+-----+-----+-----+     i=1, j=2 : k = 1 * (4-1) - 1*(1-1)/2 + (2-1-1) = 3
            //   2  |  -  |  -  |  -  |  5  |     i=1, j=3 : k = 1 * (4-1) - 1*(1-1)/2 + (3-1-1) = 4
            //      +-----+-----+-----+-----+     i=2, j=3 : k = 2 * (4-1) - 2*(2-1)/2 + (3-2-1) = 5
            //   3  |  -  |  -  |  -  |  -  |
            //      +-----+-----+-----+-----+
            //
            unsigned int index_pair = index_channel1 * (obj->num_channels-1) - index_channel1 * (index_channel1-1) / 2 + (index_channel2 - index_channel1 - 1);

            #pragma omp simd
            for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

                cplx_t xcorr = obj->xcorrs[index_pair][index_bin];
                cplx_t xspec = cplx_mul(freqs->bins[index_channel1][index_bin], cplx_conj(freqs->bins[index_channel2][index_bin]));

                float gain = masks->gains[index_channel1][index_bin] * masks->gains[index_channel2][index_bin];
                float alpha = obj->alpha * gain;

                xcorr = cplx_add(cplx_mul(cplx_cst(1.0f - alpha, 0.0f), xcorr), cplx_mul(cplx_cst(alpha, 0.0f), xspec));
                obj->xcorrs[index_pair][index_bin] = xcorr;

            }

            memcpy(covs->xcorrs[index_pair], obj->xcorrs[index_pair], sizeof(cplx_t) * obj->num_bins);

        }

    }

    #pragma omp parallel for num_threads(get_thread_count())
    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

        #pragma omp simd
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

