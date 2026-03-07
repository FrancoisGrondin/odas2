#ifndef __DECOMPOSE_H
#define __DECOMPOSE_H

#include "linalg.h"
#include "../types/cplx.h"

typedef struct invpds_t {

    unsigned int num_rowscols;
    float * A;
    float * L;
    float * Linv;
    float * Ainv;

} invpds_t;

typedef struct invpdh_t {

    unsigned int num_rowscols;
    cplx_t * A;
    cplx_t * L;
    cplx_t * Linv;
    cplx_t * Ainv;

} invpdh_t;

invpds_t * invpds_construct(const unsigned int num_rowscols);

void invpds_destroy(invpds_t * obj);

int invpds_process(invpds_t * obj, const float * in_diag, const float * in_triu, float * out_diag, float * out_triu);

invpdh_t * invpdh_construct(const unsigned int num_rowscols);

void invpdh_destroy(invpdh_t * obj);

int invpdh_process(invpdh_t * obj, const float * in_diag, const cplx_t * in_triu, float * out_diag, cplx_t * out_triu);

#endif // __DECOMPOSE_H