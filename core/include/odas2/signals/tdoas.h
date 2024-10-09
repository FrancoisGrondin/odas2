#ifndef __TDOAS_H
#define __TDOAS_H

#include <stdio.h>

typedef struct tau_t {

    float delay;
    float amplitude;

} tau_t;

typedef struct tdoas_t {

    char label[64];

    unsigned int num_sources;
    unsigned int num_channels;
    unsigned int num_pairs;

    tau_t ** taus;

} tdoas_t;

tdoas_t * tdoas_construct(const char * label, const unsigned int num_channels, const unsigned int num_sources);

void tdoas_destroy(tdoas_t * obj);

void tdoas_fprintf(const tdoas_t * obj, FILE * fp);

#endif // __TDOAS_H