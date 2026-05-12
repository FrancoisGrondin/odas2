#include "test_rankone.h"

int test_rankone(void) {

    const float eps = 0.001f;

    {

        const unsigned int num_channels = 4;
        const unsigned int num_pairs = 6;
        const unsigned int num_bins = 2;
        const unsigned int num_iterations = 10;  

        float cov_in_bin0_acorr[4] = { 11.0f, 23.0f, 31.0f, 18.0f };
        cplx_t cov_in_bin0_xcorr[6] = { { .real =  -5.0f, .imag =  -2.0f },
                                        { .real =  -4.0f, .imag = -15.0f },
                                        { .real =  -4.0f, .imag =  -1.0f },
                                        { .real = +13.0f, .imag =  +7.0f },
                                        { .real = +12.0f, .imag =  +0.0f },
                                        { .real =  +8.0f, .imag =  -9.0f } };

        float cov_in_bin1_acorr[4] = { 2.0f, 2.0f, 4.0f, 9.0f };
        cplx_t cov_in_bin1_xcorr[6] = { { .real =  +0.0f, .imag =  -2.0f },
                                        { .real =  -2.0f, .imag =  +0.0f },
                                        { .real =  +0.0f, .imag =  +3.0f },
                                        { .real =  +0.0f, .imag =  -2.0f },
                                        { .real =  -3.0f, .imag =  +0.0f },
                                        { .real =  +0.0f, .imag =  -6.0f } };

        float cov_out_bin0_acorr[4] = { 6.1198f, 14.1395f, 26.6312f, 9.3034f };
        cplx_t cov_out_bin0_xcorr[6] = { { .real =  -7.6201f, .imag =  -5.3352f },
                                         { .real =  -4.3905f, .imag = -11.9875f },
                                         { .real =  -6.7794f, .imag =  -3.3128f },
                                         { .real = +15.9176f, .imag = +11.0987f },
                                         { .real = +11.3295f, .imag =  -1.7854f },
                                         { .real = +11.3529f, .imag = -10.9029f } };

        float cov_out_bin1_acorr[4] = { 1.2941f, 1.2941f, 3.9116f, 8.8010f };
        cplx_t cov_out_bin1_xcorr[6] = { { .real =  +0.0000f, .imag =  -1.2941f },
                                         { .real =  -2.2499f, .imag =  +0.0000f },
                                         { .real =  +0.0000f, .imag =  +3.3748f },
                                         { .real =  +0.0000f, .imag =  -2.2499f },
                                         { .real =  -3.3748f, .imag =  +0.0000f },
                                         { .real =  +0.0000f, .imag =  -5.8673f } };

        covs_t * covs_in = covs_construct("XXs", num_channels, num_bins);
        covs_t * covs_out = covs_construct("SSs", num_channels, num_bins);
        rankone_t * rankone = rankone_construct(num_channels, num_bins, num_iterations);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
            covs_in->acorrs[index_channel][0] = cov_in_bin0_acorr[index_channel];
            covs_in->acorrs[index_channel][1] = cov_in_bin1_acorr[index_channel];
        }

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            covs_in->xcorrs[index_pair][0] = cov_in_bin0_xcorr[index_pair];
            covs_in->xcorrs[index_pair][1] = cov_in_bin1_xcorr[index_pair];
        }

        rankone_process(rankone, covs_in, covs_out);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {

            if (!(fabsf(covs_out->acorrs[index_channel][0] - cov_out_bin0_acorr[index_channel]) < eps)) {
                return -1;
            }

            if (!(fabsf(covs_out->acorrs[index_channel][1] - cov_out_bin1_acorr[index_channel]) < eps)) {
                return -1;
            }            

        }

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {

            if (!(sqrtf(cplx_l2(cplx_sub(covs_out->xcorrs[index_pair][0], cov_out_bin0_xcorr[index_pair]))) < eps)) {
                return -1;
            }

            if (!(sqrtf(cplx_l2(cplx_sub(covs_out->xcorrs[index_pair][1], cov_out_bin1_xcorr[index_pair]))) < eps)) {
                return -1;
            }

        }

        covs_destroy(covs_in);
        covs_destroy(covs_out);
        rankone_destroy(rankone);

    }

    return 0;

}