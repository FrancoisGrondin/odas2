#ifndef __STEERING_H
#define __STEERING_H

#include "../signals/doas.h"
#include "../signals/tdoas.h"
#include "../utils/mics.h"

typedef struct steering_t {

    unsigned int num_channels;
    unsigned int num_pairs;
    unsigned int num_sources;

    const mics_t * mics;
    float sample_rate;
    float sound_speed;

} steering_t;

steering_t * steering_construct(const mics_t * mics, const float sample_rate, const float sound_speed, const unsigned int num_sources);

void steering_destroy(steering_t * obj);

int steering_process(const steering_t * obj, const doas_t * doas, tdoas_t * tdoas);

#endif // __STEERING_H