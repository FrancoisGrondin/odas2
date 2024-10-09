#include "test_stft.h"

int test_stft(void) {

    const float eps = 0.001f;

    {

        const unsigned int num_channels = 2;
        const unsigned int num_samples = 16;
        const unsigned int num_shifts = 4;
        const unsigned int num_bins = 9;
        const char * window = "hann";

        const float hop_ch1_frame1[4] = { +1.0f, -2.0f, +2.0f, +1.0f };
        const float hop_ch1_frame2[4] = { -1.0f, -3.0f, +0.0f, -3.0f };
        const float hop_ch1_frame3[4] = { +2.0f, -1.0f, +1.0f, +2.0f };
        const float hop_ch1_frame4[4] = { +4.0f, -3.0f, +3.0f, +1.0f };
        const float hop_ch1_frame5[4] = { +1.0f, +1.0f, -2.0f, +1.0f };

        const float hop_ch2_frame1[4] = { +2.0f, +3.0f, +0.0f, -1.0f };
        const float hop_ch2_frame2[4] = { -3.0f, -1.0f, +1.0f, -1.0f };
        const float hop_ch2_frame3[4] = { +1.0f, +2.0f, +2.0f, -3.0f };
        const float hop_ch2_frame4[4] = { +3.0f, -4.0f, +0.0f, +2.0f };
        const float hop_ch2_frame5[4] = { +0.0f, -2.0f, +1.0f, +3.0f };

        const cplx_t freq_ch1_frame1[9] = { { .real = +0.101f, .imag = +0.000f },
                                            { .real = -0.065f, .imag = +0.101f },
                                            { .real = -0.112f, .imag = -0.148f },
                                            { .real = +0.245f, .imag = -0.158f },
                                            { .real = +0.259f, .imag = +0.331f },
                                            { .real = -0.367f, .imag = +0.411f },
                                            { .real = -0.579f, .imag = -0.320f },
                                            { .real = +0.188f, .imag = -0.712f },
                                            { .real = +0.763f, .imag = +0.000f } };

        const cplx_t freq_ch1_frame2[9] = { { .real = +0.391f, .imag = +0.000f }, 
                                            { .real = -0.780f, .imag = +0.075f }, 
                                            { .real = +0.016f, .imag = -0.962f }, 
                                            { .real = +1.733f, .imag = -0.287f }, 
                                            { .real = -0.856f, .imag = +2.858f }, 
                                            { .real = -1.589f, .imag = -3.099f }, 
                                            { .real = +2.653f, .imag = +2.038f }, 
                                            { .real = -3.320f, .imag = -1.356f }, 
                                            { .real = +3.897f, .imag = +0.000f } };

        const cplx_t freq_ch1_frame3[9] = { { .real = -2.940f, .imag = +0.000f },
                                            { .real = +2.478f, .imag = -2.825f },
                                            { .real = -1.103f, .imag = +4.464f },
                                            { .real = +0.134f, .imag = -4.685f },
                                            { .real = -1.598f, .imag = +3.711f },
                                            { .real = +4.341f, .imag = -1.910f },
                                            { .real = -3.362f, .imag = +0.760f },
                                            { .real = -2.997f, .imag = -0.605f },
                                            { .real = +7.153f, .imag = +0.000f } };

        const cplx_t freq_ch1_frame4[9] = { { .real = -1.236f, .imag = +0.000f },
                                            { .real = +1.695f, .imag = +5.466f },
                                            { .real = -0.734f, .imag = -5.316f },
                                            { .real = -1.244f, .imag = -0.522f },
                                            { .real = +1.597f, .imag = +2.220f },
                                            { .real = -2.303f, .imag = +2.570f },
                                            { .real = +3.031f, .imag = -3.413f },
                                            { .real = -6.061f, .imag = +0.822f },
                                            { .real = +9.273f, .imag = +0.000f } };

        const cplx_t freq_ch1_frame5[9] = { { .real = +6.541f, .imag = +0.000f },
                                            { .real = -5.946f, .imag = +0.699f },
                                            { .real = +2.650f, .imag = +1.906f },
                                            { .real = -0.811f, .imag = -3.524f },
                                            { .real = +2.338f, .imag = +4.922f },
                                            { .real = -2.518f, .imag = -6.822f },
                                            { .real = +2.362f, .imag = +4.770f },
                                            { .real = -6.550f, .imag = +0.437f },
                                            { .real = 10.408f, .imag = +0.000f } };

        const cplx_t freq_ch2_frame1[9] = { { .real = +1.187f, .imag = +0.000f }, 
                                            { .real = +0.190f, .imag = +1.150f },
                                            { .real = -1.042f, .imag = +0.351f }, 
                                            { .real = -0.459f, .imag = -0.881f }, 
                                            { .real = +0.691f, .imag = -0.496f }, 
                                            { .real = +0.459f, .imag = +0.501f }, 
                                            { .real = -0.340f, .imag = +0.351f },
                                            { .real = -0.190f, .imag = -0.232f }, 
                                            { .real = +0.195f, .imag = +0.000f } };

        const cplx_t freq_ch2_frame2[9] = { { .real = +2.981f, .imag = +0.000f },
                                            { .real = -4.307f, .imag = -0.631f },
                                            { .real = +5.441f, .imag = -1.602f },
                                            { .real = -3.405f, .imag = +3.849f },
                                            { .real = +0.898f, .imag = -3.100f },
                                            { .real = -0.613f, .imag = +1.715f },
                                            { .real = +0.588f, .imag = -1.688f },
                                            { .real = +0.411f, .imag = +1.381f },
                                            { .real = -1.011f, .imag = +0.000f } };

        const cplx_t freq_ch2_frame3[9] = { { .real = -0.546f, .imag = +0.000f },
                                            { .real = +3.724f, .imag = -2.418f },
                                            { .real = -7.191f, .imag = +1.492f },
                                            { .real = +5.424f, .imag = +2.374f },
                                            { .real = -2.354f, .imag = -3.218f },
                                            { .real = +1.449f, .imag = -0.327f },
                                            { .real = -1.644f, .imag = +2.819f },
                                            { .real = +1.272f, .imag = -2.083f },
                                            { .real = -0.816f, .imag = +0.000f } };

        const cplx_t freq_ch2_frame4[9] = { { .real = +0.309f, .imag = +0.000f },
                                            { .real = -2.791f, .imag = +3.006f },
                                            { .real = +4.695f, .imag = -2.248f },
                                            { .real = -0.846f, .imag = +0.661f },
                                            { .real = -2.036f, .imag = -3.518f },
                                            { .real = +2.268f, .imag = +5.206f },
                                            { .real = -1.476f, .imag = -1.057f },
                                            { .real = -2.587f, .imag = -3.223f },
                                            { .real = +5.238f, .imag = +0.000f } };

        const cplx_t freq_ch2_frame5[9] = { { .real = +0.837f, .imag = +0.000f },
                                            { .real = +0.690f, .imag = -2.502f },
                                            { .real = -3.636f, .imag = +2.468f },
                                            { .real = +3.699f, .imag = -1.229f },
                                            { .real = +1.502f, .imag = +0.284f },
                                            { .real = -7.370f, .imag = +0.398f },
                                            { .real = +8.465f, .imag = -0.906f },
                                            { .real = -8.887f, .imag = +1.333f },
                                            { .real = 10.237f, .imag = +0.000f} };


        hops_t * hops = hops_construct("xs", num_channels, num_shifts);
        stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, num_bins, window);
        freqs_t * freqs = freqs_construct("Xs", num_channels, num_bins);

        memcpy(hops->samples[0], hop_ch1_frame1, sizeof(hop_ch1_frame1));
        memcpy(hops->samples[1], hop_ch2_frame1, sizeof(hop_ch2_frame1));

        stft_process(stft, hops, freqs);

        for (unsigned int index_bin = 0; index_bin < num_bins; index_bin++) {
            if (!(sqrtf(cplx_l2(cplx_sub(freqs->bins[0][index_bin], freq_ch1_frame1[index_bin]))) < eps)) {
                return -1;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(freqs->bins[1][index_bin], freq_ch2_frame1[index_bin]))) < eps)) {
                return -2;
            }            
        }

        memcpy(hops->samples[0], hop_ch1_frame2, sizeof(hop_ch1_frame2));
        memcpy(hops->samples[1], hop_ch2_frame2, sizeof(hop_ch2_frame2));

        stft_process(stft, hops, freqs);

        for (unsigned int index_bin = 0; index_bin < num_bins; index_bin++) {
            if (!(sqrtf(cplx_l2(cplx_sub(freqs->bins[0][index_bin], freq_ch1_frame2[index_bin]))) < eps)) {
                return -3;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(freqs->bins[1][index_bin], freq_ch2_frame2[index_bin]))) < eps)) {
                return -4;
            }            
        }

        memcpy(hops->samples[0], hop_ch1_frame3, sizeof(hop_ch1_frame3));
        memcpy(hops->samples[1], hop_ch2_frame3, sizeof(hop_ch2_frame3));

        stft_process(stft, hops, freqs);

        for (unsigned int index_bin = 0; index_bin < num_bins; index_bin++) {
            if (!(sqrtf(cplx_l2(cplx_sub(freqs->bins[0][index_bin], freq_ch1_frame3[index_bin]))) < eps)) {
                return -5;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(freqs->bins[1][index_bin], freq_ch2_frame3[index_bin]))) < eps)) {
                return -6;
            }            
        }

        memcpy(hops->samples[0], hop_ch1_frame4, sizeof(hop_ch1_frame4));
        memcpy(hops->samples[1], hop_ch2_frame4, sizeof(hop_ch2_frame4));

        stft_process(stft, hops, freqs);

        for (unsigned int index_bin = 0; index_bin < num_bins; index_bin++) {
            if (!(sqrtf(cplx_l2(cplx_sub(freqs->bins[0][index_bin], freq_ch1_frame4[index_bin]))) < eps)) {
                return -7;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(freqs->bins[1][index_bin], freq_ch2_frame4[index_bin]))) < eps)) {
                return -8;
            }            
        }

        memcpy(hops->samples[0], hop_ch1_frame5, sizeof(hop_ch1_frame5));
        memcpy(hops->samples[1], hop_ch2_frame5, sizeof(hop_ch2_frame5));

        stft_process(stft, hops, freqs);

        for (unsigned int index_bin = 0; index_bin < num_bins; index_bin++) {
            if (!(sqrtf(cplx_l2(cplx_sub(freqs->bins[0][index_bin], freq_ch1_frame5[index_bin]))) < eps)) {
                return -9;
            }
            if (!(sqrtf(cplx_l2(cplx_sub(freqs->bins[1][index_bin], freq_ch2_frame5[index_bin]))) < eps)) {
                return -10;
            }            
        }           

        hops_destroy(hops);
        stft_destroy(stft);
        freqs_destroy(freqs);

    }

    {

        const unsigned int num_channels = 2;
        const unsigned int num_samples = 16;
        const unsigned int num_shifts = 4;
        const unsigned int num_bins = 9;
        const char * window = "hann";

        const cplx_t freq_ch1_frame1[9] = { { .real = +2.0f, .imag = +0.0f },
                                            { .real = -1.0f, .imag = +1.0f },
                                            { .real = +3.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = -1.0f },
                                            { .real = -2.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = +2.0f },
                                            { .real = +2.0f, .imag = -1.0f },
                                            { .real = -3.0f, .imag = -1.0f },
                                            { .real = +1.0f, .imag = +0.0f } };

        const cplx_t freq_ch1_frame2[9] = { { .real = +1.0f, .imag = +0.0f },
                                            { .real = +1.0f, .imag = -2.0f },
                                            { .real = +2.0f, .imag = +1.0f },
                                            { .real = +3.0f, .imag = +2.0f },
                                            { .real = -1.0f, .imag = -1.0f },
                                            { .real = -2.0f, .imag = -1.0f },
                                            { .real = +1.0f, .imag = +2.0f },
                                            { .real = +0.0f, .imag = +3.0f },
                                            { .real = +2.0f, .imag = +0.0f } };

        const cplx_t freq_ch1_frame3[9] = { { .real = -2.0f, .imag = +0.0f },
                                            { .real = +3.0f, .imag = +1.0f },
                                            { .real = +1.0f, .imag = -2.0f },
                                            { .real = +1.0f, .imag = -1.0f },
                                            { .real = -2.0f, .imag = +0.0f },
                                            { .real = -3.0f, .imag = -2.0f },
                                            { .real = +1.0f, .imag = -2.0f },
                                            { .real = +0.0f, .imag = +3.0f },
                                            { .real = +1.0f, .imag = +0.0f } };

        const cplx_t freq_ch1_frame4[9] = { { .real = +3.0f, .imag = +0.0f },
                                            { .real = +1.0f, .imag = +2.0f },
                                            { .real = -2.0f, .imag = -2.0f },
                                            { .real = -1.0f, .imag = -2.0f },
                                            { .real = -2.0f, .imag = -3.0f },
                                            { .real = -3.0f, .imag = +1.0f },
                                            { .real = -2.0f, .imag = +2.0f },
                                            { .real = +3.0f, .imag = +3.0f },
                                            { .real = +2.0f, .imag = +0.0f } };

        const cplx_t freq_ch1_frame5[9] = { { .real = +1.0f, .imag = +0.0f },
                                            { .real = +2.0f, .imag = -3.0f },
                                            { .real = +3.0f, .imag = +2.0f },
                                            { .real = +2.0f, .imag = -1.0f },
                                            { .real = +1.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = +1.0f },
                                            { .real = -1.0f, .imag = -2.0f },
                                            { .real = -2.0f, .imag = +3.0f },
                                            { .real = -3.0f, .imag = +0.0f } };                                            

        const cplx_t freq_ch2_frame1[9] = { { .real = +1.0f, .imag = +0.0f },
                                            { .real = -2.0f, .imag = +1.0f },
                                            { .real = +2.0f, .imag = +0.0f },
                                            { .real = +1.0f, .imag = -1.0f },
                                            { .real = -2.0f, .imag = +2.0f },
                                            { .real = -2.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = -1.0f },
                                            { .real = +3.0f, .imag = -1.0f },
                                            { .real = +2.0f, .imag = +0.0f } };

        const cplx_t freq_ch2_frame2[9] = { { .real = +3.0f, .imag = +0.0f },
                                            { .real = +2.0f, .imag = -3.0f },
                                            { .real = +1.0f, .imag = +3.0f },
                                            { .real = -1.0f, .imag = -1.0f },
                                            { .real = -1.0f, .imag = -2.0f },
                                            { .real = +2.0f, .imag = +2.0f },
                                            { .real = -1.0f, .imag = +2.0f },
                                            { .real = +0.0f, .imag = -1.0f },
                                            { .real = -1.0f, .imag = +0.0f } };

        const cplx_t freq_ch2_frame3[9] = { { .real = +1.0f, .imag = +0.0f },
                                            { .real = +2.0f, .imag = -1.0f },
                                            { .real = -1.0f, .imag = +1.0f },
                                            { .real = -1.0f, .imag = +0.0f },
                                            { .real = +2.0f, .imag = -2.0f },
                                            { .real = +2.0f, .imag = +0.0f },
                                            { .real = +2.0f, .imag = +2.0f },
                                            { .real = -1.0f, .imag = -3.0f },
                                            { .real = +1.0f, .imag = +0.0f } };          

        const cplx_t freq_ch2_frame4[9] = { { .real = +0.0f, .imag = +0.0f },
                                            { .real = -1.0f, .imag = +1.0f },
                                            { .real = +2.0f, .imag = -3.0f },
                                            { .real = -2.0f, .imag = +1.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = -1.0f, .imag = +2.0f },
                                            { .real = +1.0f, .imag = -1.0f },
                                            { .real = -2.0f, .imag = +1.0f },
                                            { .real = +1.0f, .imag = +0.0f } };

        const cplx_t freq_ch2_frame5[9] = { { .real = -1.0f, .imag = +0.0f },
                                            { .real = +0.0f, .imag = -1.0f },
                                            { .real = +1.0f, .imag = +1.0f },
                                            { .real = +1.0f, .imag = +2.0f },
                                            { .real = +2.0f, .imag = +2.0f },
                                            { .real = -3.0f, .imag = +1.0f },
                                            { .real = +3.0f, .imag = +2.0f },
                                            { .real = -2.0f, .imag = +1.0f },
                                            { .real = +3.0f, .imag = +0.0f } };                                               

        const float hop_ch1_frame1[4] = { +0.000f, +0.002f, -0.007f, +0.197f };
        const float hop_ch1_frame2[4] = { -0.725f, -0.392f, +1.090f, -0.380f };
        const float hop_ch1_frame3[4] = { +1.307f, +0.492f, +0.948f, -0.580f };
        const float hop_ch1_frame4[4] = { +0.060f, -0.923f, +0.398f, -1.319f };
        const float hop_ch1_frame5[4] = { -0.466f, -0.359f, -0.556f, +0.255f };
        const float hop_ch2_frame1[4] = { +0.000f, -0.030f, +0.096f, -0.121f };
        const float hop_ch2_frame2[4] = { -0.587f, +0.125f, +0.537f, +0.616f };
        const float hop_ch2_frame3[4] = { +0.240f, +1.004f, +0.665f, +1.244f };
        const float hop_ch2_frame4[4] = { -0.306f, +0.438f, -0.616f, -0.987f };
        const float hop_ch2_frame5[4] = { +0.118f, -1.192f, -0.055f, -1.476f };

        freqs_t * freqs = freqs_construct("Xs", num_channels, num_bins);
        istft_t * istft = istft_construct(num_channels, num_samples, num_shifts, num_bins, window);
        hops_t * hops = hops_construct("xs", num_channels, num_shifts);

        memcpy(freqs->bins[0], freq_ch1_frame1, sizeof(freq_ch1_frame1));
        memcpy(freqs->bins[1], freq_ch2_frame1, sizeof(freq_ch2_frame1));

        istft_process(istft, freqs, hops);

        for (unsigned int index_shift = 0; index_shift < num_shifts; index_shift++) {
            if (!(fabsf(hops->samples[0][index_shift] - hop_ch1_frame1[index_shift]) < eps)) {
                return -11;
            }
        }
        for (unsigned int index_shift = 0; index_shift < num_shifts; index_shift++) {
            if (!(fabsf(hops->samples[1][index_shift] - hop_ch2_frame1[index_shift]) < eps)) {
                return -12;
            }
        }

        memcpy(freqs->bins[0], freq_ch1_frame2, sizeof(freq_ch1_frame2));
        memcpy(freqs->bins[1], freq_ch2_frame2, sizeof(freq_ch2_frame2));

        istft_process(istft, freqs, hops);

        for (unsigned int index_shift = 0; index_shift < num_shifts; index_shift++) {
            if (!(fabsf(hops->samples[0][index_shift] - hop_ch1_frame2[index_shift]) < eps)) {
                return -13;
            }
        }
        for (unsigned int index_shift = 0; index_shift < num_shifts; index_shift++) {
            if (!(fabsf(hops->samples[1][index_shift] - hop_ch2_frame2[index_shift]) < eps)) {
                return -14;
            }
        }        

        memcpy(freqs->bins[0], freq_ch1_frame3, sizeof(freq_ch1_frame3));
        memcpy(freqs->bins[1], freq_ch2_frame3, sizeof(freq_ch2_frame3));

        istft_process(istft, freqs, hops);

        for (unsigned int index_shift = 0; index_shift < num_shifts; index_shift++) {
            if (!(fabsf(hops->samples[0][index_shift] - hop_ch1_frame3[index_shift]) < eps)) {
                return -15;
            }
        }
        for (unsigned int index_shift = 0; index_shift < num_shifts; index_shift++) {
            if (!(fabsf(hops->samples[1][index_shift] - hop_ch2_frame3[index_shift]) < eps)) {
                return -16;
            }
        }        

        memcpy(freqs->bins[0], freq_ch1_frame4, sizeof(freq_ch1_frame4));
        memcpy(freqs->bins[1], freq_ch2_frame4, sizeof(freq_ch2_frame4));

        istft_process(istft, freqs, hops);

        for (unsigned int index_shift = 0; index_shift < num_shifts; index_shift++) {
            if (!(fabsf(hops->samples[0][index_shift] - hop_ch1_frame4[index_shift]) < eps)) {
                return -17;
            }
        }
        for (unsigned int index_shift = 0; index_shift < num_shifts; index_shift++) {
            if (!(fabsf(hops->samples[1][index_shift] - hop_ch2_frame4[index_shift]) < eps)) {
                return -18;
            }
        }

        memcpy(freqs->bins[0], freq_ch1_frame5, sizeof(freq_ch1_frame5));
        memcpy(freqs->bins[1], freq_ch2_frame5, sizeof(freq_ch2_frame5));

        istft_process(istft, freqs, hops);

        for (unsigned int index_shift = 0; index_shift < num_shifts; index_shift++) {
            if (!(fabsf(hops->samples[0][index_shift] - hop_ch1_frame5[index_shift]) < eps)) {
                return -19;
            }
        }
        for (unsigned int index_shift = 0; index_shift < num_shifts; index_shift++) {
            if (!(fabsf(hops->samples[1][index_shift] - hop_ch2_frame5[index_shift]) < eps)) {
                return -20;
            }
        }     

        freqs_destroy(freqs);        
        istft_destroy(istft);
        hops_destroy(hops);

    }

    return 0;

}