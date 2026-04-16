#include "test_spinv.h"

int test_spinv(void) {

    const float eps = 0.001f;

    {

        const unsigned int num_channels = 4;
        const unsigned int num_pairs = 6;
        const unsigned int num_bins = 2;
        const float epsilon = 0.001f;
        const float gamma = 0.1f;

        float cov_interf_bin0_acorr[4] = { +1.0f, +1.0f, +5.0f, +2.0f };
        cplx_t cov_interf_bin0_xcorr[6] = { { .real = +0.0f, .imag = +1.0f },
                                            { .real = -2.0f, .imag = +1.0f },
                                            { .real = -1.0f, .imag = +1.0f },
                                            { .real = +1.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = +3.0f, .imag = -1.0f } };

        float cov_interf_bin1_acorr[4] = { +4.0f, +2.0f, +2.0f, +9.0f };
        cplx_t cov_interf_bin1_xcorr[6] = { { .real = +2.0f, .imag = -2.0f },
                                            { .real = +2.0f, .imag = +2.0f },
                                            { .real = +0.0f, .imag = -6.0f },
                                            { .real = +0.0f, .imag = +2.0f },
                                            { .real = +3.0f, .imag = -3.0f },
                                            { .real = -3.0f, .imag = -3.0f } };

        float cov_invinterf_bin0_acorr[4] = { +3.9452f, +3.9452f, +2.0268f, +3.4656f };
        cplx_t cov_invinterf_bin0_xcorr[6] = { { .real = +0.0000f, .imag = -0.4796f },
                                               { .real = +0.9592f, .imag = -0.4796f },
                                               { .real = +0.4796f, .imag = -0.4796f },
                                               { .real = -0.4796f, .imag = -0.9592f },
                                               { .real = -0.4796f, .imag = -0.4796f },
                                               { .real = -1.4388f, .imag = +0.4796f } };

        float cov_invinterf_bin1_acorr[4] = { +1.8086f, +2.0780f, +2.0780f, +1.1350f };
        cplx_t cov_invinterf_bin1_xcorr[6] = { { .real = -0.2694f, .imag = +0.2694f },
                                               { .real = -0.2694f, .imag = -0.2694f },
                                               { .real = +0.0000f, .imag = +0.8082f },
                                               { .real = +0.0000f, .imag = -0.2694f },
                                               { .real = -0.4041f, .imag = +0.4041f },
                                               { .real = +0.4041f, .imag = +0.4041f } };

        covs_t * interf = covs_construct("NNs", num_channels, num_bins);
        covs_t * invinterf = covs_construct("NNsInv", num_channels, num_bins);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
            interf->acorrs[index_channel][0] = cov_interf_bin0_acorr[index_channel];
            interf->acorrs[index_channel][1] = cov_interf_bin1_acorr[index_channel];
        }
        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            interf->xcorrs[index_pair][0] = cov_interf_bin0_xcorr[index_pair];
            interf->xcorrs[index_pair][1] = cov_interf_bin1_xcorr[index_pair];
        }

        spinv_t * spinv = spinv_construct(num_channels, num_bins, epsilon, gamma);

        spinv_process(spinv, interf, invinterf);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {

            if (!(fabsf(invinterf->acorrs[index_channel][0] - cov_invinterf_bin0_acorr[index_channel]) < eps)) {
                return -1;
            }
            if (!(fabsf(invinterf->acorrs[index_channel][1] - cov_invinterf_bin1_acorr[index_channel]) < eps)) {
                return -2;
            }

        }

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {

            if (!(sqrtf(cplx_l2(cplx_sub(invinterf->xcorrs[index_pair][0], cov_invinterf_bin0_xcorr[index_pair]))) < eps)) {
                return -3;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(invinterf->xcorrs[index_pair][1], cov_invinterf_bin1_xcorr[index_pair]))) < eps)) {
                return -4;
            }

        }

        spinv_destroy(spinv);

        covs_destroy(interf);
        covs_destroy(invinterf);

    }

    return 0;

}