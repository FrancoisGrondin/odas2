#include <systems/enhancement.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

enhancement_t * enhancement_construct(const unsigned int num_channels, const unsigned int num_bins) {

    enhancement_t * obj = (enhancement_t *) malloc(sizeof(enhancement_t));

    obj->num_channels = num_channels;
    obj->num_bins = num_bins;

    if ((num_bins != num_dims_in) && (num_bins != num_dims_out)) {
        printf("Invalid number of bins for available neural network.\n");
        exit(EXIT_FAILURE);
    }

    obj->beta = beta;
    obj->rnn = ugru_construct(&rnn_params);
    obj->fc = linear_construct(&fc_params);

    obj->spec = (float *) calloc(num_bins, sizeof(float));
    obj->mask = (float *) calloc(num_bins, sizeof(float));

    obj->x = tensor_construct(1, 1, num_channels, num_dims_in);
    obj->h = tensor_construct(1, 1, num_channels, num_dims_hidden);
    obj->y = tensor_construct(1, 1, num_channels, num_dims_out);
    obj->m = tensor_construct(1, 1, num_channels, num_dims_out);

    return obj;

}

void enhancement_destroy(enhancement_t * obj) {

    ugru_destroy(obj->rnn);
    linear_destroy(obj->fc);

    free((void *) obj->spec);
    free((void *) obj->mask);

    tensor_destroy(obj->x);
    tensor_destroy(obj->h);
    tensor_destroy(obj->y);
    tensor_destroy(obj->m);

    free(obj);

}

int enhancement_process(enhancement_t * obj, const freqs_t * freqs, masks_t * masks) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
            // Load in x
        }
    }

    ugru_forward(obj->rnn, obj->x, obj->h, obj->h);
    linear_forward(obj->fc, obj->h, obj->y);
    fcns_sigmoid(obj->y, obj->m);

    return 0;

}

static const unsigned int num_dims_in = 257;
static const unsigned int num_dims_hidden = 128;
static const unsigned int num_dims_out = 257;

static const float beta = 0.25f;

static const ugru_params rnn_params = {

    .num_dims_in = num_dims_in,
    .num_dims_out = num_dims_hidden,
    .W_ih = (float []) { +0.0 },
    .W_hh = (float []) { +0.0 },
    .b_ih = (float []) { +0.0 },
    .b_hh = (float []) { +0.0 }

};

static const linear_params fc_params = {

    .num_dims_in = num_dims_hidden,
    .num_dims_out = num_dims_out,
    .W = (float []) { +0.0 },
    .b = (float []) { +0.0 }

};