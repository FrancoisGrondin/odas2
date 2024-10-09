#ifndef __LAYERS
#define __LAYERS

#include "tensors.h"

typedef struct linear_params {

    unsigned int num_dims_in;  // I
    unsigned int num_dims_out; // O

    float * W; // O x I
    float * b; // O x 1

} linear_params;

typedef struct linear {

    const linear_params * params;

} linear;

typedef struct ugru_params {

    unsigned int num_dims_in;  // I
    unsigned int num_dims_out; // O

    float * W_ih; // 3O x I
    float * W_hh; // 3O x O
    float * b_ih; // 3O x 1
    float * b_hh; // 3O x 1

} ugru_params;

typedef struct ugru {

    const ugru_params * params;

    float * h;

} ugru;

typedef struct ulstm_params {

    unsigned int num_dims_in;  // I
    unsigned int num_dims_out; // O

    float * W_ih; // 4O x I
    float * W_hh; // 4O x O
    float * b_ih; // 4O x 1
    float * b_hh; // 4O x 1

} ulstm_params;

typedef struct ulstm {

    const ulstm_params * params;

    float * h;
    float * c;

} ulstm;

linear * linear_construct(const linear_params * params);

void linear_destroy(linear * obj);

int linear_forward(const linear * obj, const tensor * in, tensor * out);

ugru * ugru_construct(const ugru_params * params);

void ugru_destroy(ugru * obj);

int ugru_forward(const ugru * obj, const tensor * in, const tensor * hidden_in, tensor * hidden_out);

ulstm * ulstm_construct(const ulstm_params * params);

void ulstm_destroy(ulstm * obj);

int ulstm_forward(const ulstm * obj, const tensor * in, const tensor * hidden_in, const tensor * cell_in, tensor * hidden_out, tensor * cell_out);

#endif // __LAYERS