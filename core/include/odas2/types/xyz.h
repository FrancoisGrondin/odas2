#ifndef __XYZ_H
#define __XYZ_H

#include <math.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xyz_t {

    float x;
    float y;
    float z;

} xyz_t;

inline xyz_t xyz_cst(const float x, const float y, const float z) {

    xyz_t rtn;

    rtn.x = x;
    rtn.y = y;
    rtn.z = z;

    return rtn;

}

inline float xyz_mag(const xyz_t op) {

    float rtn;

    // (x^2 + y^2 + z^2)^0.5
    rtn = sqrtf(op.x * op.x + op.y * op.y + op.z * op.z);

    return rtn;

}

inline xyz_t xyz_unit(const xyz_t op) {

    xyz_t rtn;
    float mag;

    mag = xyz_mag(op);

    // x' = x / (x^2 + y^2 + z^2)^0.5
    // y' = y / (x^2 + y^2 + z^2)^0.5
    // z' = z / (x^2 + y^2 + z^2)^0.5
    // Added epsilon to avoid division by 0
    rtn.x = op.x / (mag + 1e-20);
    rtn.y = op.y / (mag + 1e-20);
    rtn.z = op.z / (mag + 1e-20);

    return rtn;

}

inline float xyz_l2(const xyz_t op) {

    float rtn;

    // (x^2 + y^2 + z^2)
    rtn = op.x * op.x + op.y * op.y + op.z * op.z;

    return rtn;

}

inline float xyz_dot(const xyz_t op1, const xyz_t op2) {

    float rtn;

    // Dot product
    // x1 * x2 + y1 * y2 + z1 * z2
    rtn = op1.x * op2.x + op1.y * op2.y + op1.z * op2.z;

    return rtn;

}

inline xyz_t xyz_vec(const xyz_t op1, const xyz_t op2) {

    xyz_t rtn;

    // Cross product
    // x' = (y1 * z2 - z1 * y2)
    // y' = (z1 * x2 - x1 * z2)
    // z' = (x1 * y2 - y1 * x2)
    rtn.x = op1.y * op2.z - op1.z * op2.y;
    rtn.y = op1.z * op2.x - op1.x * op2.z;
    rtn.z = op1.x * op2.y - op1.y * op2.x;

    return rtn;

}

inline xyz_t xyz_add(const xyz_t op1, const xyz_t op2) {

    xyz_t rtn;

    rtn.x = op1.x + op2.x;
    rtn.y = op1.y + op2.y;
    rtn.z = op1.z + op2.z;

    return rtn;

}

inline xyz_t xyz_sub(const xyz_t op1, const xyz_t op2) {

    xyz_t rtn;

    rtn.x = op1.x - op2.x;
    rtn.y = op1.y - op2.y;
    rtn.z = op1.z - op2.z;

    return rtn;

}

inline xyz_t xyz_scale(const xyz_t op, const float scale) {

    xyz_t rtn;

    rtn.x = op.x * scale;
    rtn.y = op.y * scale;
    rtn.z = op.z * scale;

    return rtn;

}

inline void xyz_printf(const xyz_t op) {

    printf("(%+1.3f, %+1.3f, %+1.3f)\n", op.x, op.y, op.z);

}

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __XYZ_H
