#ifndef __POINTS_H
#define __POINTS_H

#include "../types/xyz.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum points_geometry_t {
    POINTS_GEOMETRY_SPHERE,
    POINTS_GEOMETRY_HALFSPHERE,
    POINTS_GEOMETRY_CIRCLE,
    POINTS_GEOMETRY_ARC
} points_geometry_t;

typedef struct points_t {

    unsigned int num_points;
    const xyz_t * points;

} points_t;

points_t * points_construct(const points_geometry_t geometry, const unsigned int num_points);

void points_destroy(points_t * obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __POINTS_H
