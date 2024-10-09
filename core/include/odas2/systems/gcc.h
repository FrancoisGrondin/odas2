#ifndef __GCC_H
#define __GCC_H

#include "../utils/fft.h"
#include "../signals/covs.h"
#include "../signals/tdoas.h"

typedef struct gcc_t {

    unsigned int num_sources;
    unsigned int num_channels;
    unsigned int num_pairs;
    unsigned int num_bins;
    unsigned int num_samples;
    unsigned int interpolation_factor;

    fft_t * fft;

    cplx_t * XX;
    float * xx;

} gcc_t;

gcc_t * gcc_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins);

void gcc_destroy(gcc_t * obj);

int gcc_process(gcc_t * obj, const covs_t * covs, tdoas_t * tdoas);

#endif // __GCC_H