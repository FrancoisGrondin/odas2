#ifndef __RANKONE_H
#define __RANKONE_H

#include "../signals/covs.h"
#include "../types/cplx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rankone_t {

    unsigned int num_channels;
    unsigned int num_pairs;
    unsigned int num_bins;
    unsigned int num_iterations;

    cplx_t * A;
    cplx_t * bk;
    cplx_t * bkp1;
    cplx_t * bs;

} rankone_t;

rankone_t * rankone_construct(const unsigned int num_channels, const unsigned int num_bins, const unsigned int num_iterations);

void rankone_destroy(rankone_t * obj);

int rankone_process(rankone_t * obj, const covs_t * full, const covs_t * proj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __RANKONE_H