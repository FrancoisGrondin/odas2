#include "test_phat.h"

int test_phat(void) {

    const float eps = 0.001f;

    {

        const unsigned int num_channels = 3;
        const unsigned int num_bins = 4;

        cplx_t cov_xcorr0[4] = { { .real = +2.0f, .imag = +1.0f },
                                 { .real = -3.0f, .imag = +2.0f },
                                 { .real = +1.0f, .imag = -2.0f },
                                 { .real = +2.0f, .imag = -1.0f } };

        cplx_t cov_xcorr1[4] = { { .real = +1.0f, .imag = -3.0f },
                                 { .real = -2.0f, .imag = +1.0f },
                                 { .real = +4.0f, .imag = +2.0f },
                                 { .real = -1.0f, .imag = +1.0f } };

        cplx_t cov_xcorr2[4] = { { .real = -2.0f, .imag = +2.0f },
                                 { .real = +0.0f, .imag = +0.0f },
                                 { .real = -1.0f, .imag = +2.0f },
                                 { .real = -3.0f, .imag = +1.0f } };

        float cov_acorr0[4] = { +2.0f, +1.0f, +3.0f, +2.0f };
        float cov_acorr1[4] = { +1.0f, +2.0f, +1.0f, +3.0f };
        float cov_acorr2[4] = { +2.0f, +2.0f, +2.0f, +4.0f };

        cplx_t rtn_cov_xcorr0[4] = { { .real = +0.8944, .imag = +0.4472 },
                                     { .real = -0.8321, .imag = +0.5547 },
                                     { .real = +0.4472, .imag = -0.8944 },
                                     { .real = +0.8944, .imag = -0.4472 } };

        cplx_t rtn_cov_xcorr1[4] = { { .real = +0.3162, .imag = -0.9487 },
                                     { .real = -0.8944, .imag = +0.4472 },
                                     { .real = +0.8944, .imag = +0.4472 },
                                     { .real = -0.7071, .imag = +0.7071 } };

        cplx_t rtn_cov_xcorr2[4] = { { .real = -0.7071, .imag = +0.7071 },
                                     { .real = +0.0000, .imag = +0.0000 },
                                     { .real = -0.4472, .imag = +0.8944 },
                                     { .real = -0.9487, .imag = +0.3162 } };

        float rtn_cov_acorr0[4] = { +1.0f, +1.0f, +1.0f, +1.0f };
        float rtn_cov_acorr1[4] = { +1.0f, +1.0f, +1.0f, +1.0f };
        float rtn_cov_acorr2[4] = { +1.0f, +1.0f, +1.0f, +1.0f };

        covs_t * covs_in = covs_construct("XXs", num_channels, num_bins);
        covs_t * covs_out = covs_construct("XXps", num_channels, num_bins);
        phat_t * phat = phat_construct(num_channels, num_bins);

        memcpy(covs_in->xcorrs[0], cov_xcorr0, sizeof(cov_xcorr0));
        memcpy(covs_in->xcorrs[1], cov_xcorr1, sizeof(cov_xcorr1));
        memcpy(covs_in->xcorrs[2], cov_xcorr2, sizeof(cov_xcorr2));

        memcpy(covs_in->acorrs[0], cov_acorr0, sizeof(cov_acorr0));
        memcpy(covs_in->acorrs[1], cov_acorr1, sizeof(cov_acorr1));
        memcpy(covs_in->acorrs[2], cov_acorr2, sizeof(cov_acorr2));

        phat_process(phat, covs_in, covs_out);

        for (unsigned int index_bin = 0; index_bin < num_bins; index_bin++) {
            if (!(sqrtf(cplx_l2(cplx_sub(covs_out->xcorrs[0][index_bin], rtn_cov_xcorr0[index_bin]))) < eps)) {
                return -1;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(covs_out->xcorrs[1][index_bin], rtn_cov_xcorr1[index_bin]))) < eps)) {
                return -1;
            }            
            if (!(sqrtf(cplx_l2(cplx_sub(covs_out->xcorrs[2][index_bin], rtn_cov_xcorr2[index_bin]))) < eps)) {
                return -1;
            }
            if (!(fabsf(covs_out->acorrs[0][index_bin] - rtn_cov_acorr0[index_bin]) < eps)) {
                return -1;
            }
            if (!(fabsf(covs_out->acorrs[1][index_bin] - rtn_cov_acorr1[index_bin]) < eps)) {
                return -1;
            }
            if (!(fabsf(covs_out->acorrs[2][index_bin] - rtn_cov_acorr2[index_bin]) < eps)) {
                return -1;
            }
        }

        covs_destroy(covs_in);
        covs_destroy(covs_out);

    }

    return 0;

}