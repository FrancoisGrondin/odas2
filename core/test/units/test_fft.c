#include "test_fft.h"

int test_fft(void) {

    const float eps = 0.001f;

    {

        // For real FFT with size 16, complex FFT of size 8, and thus 4 twiddle factors
        unsigned int num_samples = 16;
        cplx_t W[4] = { { .real = +1.000f, .imag = +0.000f },
                        { .real = +0.707f, .imag = -0.707f },
                        { .real = +0.000f, .imag = -1.000f },
                        { .real = -0.707f, .imag = -0.707f } };

        fft_t * rtn = fft_construct(num_samples);

        for (unsigned int n = 0; n < 4; n++) {
            if (!((fabsf(W[n].real - rtn->twiddle[n].real) < eps) && (fabsf(W[n].imag - rtn->twiddle[n].imag) < eps))) {
                return -1;
            }
        }

        fft_destroy(rtn);

    }

    {

        // For real FFT with size 16, convert convex FFT of size 8 to real FFT of size 16 with
        // coefficients that correspond to 8 twiddle factors
        unsigned int num_samples = 16;
        cplx_t W[8] = { { .real = +1.000f, .imag = +0.000f },
                        { .real = +0.924f, .imag = -0.383f },
                        { .real = +0.707f, .imag = -0.707f },
                        { .real = +0.383f, .imag = -0.924f },
                        { .real = +0.000f, .imag = -1.000f },
                        { .real = -0.383f, .imag = -0.924f },
                        { .real = -0.707f, .imag = -0.707f },
                        { .real = -0.924f, .imag = -0.383f } };

        fft_t * rtn = fft_construct(num_samples);

        for (unsigned int n = 0; n < 8; n++) {
            if (!((fabsf(W[n].real - rtn->coefficients[n].real) < eps) && (fabsf(W[n].imag - rtn->coefficients[n].imag) < eps))) {
                return -2;
            }
        }        

        fft_destroy(rtn);

    }

    {

        // For real FFT with size 16, complex FFT of size 8, and thus reverse bits
        // with a total of 8 samples
        unsigned int num_samples = 16;
        unsigned int nrev[8] = { 0, 4, 2, 6, 1, 5, 3, 7 };

        fft_t * rtn = fft_construct(num_samples);

        for (unsigned int n = 0; n < 8; n++) {
            if (!(rtn->revbits[n] == nrev[n])) {
                return -3;
            }
        }

        fft_destroy(rtn);

    }

    {

        unsigned int num_samples = 16;

        float in[16] = { +1.0f, -2.0f, +0.0f, +2.0f, 
                         +3.0f, -1.0f, -1.0f, +4.0f, 
                         -3.0f, -2.0f, +2.0f, -4.0f,
                         -1.0f, +0.0f, +1.0f, +1.0f };

        cplx_t rtn_out[9] = { { .real =  +0.000f, .imag =  +0.000f },
                              { .real =  +3.907f, .imag =  -6.939f },
                              { .real =  -1.172f, .imag =  +5.071f },
                              { .real =  -3.615f, .imag =  +5.970f },
                              { .real =  -2.000f, .imag =  +8.000f },
                              { .real = +11.615f, .imag =  -7.687f },
                              { .real =  -6.828f, .imag =  +9.071f },
                              { .real =  +4.093f, .imag =  -4.596f },
                              { .real =  +4.000f, .imag =  +0.000f } };

        cplx_t out[9];

        fft_t * obj = fft_construct(num_samples);

        fft_rfft(obj, in, out);

        for (unsigned int k = 0; k < 9; k++) {
            if (!((fabsf(out[k].real - rtn_out[k].real) < eps) && ((fabsf(out[k].imag - rtn_out[k].imag) < eps)))) {
                return -4;
            }
        }

        fft_destroy(obj);
    }

    {

        unsigned int num_samples = 16;

        cplx_t in[9] = { { .real =  +0.000f, .imag =  +0.000f },
                         { .real =  +3.907f, .imag =  -6.939f },
                         { .real =  -1.172f, .imag =  +5.071f },
                         { .real =  -3.615f, .imag =  +5.970f },
                         { .real =  -2.000f, .imag =  +8.000f },
                         { .real = +11.615f, .imag =  -7.687f },
                         { .real =  -6.828f, .imag =  +9.071f },
                         { .real =  +4.093f, .imag =  -4.596f },
                         { .real =  +4.000f, .imag =  +0.000f } };

        float rtn_out[16] = { +1.0f, -2.0f, +0.0f, +2.0f, 
                              +3.0f, -1.0f, -1.0f, +4.0f, 
                              -3.0f, -2.0f, +2.0f, -4.0f,
                              -1.0f, +0.0f, +1.0f, +1.0f };

        float out[16];

        fft_t * obj = fft_construct(num_samples);

        fft_irfft(obj, in, out);

        for (unsigned int n = 0; n < 16; n++) {
            if (!(fabsf(out[n] - rtn_out[n]) < eps)) {
                return -5;
            }
        }

        fft_destroy(obj);

    }

    {

        unsigned int num_samples = 512;

        float in1[512];
        cplx_t out1[257];
        cplx_t in2[257];
        float out2[512];

        fft_t * obj = fft_construct(num_samples);

        for (unsigned int n = 0; n < num_samples; n++) {
            in1[n] = (float) rand() / (float)RAND_MAX;
        }

        fft_rfft(obj, in1, out1);

        for (unsigned int k = 0; k < (num_samples/2+1); k++) {
            in2[k] = out1[k];
        }

        fft_irfft(obj, in2, out2);

        for (unsigned int n = 0; n < num_samples; n++) {
            if (!(fabsf(in1[n] - out2[n]) < eps)) {
                return -6;
            }
        }

        fft_destroy(obj);

    }

    return 0;

}