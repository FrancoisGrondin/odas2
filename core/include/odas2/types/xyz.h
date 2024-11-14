#ifndef __XYZ_H
#define __XYZ_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xyz_t {

    float x;
    float y;
    float z;

} xyz_t;

xyz_t xyz_cst(const float x, const float y, const float z);

xyz_t xyz_unit(const xyz_t op);

float xyz_mag(const xyz_t op);

float xyz_l2(const xyz_t op);

float xyz_dot(const xyz_t op1, const xyz_t op2);

xyz_t xyz_vec(const xyz_t op1, const xyz_t op2);

xyz_t xyz_add(const xyz_t op1, const xyz_t op2);

xyz_t xyz_sub(const xyz_t op1, const xyz_t op2);

xyz_t xyz_scale(const xyz_t op, const float scale);

void xyz_printf(const xyz_t op);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __XYZ_H
