#ifndef __FREQS_H
#define __FREQS_H

#include "../types/cplx.h"
#include <stdio.h>

typedef struct freqs_t {

    char label[64];

    unsigned int num_channels;
    unsigned int num_bins;

    cplx_t ** bins;

} freqs_t;

freqs_t * freqs_construct(const char * label, const unsigned int num_channels, const unsigned int num_bins);

void freqs_destroy(freqs_t * obj);

void freqs_fprintf(const freqs_t * obj, FILE * fp);

#endif // __FREQS_H