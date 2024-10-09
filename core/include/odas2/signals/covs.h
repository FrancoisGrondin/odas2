#ifndef __COVS_H
#define __COVS_H

#include "../types/cplx.h"
#include <stdio.h>

typedef struct covs_t {

    char label[64];

    unsigned int num_channels;
    unsigned int num_pairs;
    unsigned int num_bins;

    cplx_t ** xcorrs;
    float ** acorrs;

} covs_t;

covs_t * covs_construct(const char * label, const unsigned int num_channels, const unsigned int num_bins);

void covs_destroy(covs_t * obj);

void covs_fprintf(const covs_t * obj, FILE * fp);

#endif // __COVS_H