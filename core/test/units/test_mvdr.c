#include "test_mvdr.h"

int test_mvdr(void) {

    const float eps = 0.001f;

    {

        const unsigned int num_sources = 1;
        const unsigned int num_channels = 4;
        const unsigned int num_bins = 2;

        const cplx_t cov_xcorr0[2] = { { .real =  -8.0f, .imag =  +4.0f },
                                       { .real =  -4.0f, .imag =  +8.0f } };

        const cplx_t cov_xcorr1[2] = { { .real =  +6.0f, .imag =  -8.0f },
                                       { .real =  +4.0f, .imag = +12.0f } };

        const cplx_t cov_xcorr2[2] = { { .real = -11.0f, .imag =  -7.0f },
                                       { .real =  -8.0f, .imag =  -4.0f } };

        const cplx_t cov_xcorr3[2] = { { .real =  -8.0f, .imag =  +4.0f },
                                       { .real = +10.0f, .imag = -10.0f } };

        const cplx_t cov_xcorr4[2] = { { .real =  +6.0f, .imag = +10.0f },
                                       { .real =  +0.0f, .imag = +10.0f } };

        const cplx_t cov_xcorr5[2] = { { .real =  -1.0f, .imag = -13.0f },
                                       { .real = +10.0f, .imag =  +0.0f } };

        const float cov_acorr0[2] = { +10.0f,  +8.0f };
        const float cov_acorr1[2] = {  +8.0f, +10.0f };
        const float cov_acorr2[2] = { +10.0f, +20.0f };
        const float cov_acorr3[2] = { +17.0f, +10.0f };

        const cplx_t weights_coeffs[2][4] = { { { .real = +0.2222f, .imag = +0.0000f },
                                                { .real = -0.1778f, .imag = -0.0889f },
                                                { .real = +0.1333f, .imag = +0.1778f },
                                                { .real = -0.2444f, .imag = +0.1556f } },
                                              { { .real = +0.1667f, .imag = +0.0000f },
                                                { .real = -0.0833f, .imag = -0.1667f },
                                                { .real = +0.0833f, .imag = -0.2500f },
                                                { .real = -0.1667f, .imag = +0.0833f } } };

        covs_t * covs = covs_construct("XXs", num_channels, num_bins);
        mvdr_t * mvdr = mvdr_construct(num_channels, num_bins);
        weights_t * weights = weights_construct("ws", num_sources, num_channels, num_bins);

        memcpy(covs->xcorrs[0], cov_xcorr0, sizeof(cov_xcorr0));
        memcpy(covs->xcorrs[1], cov_xcorr1, sizeof(cov_xcorr1));
        memcpy(covs->xcorrs[2], cov_xcorr2, sizeof(cov_xcorr2));
        memcpy(covs->xcorrs[3], cov_xcorr3, sizeof(cov_xcorr3));
        memcpy(covs->xcorrs[4], cov_xcorr4, sizeof(cov_xcorr4));
        memcpy(covs->xcorrs[5], cov_xcorr5, sizeof(cov_xcorr5));

        memcpy(covs->acorrs[0], cov_acorr0, sizeof(cov_acorr0));
        memcpy(covs->acorrs[1], cov_acorr1, sizeof(cov_acorr1));
        memcpy(covs->acorrs[2], cov_acorr2, sizeof(cov_acorr2));
        memcpy(covs->acorrs[3], cov_acorr3, sizeof(cov_acorr3));

        mvdr_process(mvdr, covs, weights);

        unsigned int index_source = 0;
        for (unsigned int index_bin = 0; index_bin < num_bins; index_bin++) {
            for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
                if (!(fabsf(weights->bins[index_source][index_channel][index_bin].real - weights_coeffs[index_bin][index_channel].real) < eps)) {
                    return -1;
                }
                if (!(fabsf(weights->bins[index_source][index_channel][index_bin].imag - weights_coeffs[index_bin][index_channel].imag) < eps)) {
                    return -1;
                }                
            }
        }

        covs_destroy(covs);
        mvdr_destroy(mvdr);
        weights_destroy(weights);

    }

    return 0;

}