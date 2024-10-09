#ifndef __MVDR_H
#define __MVDR_H

#include "../utils/linalg.h"
#include "../signals/covs.h"
#include "../signals/freqs.h"
#include "../signals/weights.h"

typedef struct mvdr_t {

    unsigned int num_channels;
    unsigned int num_bins;

} mvdr_t;

mvdr_t * mvdr_construct(const unsigned int num_channels, const unsigned int num_bins);

void mvdr_destroy(mvdr_t * obj);

int mvdr_process(mvdr_t * obj, const covs_t * covs, weights_t * weights);

#endif // __MVDR_H