#ifndef __HOPS_H
#define __HOPS_H

#include <stdio.h>

typedef struct hops_t {

    char label[64];

    unsigned int num_channels;
    unsigned int num_shifts;

    float ** samples;

} hops_t;

hops_t * hops_construct(const char * label, const unsigned int num_channels, const unsigned int num_shifts);

void hops_destroy(hops_t * obj);

void hops_fprintf(const hops_t * obj, FILE * fp);

#endif // __HOPS_H