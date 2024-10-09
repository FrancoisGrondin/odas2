#ifndef __MASKS_H
#define __MASKS_H

#include <stdio.h>

typedef struct masks_t {

    char label[64];

    unsigned int num_channels;
    unsigned int num_bins;

    float ** gains;

} masks_t;

masks_t * masks_construct(const char * label, const unsigned int num_channels, const unsigned int num_bins);

void masks_destroy(masks_t * obj);

void masks_ones(masks_t * obj);

void masks_zeros(masks_t * obj);

void masks_fprintf(const masks_t * obj, FILE * fp);


#endif // __MASKS_H