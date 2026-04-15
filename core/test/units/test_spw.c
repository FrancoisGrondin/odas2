#include "test_spw.h"

int test_spw(void) {

    const float eps = 0.001f;

    {

        const unsigned int num_channels = 4;
        const unsigned int num_pairs = 6;
        const unsigned int num_bins = 2;
        const float epsilon = 0.001f;
        const float gamma = 0.1f;

        float cov_target_bin0_acorr[4] = { +5.0f, +5.0f, +1.0f, +10.0f };
        cplx_t cov_target_bin0_xcorr[6] = { { .real = +0.0f, .imag = -5.0f },
                                            { .real = +1.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = +7.0f },
                                            { .real = -2.0f, .imag = +1.0f },
                                            { .real = -7.0f, .imag = +1.0f },
                                            { .real = +3.0f, .imag = +1.0f } };

        float cov_target_bin1_acorr[4] = { +2.0f, +5.0f, +10.0f, +4.0f };
        cplx_t cov_target_bin1_xcorr[6] = { { .real = +1.0f, .imag = +3.0f },
                                            { .real = -4.0f, .imag = +2.0f },
                                            { .real = -2.0f, .imag = -2.0f },
                                            { .real = +1.0f, .imag = +7.0f },
                                            { .real = -4.0f, .imag = +2.0f },
                                            { .real = +2.0f, .imag = +6.0f } };

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

        float cov_whitened_bin0_acorr[4] = { +19.2463f, +25.0015f, -1.8100f, +30.8190f };
        cplx_t cov_whitened_bin0_xcorr[6] = { { .real = -6.2348f, .imag = -22.1239f },
                                              { .real = +5.1442f, .imag = -0.0230f },
                                              { .real = +6.8228f, .imag = +23.7795f },
                                              { .real = -2.3750f, .imag = +3.2258f },
                                              { .real = -29.0551f, .imag = -1.3304f },
                                              { .real = +0.3252f, .imag = +5.3840f } };

        float cov_whitened_bin1_acorr[4] = { +3.0783f, +11.4677f, +22.6659f, +8.5814f };
        cplx_t cov_whitened_bin1_xcorr[6] = { { .real = +0.4615f, .imag = +6.2340f },
                                              { .real = -8.4467f, .imag = +2.1354f },
                                              { .real = -4.1560f, .imag = -3.2131f },
                                              { .real = +2.7515f, .imag = +15.8931f },
                                              { .real = -7.4364f, .imag = +7.3216f },
                                              { .real = +8.3319f, .imag = +11.9292f } };

        covs_t * target = covs_construct("SSs", num_channels, num_bins);
        covs_t * interf = covs_construct("NNs", num_channels, num_bins);
        covs_t * whitened = covs_construct("WWs", num_channels, num_bins);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
            target->acorrs[index_channel][0] = cov_target_bin0_acorr[index_channel];
            target->acorrs[index_channel][1] = cov_target_bin1_acorr[index_channel];
            interf->acorrs[index_channel][0] = cov_interf_bin0_acorr[index_channel];
            interf->acorrs[index_channel][1] = cov_interf_bin1_acorr[index_channel];
        }
        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            target->xcorrs[index_pair][0] = cov_target_bin0_xcorr[index_pair];
            target->xcorrs[index_pair][1] = cov_target_bin1_xcorr[index_pair];
            interf->xcorrs[index_pair][0] = cov_interf_bin0_xcorr[index_pair];
            interf->xcorrs[index_pair][1] = cov_interf_bin1_xcorr[index_pair];
        }

        spw_t * spw = spw_construct(num_channels, num_bins, epsilon, gamma);

        spw_process(spw, target, interf, whitened);

        for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {

            if (!(fabsf(whitened->acorrs[index_channel][0] - cov_whitened_bin0_acorr[index_channel]) < eps)) {
                return -1;
            }
            if (!(fabsf(whitened->acorrs[index_channel][1] - cov_whitened_bin1_acorr[index_channel]) < eps)) {
                return -2;
            }

        }

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {

            if (!(sqrtf(cplx_l2(cplx_sub(whitened->xcorrs[index_pair][0], cov_whitened_bin0_xcorr[index_pair]))) < eps)) {
                return -3;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(whitened->xcorrs[index_pair][1], cov_whitened_bin1_xcorr[index_pair]))) < eps)) {
                return -4;
            }

        }

        spw_destroy(spw);

        covs_destroy(target);
        covs_destroy(interf);
        covs_destroy(whitened);

    }

    return 0;

}