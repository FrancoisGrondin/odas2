#include <utils/points.h>
#include <utils/error.h>
#include <utils/pi.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

static xyz_t * generate_sphere(const unsigned int num_points) {

    xyz_t * points = (xyz_t *) calloc(sizeof(xyz_t), num_points);

    float golden_ratio = (1.0f + powf(5.0f, 0.5f)) / 2.0f;

    for (unsigned int index_point = 0; index_point < num_points; index_point++) {

        float theta = 2.f * PI_F * (float)index_point / golden_ratio;
        float phi = acosf(1.0f - 2.0f*((float)index_point + 0.5f)/((float)num_points));

        points[index_point].x = cosf(theta) * sinf(phi);
        points[index_point].y = sinf(theta) * sinf(phi);
        points[index_point].z = cosf(phi);

    }

    return points;

}

static xyz_t * generate_halfsphere(const unsigned int num_points) {

    xyz_t * points = (xyz_t *) calloc(sizeof(xyz_t), num_points);

    float golden_ratio = (1.0f + powf(5.0f, 0.5f)) / 2.0f;

    for (unsigned int index_point = 0; index_point < num_points; index_point++) {

        float theta = 2.f * PI_F * (float)(index_point) / golden_ratio;
        float phi = acosf(1.0f - 2.0f*((float)index_point + 0.5f)/((float)(num_points*2)));

        points[index_point].x = cosf(theta) * sinf(phi);
        points[index_point].y = sinf(theta) * sinf(phi);
        points[index_point].z = cosf(phi);

    }

    return points;

}

static xyz_t * generate_circle(const unsigned int num_points) {

    xyz_t * points = (xyz_t *) calloc(sizeof(xyz_t), num_points);

    for (unsigned int index_point = 0; index_point < num_points; index_point++) {

        float theta = 2.f * PI_F * ((float) index_point) / ((float) num_points);

        points[index_point].x = cosf(theta);
        points[index_point].y = sinf(theta);
        points[index_point].z = 0.0f;

    }

    return points;

}

static xyz_t * generate_arc(const unsigned int num_points) {

    xyz_t * points = (xyz_t *) calloc(sizeof(xyz_t), num_points);

    for (unsigned int index_point = 0; index_point < num_points; index_point++) {

        float theta = 2.f * PI_F * ((float) index_point) / ((float) (num_points-1));

        points[index_point].x = cosf(theta);
        points[index_point].y = sinf(theta);
        points[index_point].z = 0.0f;

    }

    return points;

}

points_t * points_construct(const points_geometry_t geometry, const unsigned int num_points) {

    points_t * obj = (points_t *) malloc(sizeof(points_t));

    obj->num_points = num_points;

    switch (geometry)
    {
        case POINTS_GEOMETRY_SPHERE:
            obj->points = generate_sphere(num_points);
            break;
        case POINTS_GEOMETRY_HALFSPHERE:
            obj->points = generate_halfsphere(num_points);
            break;
        case POINTS_GEOMETRY_CIRCLE:
            obj->points = generate_circle(num_points);
            break;
        case POINTS_GEOMETRY_ARC:
            obj->points = generate_arc(num_points);
            break;
        default:
            odas2_set_error_number(ODAS2_ERROR_POINTS_GEOMETRY_UNKNOWN);
            free((void *) obj);
            return NULL;
    }

    return obj;

}

void points_destroy(points_t * obj) {

    free((void *) obj->points);
    free((void *) obj);

}
