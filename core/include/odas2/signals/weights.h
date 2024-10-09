#ifndef __WEIGHTS_H
#define __WEIGHTS_H

#include "../types/cplx.h"
#include <stdio.h>

typedef struct weights_t {

    char label[64];

    unsigned int num_sources;
    unsigned int num_channels;
    unsigned int num_bins;

    cplx_t *** bins;

} weights_t;

weights_t * weights_construct(const char * label, const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins);

void weights_destroy(weights_t * obj);

void weights_fprintf(const weights_t * obj, FILE * fp);

#endif // __WEIGHTS_H