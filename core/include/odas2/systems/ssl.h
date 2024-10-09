#ifndef __SSL_H
#define __SSL_H

#include "../signals/doas.h"
#include "../signals/tdoas.h"
#include "../utils/mics.h"
#include "../utils/points.h"

typedef struct aoa_t {

    float degree;
    float amplitude;

} aoa_t;

typedef struct ssl_t {

    unsigned int num_channels;
    unsigned int num_pairs;
    unsigned int num_sources;
    unsigned int num_directions;
    unsigned int num_points;

    float sample_rate;
    float sound_speed;

    float * distances;
    aoa_t ** aoas;

    const points_t * points;
    const mics_t * mics;
    unsigned char * table;
    float * norms;

    unsigned int kernel_size;
    const float * kernel;

    float ** synthesis;

    float * projections;

} ssl_t;

ssl_t * ssl_construct(const mics_t * mics, const points_t * points, const float sample_rate, const float sound_speed, const unsigned int num_sources, const unsigned int num_directions);

void ssl_destroy(ssl_t * obj);

int ssl_process(ssl_t * obj, const tdoas_t * tdoas, doas_t * doas);

void ssl_printf(const ssl_t * obj);

static const float kernel_gaussian[11] = { 1.00f, 0.96f, 0.85f, 0.70f, 0.53f, 0.37f, 0.24f, 0.14f, 0.08f, 0.04f, 0.02f };

#endif // __SSL_H