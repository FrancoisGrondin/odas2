#ifndef __DECOMPOSE_H
#define __DECOMPOSE_H

#include "linalg.h"
#include "../types/cplx.h"

typedef struct choleskyreal_t {

    unsigned int num_rowscols;
    float * A;
    float * L;
    float * Linv;
    float * Ainv;

} choleskyreal_t;

typedef struct choleskycplx_t {

    unsigned int num_rowscols;
    cplx_t * A;
    cplx_t * L;
    cplx_t * Linv;
    cplx_t * Ainv;

} choleskycplx_t;

choleskyreal_t * choleskyreal_construct(const unsigned int num_rowscols);

void choleskyreal_destroy(choleskyreal_t * obj);

int choleskyreal_process(choleskyreal_t * obj, const float * in_diag, const float * in_triu, float * out_diag, float * out_triu);

choleskycplx_t * choleskycplx_construct(const unsigned int num_rowscols);

void choleskycplx_destroy(choleskycplx_t * obj);

int choleskycplx_process(choleskycplx_t * obj, const float * in_diag, const cplx_t * in_triu, float * out_diag, cplx_t * out_triu);

#endif // __DECOMPOSE_H