#include "test_xyz.h"

int test_xyz(void) {

    const float eps = 0.001f;

    {

        float x = +1.0f;
        float y = -2.0f;
        float z = +3.0f;
        float rtn_x = +1.0f;
        float rtn_y = -2.0f;
        float rtn_z = +3.0f;        

        xyz_t rtn = xyz_cst(x, y, z);

        if (!((fabsf(rtn.x - rtn_x) < eps) && (fabsf(rtn.y - rtn_y) < eps) && (fabsf(rtn.z - rtn_z) < eps))) {
            return -1;
        }

    }

    {

        float x = +1.0f;
        float y = -2.0f;
        float z = +3.0f;
        float rtn_x = +0.267261242f;
        float rtn_y = -0.534522484f;
        float rtn_z = +0.801783726f;

        xyz_t rtn = xyz_unit(xyz_cst(x, y, z));

        if (!((fabsf(rtn.x - rtn_x) < eps) && (fabsf(rtn.y - rtn_y) < eps) && (fabsf(rtn.z - rtn_z) < eps))) {
            return -2;
        }

    }

    {

        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float rtn_x = 0.0f;
        float rtn_y = 0.0f;
        float rtn_z = 0.0f;     

        xyz_t rtn = xyz_unit(xyz_cst(x, y, z));

        if (!((fabsf(rtn.x - rtn_x) < eps) && (fabsf(rtn.y - rtn_y) < eps) && (fabsf(rtn.z - rtn_z) < eps))) {
            return -3;
        }

    }

    {

        float x = +1.0f;
        float y = -2.0f;
        float z = +3.0f;
        float rtn_mag = 3.741657387f;

        float rtn = xyz_mag(xyz_cst(x, y, z));

        if (!(fabsf(rtn - rtn_mag) < eps)) {
            return -4;
        }

    }

    {

        float x1 = +1.0f;
        float y1 = -2.0f;
        float z1 = +3.0f;
        float x2 = -3.0f;
        float y2 = +1.0f;
        float z2 = -1.0f;
        float rtn_dot = -8.0f;

        float rtn = xyz_dot(xyz_cst(x1, y1, z1), xyz_cst(x2, y2, z2));

        if (!(fabsf(rtn - rtn_dot) < eps)) {
            return -5;
        }

    }

    {

        float x1 = +1.0f;
        float y1 = -2.0f;
        float z1 = +3.0f;
        float x2 = -3.0f;
        float y2 = +1.0f;
        float z2 = -1.0f;
        float rtn_x = -1.0f;
        float rtn_y = -8.0f;
        float rtn_z = -5.0f;

        xyz_t rtn = xyz_vec(xyz_cst(x1, y1, z1), xyz_cst(x2, y2, z2));

        if (!((fabsf(rtn.x - rtn_x) < eps) && (fabsf(rtn.y - rtn_y) < eps) && (fabsf(rtn.z - rtn_z) < eps))) {
            return -6;
        }

    }

    {

        float x1 = +1.0f;
        float y1 = -2.0f;
        float z1 = +3.0f;
        float x2 = -3.0f;
        float y2 = +1.0f;
        float z2 = -1.0f;
        float rtn_x = -2.0f;
        float rtn_y = -1.0f;
        float rtn_z = +2.0f;

        xyz_t rtn = xyz_add(xyz_cst(x1, y1, z1), xyz_cst(x2, y2, z2));

        if (!((fabsf(rtn.x - rtn_x) < eps) && (fabsf(rtn.y - rtn_y) < eps) && (fabsf(rtn.z - rtn_z) < eps))) {
            return -7;
        }

    }

    {

        float x1 = +1.0f;
        float y1 = -2.0f;
        float z1 = +3.0f;
        float x2 = -3.0f;
        float y2 = +1.0f;
        float z2 = -1.0f;
        float rtn_x = +4.0f;
        float rtn_y = -3.0f;
        float rtn_z = +4.0f;

        xyz_t rtn = xyz_sub(xyz_cst(x1, y1, z1), xyz_cst(x2, y2, z2));

        if (!((fabsf(rtn.x - rtn_x) < eps) && (fabsf(rtn.y - rtn_y) < eps) && (fabsf(rtn.z - rtn_z) < eps))) {
            return -8;
        }

    }    

    return 0;

}