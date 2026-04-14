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

    float epsilon;
    float gamma;

    choleskycplx_t * inv;
    float * target_acorr;
    cplx_t * target_xcorr;
    float * interf_acorr;
    cplx_t * interf_xcorr;
    float * interfinv_acorr;
    cplx_t * interfinv_xcorr;
    float * whitened_acorr;
    cplx_t * whitened_xcorr;

    cplx_t * A;
    cplx_t * B;
    cplx_t * C;

} spw_t;

spw_t * spw_construct(const unsigned int num_channels, const unsigned int num_bins, const float epsilon, const float gamma);

void spw_destroy(spw_t * obj);

int spw_process(spw_t * obj, const covs_t * target, const covs_t * interf, covs_t * whitened);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __SPW_H
