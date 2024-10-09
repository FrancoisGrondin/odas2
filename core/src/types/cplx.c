#include <types/cplx.h>

#include <math.h>

cplx_t cplx_cst(const float real, const float imag) {

    cplx_t rtn;

    // (a + jb)
    rtn.real = real;
    rtn.imag = imag;

    return rtn;

}

cplx_t cplx_add(const cplx_t op1, const cplx_t op2) {

    cplx_t rtn;

    // (a + jb) + (c + jd) = (a + c) + j(b + d)
    rtn.real = op1.real + op2.real;
    rtn.imag = op1.imag + op2.imag;

    return rtn;

}

cplx_t cplx_sub(const cplx_t op1, const cplx_t op2) {

    cplx_t rtn;

    // (a + jb) - (c + jd) = (a - c) + j(b - d)
    rtn.real = op1.real - op2.real;
    rtn.imag = op1.imag - op2.imag;

    return rtn;

}

cplx_t cplx_mul(const cplx_t op1, const cplx_t op2) {

    cplx_t rtn;

    // (a + jb) * (c + jd) = (ac - bd) + j(bc + ad)
    rtn.real = op1.real * op2.real - op1.imag * op2.imag;
    rtn.imag = op1.real * op2.imag + op2.real * op1.imag;

    return rtn;

}

cplx_t cplx_div(const cplx_t op1, const cplx_t op2) {

    cplx_t rtn;

    // (a + jb) / (c + jd) = (a + jb) * (c - jd) / [(c + jd) * (c - jd)]
    //                     = [(ac + bd) + j(bc - ad)] / [c^2 + d^2]
    //                     = [(ac + bd) / (c^2 + d^2)] + j[(bc - ad) / (c^2 + d^2)]
    rtn.real = (op1.real * op2.real + op1.imag * op2.imag) / (op2.real * op2.real + op2.imag * op2.imag);
    rtn.imag = (op1.imag * op2.real - op1.real * op2.imag) / (op2.real * op2.real + op2.imag * op2.imag);

    return rtn;

}

cplx_t cplx_conj(const cplx_t op) {

    cplx_t rtn;

    // (a + jb)' = a - jb
    rtn.real = op.real;
    rtn.imag = -1.0f * op.imag;

    return rtn;

}

cplx_t cplx_unit(const cplx_t op) {

    cplx_t rtn;
    const float eps = 1e-20;

    // (a + jb) / ((a^2 + b^2)^0.5 + eps) = a / ((a^2 + b^2)^0.5 + eps) + j b / ((a^2 + b^2)^0.5 + eps)
    float magnitude = sqrtf(op.real * op.real + op.imag * op.imag);
    rtn.real = op.real / (magnitude + eps);
    rtn.imag = op.imag / (magnitude + eps);

    return rtn;

}

float cplx_l2(const cplx_t op) {

    float rtn;

    // |a + jb|^2 = a^2 + b^2
    rtn = op.real * op.real + op.imag * op.imag;

    return rtn;

}