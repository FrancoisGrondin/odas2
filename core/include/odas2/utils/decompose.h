#ifndef __DECOMPOSE_H
#define __DECOMPOSE_H

#include "linalg.h"

typedef struct invpds_t {

    unsigned int num_rowscols;
    float * A;
    float * L;
    float * Linv;
    float * Ainv;

} invpds_t;

invpds_t * invpds_construct(const unsigned int num_rowscols);

void invpds_destroy(invpds_t * obj);

int invpds_process(invpds_t * obj, const float * in_diag, const float * in_triu, float * out_diag, float * out_triu);

#endif // __DECOMPOSE_H