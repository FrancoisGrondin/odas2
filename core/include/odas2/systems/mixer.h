#ifndef __MIXER_H
#define __MIXER_H

#include "../signals/hops.h"

typedef struct mixer_t {

    unsigned int * map;
    unsigned int num_channels;

} mixer_t;

mixer_t * mixer_construct(const char * channels);

void mixer_destroy(mixer_t * obj);

int mixer_process(mixer_t * obj, const hops_t * hops_in, hops_t * hops_out);

#endif // __MIXER_H