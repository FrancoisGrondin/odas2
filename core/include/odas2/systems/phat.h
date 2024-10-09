#ifndef __PHAT_H
#define __PHAT_H

#include "../signals/covs.h"

typedef struct phat_t {

    unsigned int num_channels;
    unsigned int num_pairs;
    unsigned int num_bins;

} phat_t;

phat_t * phat_construct(const unsigned int num_channels, const unsigned int num_bins);

void phat_destroy(phat_t * obj);

int phat_process(phat_t * obj, const covs_t * covs_in, covs_t * covs_out);

#endif // __PHAT_H