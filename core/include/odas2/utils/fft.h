#ifndef __FFT_H
#define __FFT_H

#include "../types/cplx.h"

typedef struct fft_t {

    unsigned int num_samples;
    unsigned int num_levels;
    cplx_t * working;
    cplx_t * reverse;
    cplx_t * twiddle;
    cplx_t * coefficients;
    unsigned int * revbits;

} fft_t;

fft_t * fft_construct(const unsigned int num_samples);

void fft_destroy(fft_t * obj);

void fft_rfft(fft_t * obj, const float * in, cplx_t * out);

void fft_irfft(fft_t * obj, const cplx_t * in, float * out);

#endif // __FFT_H