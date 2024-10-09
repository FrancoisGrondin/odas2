#ifndef __CPLX_H
#define __CPLX_H

typedef struct cplx_t {

    float real;
    float imag;

} cplx_t;

cplx_t cplx_cst(const float real, const float imag);

cplx_t cplx_add(const cplx_t op1, const cplx_t op2);

cplx_t cplx_sub(const cplx_t op1, const cplx_t op2);

cplx_t cplx_mul(const cplx_t op1, const cplx_t op2);

cplx_t cplx_div(const cplx_t op1, const cplx_t op2);

cplx_t cplx_conj(const cplx_t op);

cplx_t cplx_unit(const cplx_t op);

float cplx_l2(const cplx_t op);

#endif // __CPLX_H