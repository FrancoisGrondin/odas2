#ifndef __POINTS_H
#define __POINTS_H

#include "../types/xyz.h"

typedef struct points_t {

    unsigned int num_points;
    const xyz_t * points;

} points_t;

points_t * points_construct(const char * geometry);

void points_destroy(points_t * obj);

static const xyz_t sphere[2562];
static const xyz_t halfsphere[1321];
static const xyz_t arc[181];

#endif // __POINTS_H