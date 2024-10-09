#include "test_beamformer.h"

int test_beamformer(void) {

    const float eps = 0.001f;

    {

        const unsigned int num_channels = 4;
        const unsigned int num_sources = 2;
        const unsigned int num_bins = 5;

        const cplx_t freq_in_ch1[5] = { { .real = +1.0f, .imag = -2.0f },
                                        { .real = -2.0f, .imag = +1.0f },
                                        { .real = +3.0f, .imag = -1.0f },
                                        { .real = +1.0f, .imag = +2.0f },
                                        { .real = -2.0f, .imag = +0.0f } };

        const cplx_t freq_in_ch2[5] = { { .real = -1.0f, .imag = -1.0f },
                                        { .real = +1.0f, .imag = +0.0f },
                                        { .real = -2.0f, .imag = +2.0f },
                                        { .real = +2.0f, .imag = +1.0f },
                                        { .real = -1.0f, .imag = +1.0f } };

        const cplx_t freq_in_ch3[5] = { { .real = +0.0f, .imag = +3.0f },
                                        { .real = -1.0f, .imag = -1.0f },
                                        { .real = +1.0f, .imag = +0.0f },
                                        { .real = +0.0f, .imag = +1.0f },
                                        { .real = -1.0f, .imag = +1.0f } };

        const cplx_t freq_in_ch4[5] = { { .real = +0.0f, .imag = +0.0f },
                                        { .real = -1.0f, .imag = +3.0f },
                                        { .real = +2.0f, .imag = -2.0f },
                                        { .real = +4.0f, .imag = +1.0f },
                                        { .real = -1.0f, .imag = +3.0f } };

        const cplx_t weight_src1_ch1[5] = { { .real = +2.0f, .imag = -1.0f },
                                            { .real = -1.0f, .imag = +2.0f },
                                            { .real = +0.0f, .imag = -1.0f },
                                            { .real = +2.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = -1.0f } };

        const cplx_t weight_src1_ch2[5] = { { .real = -2.0f, .imag = +4.0f },
                                            { .real = -3.0f, .imag = -2.0f },
                                            { .real = +1.0f, .imag = +3.0f },
                                            { .real = -2.0f, .imag = -2.0f },
                                            { .real = +0.0f, .imag = +1.0f } };

        const cplx_t weight_src1_ch3[5] = { { .real = +1.0f, .imag = +1.0f },
                                            { .real = -1.0f, .imag = +1.0f },
                                            { .real = +0.0f, .imag = -2.0f },
                                            { .real = +0.0f, .imag = -1.0f },
                                            { .real = +1.0f, .imag = +1.0f } };

        const cplx_t weight_src1_ch4[5] = { { .real = +0.0f, .imag = +2.0f },
                                            { .real = +3.0f, .imag = +0.0f },
                                            { .real = -2.0f, .imag = +0.0f },
                                            { .real = -1.0f, .imag = -2.0f },
                                            { .real = -1.0f, .imag = +2.0f } };

        const cplx_t weight_src2_ch1[5] = { { .real = +0.0f, .imag = +1.0f },
                                            { .real = +2.0f, .imag = +1.0f },
                                            { .real = -2.0f, .imag = -2.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = -1.0f, .imag = +0.0f } };

        const cplx_t weight_src2_ch2[5] = { { .real = -2.0f, .imag = +2.0f },
                                            { .real = -1.0f, .imag = -4.0f },
                                            { .real = +2.0f, .imag = +4.0f },
                                            { .real = -1.0f, .imag = -4.0f },
                                            { .real = +2.0f, .imag = +0.0f } };

        const cplx_t weight_src2_ch3[5] = { { .real = +2.0f, .imag = +0.0f },
                                            { .real = -2.0f, .imag = +1.0f },
                                            { .real = +1.0f, .imag = -2.0f },
                                            { .real = +2.0f, .imag = -2.0f },
                                            { .real = -3.0f, .imag = +0.0f } };

        const cplx_t weight_src2_ch4[5] = { { .real = -2.0f, .imag = +0.0f },
                                            { .real = +1.0f, .imag = +3.0f },
                                            { .real = +1.0f, .imag = +2.0f },
                                            { .real = -2.0f, .imag = -1.0f },
                                            { .real = -3.0f, .imag = -1.0f } };

        const cplx_t freq_out_src1[5] = { { .real = +5.0f, .imag = +6.0f },
                                          { .real = -2.0f, .imag = +16.0f },
                                          { .real = +1.0f, .imag = +17.0f },
                                          { .real = -7.0f, .imag = +11.0f },
                                          { .real = +6.0f, .imag = +0.0f } };

        const cplx_t freq_out_src2[5] = { { .real = -2.0f, .imag = +9.0f },
                                          { .real = +5.0f, .imag = +17.0f },
                                          { .real = -1.0f, .imag = +16.0f },
                                          { .real = -14.0f, .imag = +12.0f },
                                          { .real = +3.0f, .imag = -11.0f } };

        beamformer_t * beamformer = beamformer_construct(num_sources, num_channels, num_bins);

        freqs_t * in = freqs_construct("Xs", num_channels, num_bins);
        freqs_t * out = freqs_construct("Ys", num_sources, num_bins);
        weights_t * coeffs = weights_construct("Ms", num_sources, num_channels, num_bins);

        memcpy(in->bins[0], freq_in_ch1, num_bins * sizeof(cplx_t));
        memcpy(in->bins[1], freq_in_ch2, num_bins * sizeof(cplx_t));
        memcpy(in->bins[2], freq_in_ch3, num_bins * sizeof(cplx_t));
        memcpy(in->bins[3], freq_in_ch4, num_bins * sizeof(cplx_t));

        memcpy(coeffs->bins[0][0], weight_src1_ch1, num_bins * sizeof(cplx_t));
        memcpy(coeffs->bins[0][1], weight_src1_ch2, num_bins * sizeof(cplx_t));
        memcpy(coeffs->bins[0][2], weight_src1_ch3, num_bins * sizeof(cplx_t));
        memcpy(coeffs->bins[0][3], weight_src1_ch4, num_bins * sizeof(cplx_t));

        memcpy(coeffs->bins[1][0], weight_src2_ch1, num_bins * sizeof(cplx_t));
        memcpy(coeffs->bins[1][1], weight_src2_ch2, num_bins * sizeof(cplx_t));
        memcpy(coeffs->bins[1][2], weight_src2_ch3, num_bins * sizeof(cplx_t));
        memcpy(coeffs->bins[1][3], weight_src2_ch4, num_bins * sizeof(cplx_t));

        beamformer_process(beamformer, in, coeffs, out);

        for (unsigned int index_bin = 0; index_bin < num_bins; index_bin++) {

            if (!(fabsf(out->bins[0][index_bin].real - freq_out_src1[index_bin].real) < eps)) {
                return -1;
            }
            if (!(fabsf(out->bins[0][index_bin].imag - freq_out_src1[index_bin].imag) < eps)) {
                return -1;
            }
            if (!(fabsf(out->bins[1][index_bin].real - freq_out_src2[index_bin].real) < eps)) {
                return -1;
            }
            if (!(fabsf(out->bins[1][index_bin].imag - freq_out_src2[index_bin].imag) < eps)) {
                return -1;
            }

        }

        freqs_destroy(in);
        freqs_destroy(out);
        weights_destroy(coeffs);

        beamformer_destroy(beamformer);

    }

    return 0;

}