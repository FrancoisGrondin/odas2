#ifndef __POINTS_H
#define __POINTS_H

#include "../types/xyz.h"

typedef struct points_t {

    unsigned int num_points;
    const xyz_t * points;

} points_t;

points_t * points_construct(const char * geometry);

void points_destroy(points_t * obj);

#endif // __POINTS_H