#ifndef __POINTS_H
#define __POINTS_H

#include "../types/xyz.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct points_t {

    unsigned int num_points;
    const xyz_t * points;

} points_t;

points_t * points_construct(const char * geometry, const unsigned int num_points);

void points_destroy(points_t * obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __POINTS_H
