#ifndef __SCM_H
#define __SCM_H

#include "../utils/linalg.h"
#include "../signals/covs.h"
#include "../signals/freqs.h"
#include "../signals/masks.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct scm_t {

    unsigned int num_channels;
    unsigned int num_pairs;
    unsigned int num_bins;
    float alpha;

    cplx_t ** xcorrs;
    float ** acorrs;

    unsigned int ** map_index_pair;

} scm_t;

scm_t * scm_construct(const unsigned int num_channels, const unsigned int num_bins, const float alpha);

void scm_destroy(scm_t * obj);

int scm_process(scm_t * obj, const freqs_t * freqs, const masks_t * masks, covs_t * covs);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __SCM_H
