#include "test_points.h"

int test_points(void) {

	const float eps = 0.001f;

	{

		points_t * points = points_construct(POINTS_GEOMETRY_SPHERE, 2000);

		if (!(points->num_points == 2000)) {
			return -1;
		}

		points_destroy(points);

	}

	{

		points_t * points = points_construct(POINTS_GEOMETRY_SPHERE, 2000);

		for (unsigned int index_point = 0; index_point < points->num_points; index_point++) {
			if (!(fabsf(xyz_mag(points->points[index_point]) - 1.0f) < eps)) {
				return -2;
			}
		}

		points_destroy(points);

	}

	{

		points_t * points = points_construct(POINTS_GEOMETRY_HALFSPHERE, 1000);

		if (!(points->num_points == 1000)) {
			return -3;
		}

		points_destroy(points);

	}

	{

		points_t * points = points_construct(POINTS_GEOMETRY_HALFSPHERE, 1000);

		for (unsigned int index_point = 0; index_point < points->num_points; index_point++) {
			if (!(fabsf(xyz_mag(points->points[index_point]) - 1.0f) < eps)) {
				return -4;
			}
		}

		points_destroy(points);

	}

	{

		points_t * points = points_construct(POINTS_GEOMETRY_HALFSPHERE, 1000);

		for (unsigned int index_point = 0; index_point < points->num_points; index_point++) {
			if (!(points->points[index_point].z >= 0.0f)) {
				return -5;
			}
		}

		points_destroy(points);

	}

	{

		points_t * points = points_construct(POINTS_GEOMETRY_ARC, 181);

		if (!(points->num_points == 181)) {
			return -6;
		}

		points_destroy(points);

	}

	{

		points_t * points = points_construct(POINTS_GEOMETRY_ARC, 181);

		for (unsigned int index_point = 0; index_point < points->num_points; index_point++) {
			if (!(fabsf(xyz_mag(points->points[index_point]) - 1.0f) < eps)) {
				return -7;
			}
		}

		points_destroy(points);

	}

	{

		points_t * points = points_construct(POINTS_GEOMETRY_ARC, 181);

		for (unsigned int index_point = 0; index_point < points->num_points; index_point++) {
			if (!(points->points[index_point].z == 0.0f)) {
				return -8;
			}
		}

		points_destroy(points);

	}

	return 0;

}
