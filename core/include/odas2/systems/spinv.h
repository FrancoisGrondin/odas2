#ifndef __SPINV_H
#define __SPINV_H

#include "../utils/cholesky.h"
#include "../signals/covs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct spinv_t {

    unsigned int num_channels;
    unsigned int num_pairs;
    unsigned int num_bins;

    float epsilon;
    float gamma;

    choleskycplx_t * inv;
    float * interf_acorr;
    cplx_t * interf_xcorr;
    float * interfinv_acorr;
    cplx_t * interfinv_xcorr;

} spinv_t;

spinv_t * spinv_construct(const unsigned int num_channels, const unsigned int num_bins, const float epsilon, const float gamma);

void spinv_destroy(spinv_t * obj);

int spinv_process(spinv_t * obj, const covs_t * interf, covs_t * invinterf);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __SPINV_H
