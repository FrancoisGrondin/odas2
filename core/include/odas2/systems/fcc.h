#ifndef __FCC_H
#define __FCC_H

#include "../utils/fft.h"
#include "../signals/covs.h"
#include "../signals/tdoas.h"

typedef struct fcc_t {

    unsigned int num_channels;
    unsigned int num_bins;

    unsigned int num_dims;
    unsigned int num_bases;
    unsigned int num_dicts;

    float * taus;
    float ** bases;
    cplx_t ** dicts;

    cplx_t * x_add;
    cplx_t * x_sub;
    cplx_t * z;
    cplx_t * y;

} fcc_t;

fcc_t * fcc_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins);

void fcc_destroy(fcc_t * obj);

int fcc_process(fcc_t * obj, const covs_t * covs, tdoas_t * tdoas);

#endif // __FCC_H