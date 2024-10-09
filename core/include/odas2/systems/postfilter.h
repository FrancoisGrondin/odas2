#ifndef __POSTFILTER_H
#define __POSTFILTER_H

#include "../signals/freqs.h"
#include "../signals/masks.h"

typedef struct postfilter_t {

    unsigned int num_channels;
    unsigned int num_bins;

} postfilter_t;

postfilter_t * postfilter_construct(const unsigned int num_channels, const unsigned int num_bins);

void postfilter_destroy(postfilter_t * obj);

int postfilter_process(postfilter_t * obj, const freqs_t * freqs_in, const masks_t * masks_in, freqs_t * freqs_out);

#endif // __PHAT_H