#ifndef __ENHANCEMENT_H
#define __ENHANCEMENT_H

#include "../dnns/functionals.h"
#include "../dnns/layers.h"
#include "../dnns/tensors.h"

#include "../signals/freqs.h"
#include "../signals/masks.h"

typedef struct enhancement_t {

    unsigned int num_channels;
    unsigned int num_bins;

    float beta;
    ugru * rnn;
    linear * fc;

    float * spec;
    float * mask;

    tensor * x;
    tensor * h;
    tensor * y;
    tensor * m;    

} enhancement_t;

enhancement_t * enhancement_construct(const unsigned int num_channels, const unsigned int num_bins);

void enhancement_destroy(enhancement_t * obj);

int enhancement_process(enhancement_t * obj, const freqs_t * freqs, masks_t * masks);

static const unsigned int num_dims_in;
static const unsigned int num_dims_hidden;
static const unsigned int num_dims_out;
static const float beta;
static const ugru_params rnn_params;
static const linear_params fc_params;

#endif // __ENHANCEMENT_H