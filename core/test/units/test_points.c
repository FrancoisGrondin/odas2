#include "test_points.h"

int test_points(void) {

	const float eps = 0.001f;

	{

		points_t * points = points_construct("sphere");

		if (!(points->num_points == 2562)) {
			return -1;
		}

		points_destroy(points);

	}

	{

		points_t * points = points_construct("sphere");

		for (unsigned int index_point = 0; index_point < points->num_points; index_point++) {
			if (!(fabsf(xyz_mag(points->points[index_point]) - 1.0f) < eps)) {
				return -2;
			}
		}

		points_destroy(points);

	}

	{

		points_t * points = points_construct("halfsphere");

		if (!(points->num_points == 1321)) {
			return -3;
		}

		points_destroy(points);

	}

	{

		points_t * points = points_construct("halfsphere");

		for (unsigned int index_point = 0; index_point < points->num_points; index_point++) {
			if (!(fabsf(xyz_mag(points->points[index_point]) - 1.0f) < eps)) {
				return -4;
			}
		}

		points_destroy(points);

	}	

	{

		points_t * points = points_construct("halfsphere");

		for (unsigned int index_point = 0; index_point < points->num_points; index_point++) {
			if (!(points->points[index_point].z >= 0.0f)) {
				return -5;
			}
		}

		points_destroy(points);

	}

	{

		points_t * points = points_construct("arc");

		if (!(points->num_points == 181)) {
			return -6;
		}

		points_destroy(points);

	}	

	{

		points_t * points = points_construct("arc");

		for (unsigned int index_point = 0; index_point < points->num_points; index_point++) {
			if (!(fabsf(xyz_mag(points->points[index_point]) - 1.0f) < eps)) {
				return -7;
			}
		}

		points_destroy(points);

	}	

	{

		points_t * points = points_construct("arc");

		for (unsigned int index_point = 0; index_point < points->num_points; index_point++) {
			if (!(points->points[index_point].z == 0.0f)) {
				return -8;
			}
		}

		points_destroy(points);

	}

	return 0;

}