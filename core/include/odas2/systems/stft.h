#ifndef __STFT_H
#define __STFT_H

#include "../utils/fft.h"
#include "../signals/hops.h"
#include "../signals/freqs.h"

typedef struct stft_t {

    unsigned int num_channels;
    unsigned int num_samples;
    unsigned int num_shifts;
    unsigned int num_bins;

    float * window;
    float ** frames;

    fft_t * fft;

    float * frame_real;
    cplx_t * frame_cplx;

} stft_t;

typedef struct istft_t {

    unsigned int num_channels;
    unsigned int num_samples;
    unsigned int num_shifts;
    unsigned int num_bins;

    float * window;
    float ** frames;

    fft_t * fft;

    float * frame_real;
    cplx_t * frame_cplx;

} istft_t;

stft_t * stft_construct(const unsigned int num_channels, const unsigned int num_samples, const unsigned int num_shifts, const unsigned int num_bins, const char * window);

void stft_destroy(stft_t * obj);

int stft_process(stft_t * obj, const hops_t * hops, freqs_t * freqs);

istft_t * istft_construct(const unsigned int num_channels, const unsigned int num_samples, const unsigned int num_shifts, const unsigned int num_bins, const char * window);

void istft_destroy(istft_t * obj);

int istft_process(istft_t * obj, const freqs_t * freqs, hops_t * hops);

#endif // __STFT_H