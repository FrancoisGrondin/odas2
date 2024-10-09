#ifndef __LINALG_H
#define __LINALG_H

#include "../types/cplx.h"

typedef struct rmat_t {

    unsigned int num_rows;
    unsigned int num_cols;
    float * elements;

} rmat_t;

typedef struct cmat_t {

    unsigned int num_rows;
    unsigned int num_cols;
    cplx_t * elements;

} cmat_t;

typedef struct smat_t {

    unsigned int num_rowscols;
    float * diag;
    float * utri;

} smat_t;

typedef struct hmat_t {

    unsigned int num_rowscols;
    float * diag;
    cplx_t * utri;

} hmat_t;

rmat_t * rmat_construct(const unsigned int num_rows, const unsigned int num_cols);

void rmat_destroy(rmat_t * obj);

int rmat_cst(rmat_t * obj, const float * elements);

int rmat_add(rmat_t * dst, const rmat_t * op1, const rmat_t * op2);

int rmat_sub(rmat_t * dst, const rmat_t * op1, const rmat_t * op2);

int rmat_mul(rmat_t * dst, const rmat_t * op1, const rmat_t * op2);

int rmat_had(rmat_t * dst, const rmat_t * op1, const rmat_t * op2);

int rmat_transpose(rmat_t * dst, const rmat_t * op);

int rmat_scale(rmat_t * dst, const rmat_t * op1, const float op2);

int rmat_sum(float * dst, const rmat_t * op1);

void rmat_printf(const rmat_t * obj);

cmat_t * cmat_construct(const unsigned int num_rows, const unsigned int num_cols);

void cmat_destroy(cmat_t * obj);

int cmat_cst(cmat_t * obj, cplx_t * elements);

int cmat_add(cmat_t * dst, const cmat_t * op1, const cmat_t * op2);

int cmat_sub(cmat_t * dst, const cmat_t * op1, const cmat_t * op2);

int cmat_mul(cmat_t * dst, const cmat_t * op1, const cmat_t * op2);

int cmat_had(cmat_t * dst, const cmat_t * op1, const cmat_t * op2);

int cmat_htranspose(cmat_t * dst, const cmat_t * op);

int cmat_scale(cmat_t * dst, const cmat_t * op1, const cplx_t op2);

int cmat_sum(cplx_t * dst, const cmat_t * op1);

void cmat_printf(const cmat_t * obj);

smat_t * smat_construct(const unsigned int num_rowscols);

void smat_destroy(smat_t * obj);

int smat_cst(smat_t * obj, const float * diag, const float * utri);

int smat_eye(smat_t * obj);

int smat_add(smat_t * dst, const smat_t * op1, const smat_t * op2);

int smat_sub(smat_t * dst, const smat_t * op1, const smat_t * op2);

int smat_had(smat_t * dst, const smat_t * op1, const smat_t * op2);

int smat_scale(smat_t * dst, const smat_t * op1, const float op2);

int smat_rank1(smat_t * dst, const rmat_t * op);

int smat_sum(float * dst, const smat_t * op1);

void smat_printf(const smat_t * obj);

hmat_t * hmat_construct(const unsigned int num_rowscols);

void hmat_destroy(hmat_t * obj);

int hmat_cst(hmat_t * obj, const float * diag, const cplx_t * utri);

int hmat_eye(hmat_t * obj);

int hmat_add(hmat_t * dst, const hmat_t * op1, const hmat_t * op2);

int hmat_sub(hmat_t * dst, const hmat_t * op1, const hmat_t * op2);

int hmat_had(hmat_t * dst, const hmat_t * op1, const hmat_t * op2);

int hmat_scale(hmat_t * dst, const hmat_t * op1, const float op2);

int hmat_rank1(hmat_t * dst, const cmat_t * op);

int hmat_sum(float * dst, const hmat_t * op1);

void hmat_printf(const hmat_t * obj);

int linalg_smat2rmat(rmat_t * dst, smat_t * src);

int linalg_rmat2smat(smat_t * dst, rmat_t * src);

int linalg_hmat2cmat(cmat_t * dst, hmat_t * src);

int linalg_cmat2hmat(hmat_t * dst, cmat_t * src);

#endif // __LINALG_H