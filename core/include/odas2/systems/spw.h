#ifndef __SPW_H
#define __SPW_H

#include "../utils/cholesky.h"
#include "../signals/covs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct spw_t {

    unsigned int num_channels;
    unsigned int num_pairs;
    unsigned int num_bins;

    cplx_t * NNinv;
    cplx_t * SS;
    cplx_t * C;

} spw_t;

spw_t * spw_construct(const unsigned int num_channels, const unsigned int num_bins);

void spw_destroy(spw_t * obj);

int spw_process(spw_t * obj, const covs_t * target, const covs_t * invinterf, covs_t * whitened);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __SPW_H
