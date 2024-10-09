#ifndef __DELAYSUM_H
#define __DELAYSUM_H

#include "../signals/tdoas.h"
#include "../signals/weights.h"

typedef struct delaysum_t {

    unsigned int num_sources;
    unsigned int num_channels;
    unsigned int num_bins;

} delaysum_t;

delaysum_t * delaysum_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins);

void delaysum_destroy(delaysum_t * obj);

int delaysum_process(delaysum_t * obj, const tdoas_t * tdoas, weights_t * coeffs);

#endif // __DELAYSUM_H