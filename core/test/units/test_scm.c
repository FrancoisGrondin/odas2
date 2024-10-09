#include "test_scm.h"

int test_scm(void) {

    const float eps = 0.001f;

    {

        const unsigned int num_channels = 3;
        const unsigned int num_pairs = 3;
        const unsigned int num_bins = 4;
        const float alpha = 0.1f;

        const cplx_t freq_ch1_frame1[4] = { { .real = +1.0f, .imag = +2.0f },
                                            { .real = -1.0f, .imag = +0.0f },
                                            { .real = +3.0f, .imag = -2.0f },
                                            { .real = +0.0f, .imag = +1.0f } };

        const cplx_t freq_ch1_frame2[4] = { { .real = +2.0f, .imag = +1.0f },
                                            { .real = +0.0f, .imag = +1.0f },
                                            { .real = +1.0f, .imag = -1.0f },
                                            { .real = -2.0f, .imag = +3.0f } };                                        

        const cplx_t freq_ch2_frame1[4] = { { .real = +0.0f, .imag = -2.0f },
                                            { .real = +2.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = -2.0f, .imag = -1.0f } };

        const cplx_t freq_ch2_frame2[4] = { { .real = +1.0f, .imag = +1.0f },
                                            { .real = -2.0f, .imag = +1.0f },
                                            { .real = +4.0f, .imag = -3.0f },
                                            { .real = -1.0f, .imag = +2.0f } };

        const cplx_t freq_ch3_frame1[4] = { { .real = +2.0f, .imag = +0.0f },
                                            { .real = +3.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = -1.0f, .imag = -2.0f } };

        const cplx_t freq_ch3_frame2[4] = { { .real = +2.0f, .imag = +2.0f },
                                            { .real = +2.0f, .imag = -3.0f },
                                            { .real = -1.0f, .imag = +1.0f },
                                            { .real = +2.0f, .imag = -1.0f } };

        float cov_bin0_frame1_acorr[3] = { 0.5f, 0.4f, 0.4f };
        cplx_t cov_bin0_frame1_xcorr[3] = { { .real = -0.4f, .imag = +0.2f },
                                            { .real = +0.2f, .imag = +0.4f },
                                            { .real = +0.0f, .imag = -0.4f } };

        float cov_bin1_frame1_acorr[3] = { 0.1f, 0.8f, 1.3f };
        cplx_t cov_bin1_frame1_xcorr[3] = { { .real = -0.2f, .imag = +0.2f },
                                            { .real = -0.3f, .imag = +0.2f },
                                            { .real = +1.0f, .imag = +0.2f } };                                            

        float cov_bin2_frame1_acorr[3] = { 1.3f, 0.2f, 0.2f };
        cplx_t cov_bin2_frame1_xcorr[3] = { { .real = +0.1f, .imag = -0.5f },
                                            { .real = +0.1f, .imag = -0.5f },
                                            { .real = +0.2f, .imag = +0.0f } };   

        float cov_bin3_frame1_acorr[3] = { 0.1f, 0.5f, 0.5f };
        cplx_t cov_bin3_frame1_xcorr[3] = { { .real = -0.1f, .imag = -0.2f },
                                            { .real = -0.2f, .imag = -0.1f },
                                            { .real = +0.4f, .imag = -0.3f } };

        float cov_bin0_frame2_acorr[3] = { 0.95f, 0.56f, 1.16f };
        cplx_t cov_bin0_frame2_xcorr[3] = { { .real = -0.06f, .imag = +0.08f },
                                            { .real = +0.78f, .imag = +0.16f },
                                            { .real = +0.40f, .imag = -0.36f } };

        float cov_bin1_frame2_acorr[3] = { 0.19f, 1.22f, 2.47f };
        cplx_t cov_bin1_frame2_xcorr[3] = { { .real = -0.08f, .imag = -0.02f },
                                            { .real = -0.57f, .imag = +0.38f },
                                            { .real = +0.20f, .imag = -0.22f } };

        float cov_bin2_frame2_acorr[3] = { 1.37f, 2.68f, 0.38f };
        cplx_t cov_bin2_frame2_xcorr[3] = { { .real = +0.79f, .imag = -0.55f },
                                            { .real = -0.11f, .imag = -0.45f },
                                            { .real = -0.52f, .imag = -0.10f } };

        float cov_bin3_frame2_acorr[3] = { 1.39f, 0.95f, 0.95f };
        cplx_t cov_bin3_frame2_xcorr[3] = { { .real = +0.71f, .imag = -0.08f },
                                            { .real = -0.88f, .imag = +0.31f },
                                            { .real = -0.04f, .imag = +0.03f } };

        freqs_t * freqs = freqs_construct("Xs", num_channels, num_bins);
        masks_t * masks = masks_construct("Ms", num_channels, num_bins);
        covs_t * covs = covs_construct("XXs", num_channels, num_bins);
        scm_t * scm = scm_construct(num_channels, num_bins, alpha);

        masks_ones(masks);

        memcpy(freqs->bins[0], freq_ch1_frame1, sizeof(freq_ch1_frame1));
        memcpy(freqs->bins[1], freq_ch2_frame1, sizeof(freq_ch2_frame1));
        memcpy(freqs->bins[2], freq_ch3_frame1, sizeof(freq_ch3_frame1));

        scm_process(scm, freqs, masks, covs);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
            if (!(fabsf(covs->acorrs[index_channel][0] - cov_bin0_frame1_acorr[index_channel]) < eps)) {
                return -1;
            }
            if (!(fabsf(covs->acorrs[index_channel][1] - cov_bin1_frame1_acorr[index_channel]) < eps)) {
                return -1;
            }
            if (!(fabsf(covs->acorrs[index_channel][2] - cov_bin2_frame1_acorr[index_channel]) < eps)) {
                return -1;
            }
            if (!(fabsf(covs->acorrs[index_channel][3] - cov_bin3_frame1_acorr[index_channel]) < eps)) {
                return -1;
            }               
        }

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][0], cov_bin0_frame1_xcorr[index_pair]))) < eps)) {
                return -1;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][1], cov_bin1_frame1_xcorr[index_pair]))) < eps)) {
                return -1;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][2], cov_bin2_frame1_xcorr[index_pair]))) < eps)) {
                return -1;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][3], cov_bin3_frame1_xcorr[index_pair]))) < eps)) {
                return -1;
            }               
        }

        memcpy(freqs->bins[0], freq_ch1_frame2, sizeof(freq_ch1_frame2));
        memcpy(freqs->bins[1], freq_ch2_frame2, sizeof(freq_ch2_frame2));
        memcpy(freqs->bins[2], freq_ch3_frame2, sizeof(freq_ch3_frame2));        

        scm_process(scm, freqs, masks, covs);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
            if (!(fabsf(covs->acorrs[index_channel][0] - cov_bin0_frame2_acorr[index_channel]) < eps)) {
                return -1;
            }
            if (!(fabsf(covs->acorrs[index_channel][1] - cov_bin1_frame2_acorr[index_channel]) < eps)) {
                return -1;
            }
            if (!(fabsf(covs->acorrs[index_channel][2] - cov_bin2_frame2_acorr[index_channel]) < eps)) {
                return -1;
            }
            if (!(fabsf(covs->acorrs[index_channel][3] - cov_bin3_frame2_acorr[index_channel]) < eps)) {
                return -1;
            }               
        }

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][0], cov_bin0_frame2_xcorr[index_pair]))) < eps)) {
                return -1;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][1], cov_bin1_frame2_xcorr[index_pair]))) < eps)) {
                return -1;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][2], cov_bin2_frame2_xcorr[index_pair]))) < eps)) {
                return -1;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][3], cov_bin3_frame2_xcorr[index_pair]))) < eps)) {
                return -1;
            }               
        }

        freqs_destroy(freqs);
        masks_destroy(masks);
        covs_destroy(covs);
        scm_destroy(scm);

    }

    {

        const unsigned int num_channels = 3;
        const unsigned int num_pairs = 3;
        const unsigned int num_bins = 4;
        const float alpha = 0.1f;

        const cplx_t freq_ch1_frame1[4] = { { .real = +1.0f, .imag = +2.0f },
                                            { .real = -1.0f, .imag = +0.0f },
                                            { .real = +3.0f, .imag = -2.0f },
                                            { .real = +0.0f, .imag = +1.0f } };

        const cplx_t freq_ch1_frame2[4] = { { .real = +2.0f, .imag = +1.0f },
                                            { .real = +0.0f, .imag = +1.0f },
                                            { .real = +1.0f, .imag = -1.0f },
                                            { .real = -2.0f, .imag = +3.0f } };                                        

        const cplx_t freq_ch2_frame1[4] = { { .real = +0.0f, .imag = -2.0f },
                                            { .real = +2.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = -2.0f, .imag = -1.0f } };

        const cplx_t freq_ch2_frame2[4] = { { .real = +1.0f, .imag = +1.0f },
                                            { .real = -2.0f, .imag = +1.0f },
                                            { .real = +4.0f, .imag = -3.0f },
                                            { .real = -1.0f, .imag = +2.0f } };

        const cplx_t freq_ch3_frame1[4] = { { .real = +2.0f, .imag = +0.0f },
                                            { .real = +3.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = -1.0f, .imag = -2.0f } };

        const cplx_t freq_ch3_frame2[4] = { { .real = +2.0f, .imag = +2.0f },
                                            { .real = +2.0f, .imag = -3.0f },
                                            { .real = -1.0f, .imag = +1.0f },
                                            { .real = +2.0f, .imag = -1.0f } };

        float cov_bin0_frame1_acorr[3] = { 0.0f, 0.0f, 0.0f };
        cplx_t cov_bin0_frame1_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f } };

        float cov_bin1_frame1_acorr[3] = { 0.0f, 0.0f, 0.0f };
        cplx_t cov_bin1_frame1_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f } };

        float cov_bin2_frame1_acorr[3] = { 0.0f, 0.0f, 0.0f };
        cplx_t cov_bin2_frame1_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f } };

        float cov_bin3_frame1_acorr[3] = { 0.0f, 0.0f, 0.0f };
        cplx_t cov_bin3_frame1_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f } };

        float cov_bin0_frame2_acorr[3] = { 0.0f, 0.0f, 0.0f };
        cplx_t cov_bin0_frame2_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f } };

        float cov_bin1_frame2_acorr[3] = { 0.0f, 0.0f, 0.0f };
        cplx_t cov_bin1_frame2_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f } };

        float cov_bin2_frame2_acorr[3] = { 0.0f, 0.0f, 0.0f };
        cplx_t cov_bin2_frame2_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f } };

        float cov_bin3_frame2_acorr[3] = { 0.0f, 0.0f, 0.0f };
        cplx_t cov_bin3_frame2_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f } };

        freqs_t * freqs = freqs_construct("Xs", num_channels, num_bins);
        masks_t * masks = masks_construct("Ms", num_channels, num_bins);
        covs_t * covs = covs_construct("XXs", num_channels, num_bins);
        scm_t * scm = scm_construct(num_channels, num_bins, alpha);

        masks_zeros(masks);

        memcpy(freqs->bins[0], freq_ch1_frame1, sizeof(freq_ch1_frame1));
        memcpy(freqs->bins[1], freq_ch2_frame1, sizeof(freq_ch2_frame1));
        memcpy(freqs->bins[2], freq_ch3_frame1, sizeof(freq_ch3_frame1));

        scm_process(scm, freqs, masks, covs);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
            if (!(fabsf(covs->acorrs[index_channel][0] - cov_bin0_frame1_acorr[index_channel]) < eps)) {
                return -2;
            }
            if (!(fabsf(covs->acorrs[index_channel][1] - cov_bin1_frame1_acorr[index_channel]) < eps)) {
                return -2;
            }
            if (!(fabsf(covs->acorrs[index_channel][2] - cov_bin2_frame1_acorr[index_channel]) < eps)) {
                return -2;
            }
            if (!(fabsf(covs->acorrs[index_channel][3] - cov_bin3_frame1_acorr[index_channel]) < eps)) {
                return -2;
            }               
        }

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][0], cov_bin0_frame1_xcorr[index_pair]))) < eps)) {
                return -2;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][1], cov_bin1_frame1_xcorr[index_pair]))) < eps)) {
                return -2;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][2], cov_bin2_frame1_xcorr[index_pair]))) < eps)) {
                return -2;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][3], cov_bin3_frame1_xcorr[index_pair]))) < eps)) {
                return -2;
            }               
        }

        memcpy(freqs->bins[0], freq_ch1_frame2, sizeof(freq_ch1_frame2));
        memcpy(freqs->bins[1], freq_ch2_frame2, sizeof(freq_ch2_frame2));
        memcpy(freqs->bins[2], freq_ch3_frame2, sizeof(freq_ch3_frame2));        

        scm_process(scm, freqs, masks, covs);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
            if (!(fabsf(covs->acorrs[index_channel][0] - cov_bin0_frame2_acorr[index_channel]) < eps)) {
                return -2;
            }
            if (!(fabsf(covs->acorrs[index_channel][1] - cov_bin1_frame2_acorr[index_channel]) < eps)) {
                return -2;
            }
            if (!(fabsf(covs->acorrs[index_channel][2] - cov_bin2_frame2_acorr[index_channel]) < eps)) {
                return -2;
            }
            if (!(fabsf(covs->acorrs[index_channel][3] - cov_bin3_frame2_acorr[index_channel]) < eps)) {
                return -2;
            }               
        }

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][0], cov_bin0_frame2_xcorr[index_pair]))) < eps)) {
                return -2;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][1], cov_bin1_frame2_xcorr[index_pair]))) < eps)) {
                return -2;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][2], cov_bin2_frame2_xcorr[index_pair]))) < eps)) {
                return -2;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][3], cov_bin3_frame2_xcorr[index_pair]))) < eps)) {
                return -2;
            }               
        }

        freqs_destroy(freqs);
        masks_destroy(masks);
        covs_destroy(covs);
        scm_destroy(scm);

    }    

    {

        const unsigned int num_channels = 3;
        const unsigned int num_pairs = 3;
        const unsigned int num_bins = 4;
        const float alpha = 0.1f;

        const cplx_t freq_ch1_frame1[4] = { { .real = +1.0f, .imag = +2.0f },
                                            { .real = -1.0f, .imag = +0.0f },
                                            { .real = +3.0f, .imag = -2.0f },
                                            { .real = +0.0f, .imag = +1.0f } };

        const cplx_t freq_ch1_frame2[4] = { { .real = +2.0f, .imag = +1.0f },
                                            { .real = +0.0f, .imag = +1.0f },
                                            { .real = +1.0f, .imag = -1.0f },
                                            { .real = -2.0f, .imag = +3.0f } };                                        

        const cplx_t freq_ch2_frame1[4] = { { .real = +0.0f, .imag = -2.0f },
                                            { .real = +2.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = -2.0f, .imag = -1.0f } };

        const cplx_t freq_ch2_frame2[4] = { { .real = +1.0f, .imag = +1.0f },
                                            { .real = -2.0f, .imag = +1.0f },
                                            { .real = +4.0f, .imag = -3.0f },
                                            { .real = -1.0f, .imag = +2.0f } };

        const cplx_t freq_ch3_frame1[4] = { { .real = +2.0f, .imag = +0.0f },
                                            { .real = +3.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = -1.0f, .imag = -2.0f } };

        const cplx_t freq_ch3_frame2[4] = { { .real = +2.0f, .imag = +2.0f },
                                            { .real = +2.0f, .imag = -3.0f },
                                            { .real = -1.0f, .imag = +1.0f },
                                            { .real = +2.0f, .imag = -1.0f } };

        const float mask_ch1_frame1[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
        const float mask_ch1_frame2[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
        const float mask_ch2_frame1[4] = { 1.0f, 0.0f, 1.0f, 0.0f };
        const float mask_ch2_frame2[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
        const float mask_ch3_frame1[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
        const float mask_ch3_frame2[4] = { 0.0f, 1.0f, 1.0f, 1.0f };

        float cov_bin0_frame1_acorr[3] = { 0.5f, 0.4f, 0.0f };
        cplx_t cov_bin0_frame1_xcorr[3] = { { .real = -0.4f, .imag = +0.2f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f } };

        float cov_bin1_frame1_acorr[3] = { 0.0f, 0.0f, 1.3f };
        cplx_t cov_bin1_frame1_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f } };

        float cov_bin2_frame1_acorr[3] = { 0.0f, 0.2f, 0.2f };
        cplx_t cov_bin2_frame1_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.2f, .imag = +0.0f } };

        float cov_bin3_frame1_acorr[3] = { 0.1f, 0.0f, 0.5f };
        cplx_t cov_bin3_frame1_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = -0.2f, .imag = -0.1f },
                                            { .real = +0.0f, .imag = +0.0f } };

        float cov_bin0_frame2_acorr[3] = { 0.5f, 0.56f, 0.0f };
        cplx_t cov_bin0_frame2_xcorr[3] = { { .real = -0.4f, .imag = +0.2f },
                                            { .real = +0.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +0.0f } };

        float cov_bin1_frame2_acorr[3] = { 0.1f, 0.5f, 2.47f };
        cplx_t cov_bin1_frame2_xcorr[3] = { { .real = +0.1f, .imag = -0.2f },
                                            { .real = -0.3f, .imag = +0.2f },
                                            { .real = -0.7f, .imag = -0.4f } };

        float cov_bin2_frame2_acorr[3] = { 0.2f, 0.2f, 0.38f };
        cplx_t cov_bin2_frame2_xcorr[3] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = -0.2f, .imag = +0.0f },
                                            { .real = +0.2f, .imag = +0.0f } };

        float cov_bin3_frame2_acorr[3] = { 1.39f, 0.5f, 0.95f };
        cplx_t cov_bin3_frame2_xcorr[3] = { { .real = +0.80f, .imag = +0.10f },
                                            { .real = -0.88f, .imag = +0.31f },
                                            { .real = -0.40f, .imag = +0.30f } };

        freqs_t * freqs = freqs_construct("Xs", num_channels, num_bins);
        masks_t * masks = masks_construct("Ms", num_channels, num_bins);
        covs_t * covs = covs_construct("XXs", num_channels, num_bins);
        scm_t * scm = scm_construct(num_channels, num_bins, alpha);

        memcpy(freqs->bins[0], freq_ch1_frame1, sizeof(freq_ch1_frame1));
        memcpy(freqs->bins[1], freq_ch2_frame1, sizeof(freq_ch2_frame1));
        memcpy(freqs->bins[2], freq_ch3_frame1, sizeof(freq_ch3_frame1));

        memcpy(masks->gains[0], mask_ch1_frame1, sizeof(mask_ch1_frame1));
        memcpy(masks->gains[1], mask_ch2_frame1, sizeof(mask_ch2_frame1));
        memcpy(masks->gains[2], mask_ch3_frame1, sizeof(mask_ch3_frame1));

        scm_process(scm, freqs, masks, covs);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
            if (!(fabsf(covs->acorrs[index_channel][0] - cov_bin0_frame1_acorr[index_channel]) < eps)) {
                return -3;
            }
            if (!(fabsf(covs->acorrs[index_channel][1] - cov_bin1_frame1_acorr[index_channel]) < eps)) {
                return -3;
            }
            if (!(fabsf(covs->acorrs[index_channel][2] - cov_bin2_frame1_acorr[index_channel]) < eps)) {
                return -3;
            }
            if (!(fabsf(covs->acorrs[index_channel][3] - cov_bin3_frame1_acorr[index_channel]) < eps)) {
                return -3;
            }               
        }

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][0], cov_bin0_frame1_xcorr[index_pair]))) < eps)) {
                return -3;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][1], cov_bin1_frame1_xcorr[index_pair]))) < eps)) {
                return -3;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][2], cov_bin2_frame1_xcorr[index_pair]))) < eps)) {
                return -3;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][3], cov_bin3_frame1_xcorr[index_pair]))) < eps)) {
                return -3;
            }               
        }

        memcpy(freqs->bins[0], freq_ch1_frame2, sizeof(freq_ch1_frame2));
        memcpy(freqs->bins[1], freq_ch2_frame2, sizeof(freq_ch2_frame2));
        memcpy(freqs->bins[2], freq_ch3_frame2, sizeof(freq_ch3_frame2));        

        memcpy(masks->gains[0], mask_ch1_frame2, sizeof(mask_ch1_frame2));
        memcpy(masks->gains[1], mask_ch2_frame2, sizeof(mask_ch2_frame2));
        memcpy(masks->gains[2], mask_ch3_frame2, sizeof(mask_ch3_frame2));

        scm_process(scm, freqs, masks, covs);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
            if (!(fabsf(covs->acorrs[index_channel][0] - cov_bin0_frame2_acorr[index_channel]) < eps)) {
                return -3;
            }
            if (!(fabsf(covs->acorrs[index_channel][1] - cov_bin1_frame2_acorr[index_channel]) < eps)) {
                return -3;
            }
            if (!(fabsf(covs->acorrs[index_channel][2] - cov_bin2_frame2_acorr[index_channel]) < eps)) {
                return -3;
            }
            if (!(fabsf(covs->acorrs[index_channel][3] - cov_bin3_frame2_acorr[index_channel]) < eps)) {
                return -3;
            }               
        }

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][0], cov_bin0_frame2_xcorr[index_pair]))) < eps)) {
                return -3;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][1], cov_bin1_frame2_xcorr[index_pair]))) < eps)) {
                return -3;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][2], cov_bin2_frame2_xcorr[index_pair]))) < eps)) {
                return -3;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs->xcorrs[index_pair][3], cov_bin3_frame2_xcorr[index_pair]))) < eps)) {
                return -3;
            }               
        }

        freqs_destroy(freqs);
        masks_destroy(masks);
        covs_destroy(covs);
        scm_destroy(scm);

    }

    /*
    {

        const unsigned int num_channels = 3;
        const unsigned int num_pairs = 3;
        const unsigned int num_bins = 4;
        const float alpha = 0.1f;

        const cplx_t freq_ch1_frame1[4] = { { .real = +1.0f, .imag = +2.0f },
                                            { .real = -1.0f, .imag = +0.0f },
                                            { .real = +3.0f, .imag = -2.0f },
                                            { .real = +0.0f, .imag = +1.0f } };

        const cplx_t freq_ch1_frame2[4] = { { .real = +2.0f, .imag = +1.0f },
                                            { .real = +0.0f, .imag = +1.0f },
                                            { .real = +1.0f, .imag = -1.0f },
                                            { .real = -2.0f, .imag = +3.0f } };                                        

        const cplx_t freq_ch2_frame1[4] = { { .real = +0.0f, .imag = -2.0f },
                                            { .real = +2.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = -2.0f, .imag = -1.0f } };

        const cplx_t freq_ch2_frame2[4] = { { .real = +1.0f, .imag = +1.0f },
                                            { .real = -2.0f, .imag = +1.0f },
                                            { .real = +4.0f, .imag = -3.0f },
                                            { .real = -1.0f, .imag = +2.0f } };

        const cplx_t freq_ch3_frame1[4] = { { .real = +2.0f, .imag = +0.0f },
                                            { .real = +3.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = -1.0f, .imag = -2.0f } };

        const cplx_t freq_ch3_frame2[4] = { { .real = +2.0f, .imag = +2.0f },
                                            { .real = +2.0f, .imag = -3.0f },
                                            { .real = -1.0f, .imag = +1.0f },
                                            { .real = +2.0f, .imag = -1.0f } };        

        freqs_t * freqs = freqs_construct("Xs", num_channels, num_bins);
        masks_t * masks = masks_construct("Ms", num_channels, num_bins);
        covs_t * covs = covs_construct("XXs", num_channels, num_bins);
        iscm_t * iscm = iscm_construct(num_channels, num_bins, alpha);

        memcpy(freqs->bins[0], freq_ch1_frame1, sizeof(freq_ch1_frame1));
        memcpy(freqs->bins[1], freq_ch2_frame1, sizeof(freq_ch2_frame1));
        memcpy(freqs->bins[2], freq_ch3_frame1, sizeof(freq_ch3_frame1));

        masks_ones(masks);

        iscm_process(iscm, freqs, masks, covs);

        freqs_destroy(freqs);
        masks_destroy(masks);
        covs_destroy(covs);
        iscm_destroy(iscm);

    }
    */

    return 0;

}