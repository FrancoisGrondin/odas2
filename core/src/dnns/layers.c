#include <dnns/layers.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

linear * linear_construct(const linear_params * params) {

    linear * obj = (linear *) malloc(sizeof(linear));

    obj->params = params;

    return obj;

}

void linear_destroy(linear * obj) {

    free((void *) obj);

}

int linear_forward(const linear * obj, const tensor * in, tensor * out) {

    const unsigned int num_dims_batch = in->num_dims1 * in->num_dims2 * in->num_dims3;
    const unsigned int num_dims_in = obj->params->num_dims_in;
    const unsigned int num_dims_out = obj->params->num_dims_out;

    for (unsigned int index_dim_batch = 0; index_dim_batch < num_dims_batch; index_dim_batch++) {

        for (unsigned int index_dim_out = 0; index_dim_out < num_dims_out; index_dim_out++) {

            float b = obj->params->b[index_dim_out];
            float y = b;

            for (unsigned int index_dim_in = 0; index_dim_in < num_dims_in; index_dim_in++) {

                float w = obj->params->W[index_dim_out * num_dims_in + index_dim_in];
                float x = in->data[index_dim_batch * num_dims_in + index_dim_in];
                y += w * x;

            }

            out->data[index_dim_batch * num_dims_out + index_dim_out] = y;

        }

    }   

    return 0;

}

ugru * ugru_construct(const ugru_params * params) {

    ugru * obj = (ugru *) malloc(sizeof(ugru));

    obj->params = params;

    obj->h = (float *) calloc(sizeof(float), params->num_dims_out);

    return obj;

}

void ugru_destroy(ugru * obj) {

    free((void *) obj->h);

    free((void *) obj);

}

int ugru_forward(const ugru * obj, const tensor * in, const tensor * hidden_in, tensor * hidden_out) {

    const unsigned int num_dims_batch = in->num_dims1 * in->num_dims2 * in->num_dims3;
    const unsigned int num_dims_in = obj->params->num_dims_in;
    const unsigned int num_dims_out = obj->params->num_dims_out;

    // Parameters are saved:
    //
    // W_ih = [ W_ir | W_iz | W_in ]
    // W_hh = [ W_hr | W_hz | W_hn ]
    //
    // b_ih = [ b_ir | b_iz | b_in ]
    // b_hh = [ b_hr | b_hz | b_hn ]
    //
    // And inference goes this way:
    //
    // r_t = \sigma(W_{ir} x_t + b_{ir} + W_{hr} h_{(t-1)} + b_{hr})
    // z_t = \sigma(W_{iz} x_t + b_{iz} + W_{hz} h_{(t-1)} + b_{hz})
    // n_t = \tanh(W_{in} x_t + b_{in} + r_t \odot (W_{hn} h_{(t-1)} + b_{hn}))
    // h_t = (1 - z_t) \odot n_t + z_t \odot h_{(t-1)}

    float * Wm_ir = &(obj->params->W_ih[0 * num_dims_in * num_dims_out]);
    float * Wm_iz = &(obj->params->W_ih[1 * num_dims_in * num_dims_out]);
    float * Wm_in = &(obj->params->W_ih[2 * num_dims_in * num_dims_out]);
    float * Wm_hr = &(obj->params->W_hh[0 * num_dims_out * num_dims_out]);
    float * Wm_hz = &(obj->params->W_hh[1 * num_dims_out * num_dims_out]);
    float * Wm_hn = &(obj->params->W_hh[2 * num_dims_out * num_dims_out]);
    float * bv_ir = &(obj->params->b_ih[0 * num_dims_out]);
    float * bv_iz = &(obj->params->b_ih[1 * num_dims_out]);
    float * bv_in = &(obj->params->b_ih[2 * num_dims_out]);
    float * bv_hr = &(obj->params->b_hh[0 * num_dims_out]);
    float * bv_hz = &(obj->params->b_hh[1 * num_dims_out]);
    float * bv_hn = &(obj->params->b_hh[2 * num_dims_out]);

    for (unsigned int index_dim_batch = 0; index_dim_batch < num_dims_batch; index_dim_batch++) {

        float * xv = &(in->data[index_dim_batch * num_dims_in]);
        float * hv = &(hidden_in->data[index_dim_batch * num_dims_out]);

        for (unsigned int index_dim_out = 0; index_dim_out < num_dims_out; index_dim_out++) {

            float b_ir = bv_ir[index_dim_out];
            float b_hr = bv_hr[index_dim_out];

            float b_iz = bv_iz[index_dim_out];
            float b_hz = bv_hz[index_dim_out];

            float b_in = bv_in[index_dim_out];
            float b_hn = bv_hn[index_dim_out];

            float r_arg = b_ir + b_hr;
            float z_arg = b_iz + b_hz;
            float n_arg1 = b_in;
            float n_arg2 = b_hn;

            for (unsigned int index_dim_in = 0; index_dim_in < num_dims_in; index_dim_in++) {

                float W_ir = Wm_ir[index_dim_out * num_dims_in + index_dim_in];
                float W_iz = Wm_iz[index_dim_out * num_dims_in + index_dim_in];
                float W_in = Wm_in[index_dim_out * num_dims_in + index_dim_in];

                float x = xv[index_dim_in];

                r_arg += W_ir * x;
                z_arg += W_iz * x;
                n_arg1 += W_in * x;

            }

            for (unsigned int index_dim_hidden = 0; index_dim_hidden < num_dims_out; index_dim_hidden++) {

                float W_hr = Wm_hr[index_dim_out * num_dims_out + index_dim_hidden];
                float W_hz = Wm_hz[index_dim_out * num_dims_out + index_dim_hidden];
                float W_hn = Wm_hn[index_dim_out * num_dims_out + index_dim_hidden];

                float h = hv[index_dim_hidden];

                r_arg += W_hr * h;
                z_arg += W_hz * h;
                n_arg2 += W_hn * h;

            }

            float r = 1.0f / (1.0f + expf(-1.0f * r_arg));
            float z = 1.0f / (1.0f + expf(-1.0f * z_arg));
            float n = tanhf(n_arg1 + r * n_arg2);

            obj->h[index_dim_out] = (1.0f - z) * n + z * hv[index_dim_out];

        }

        memcpy(&(hidden_out->data[index_dim_batch * num_dims_out]), obj->h, sizeof(float) * num_dims_out);

    }   

    return 0;

}

ulstm * ulstm_construct(const ulstm_params * params) {

    ulstm * obj = (ulstm *) malloc(sizeof(ulstm));

    obj->params = params;

    obj->h = (float *) calloc(sizeof(float), params->num_dims_out);
    obj->c = (float *) calloc(sizeof(float), params->num_dims_out);

    return obj;

}

void ulstm_destroy(ulstm * obj) {

    free((void *) obj->h);
    free((void *) obj->c);

    free((void *) obj); 

}

int ulstm_forward(const ulstm * obj, const tensor * in, const tensor * hidden_in, const tensor * cell_in, tensor * hidden_out, tensor * cell_out) {

    const unsigned int num_dims_batch = in->num_dims1 * in->num_dims2 * in->num_dims3;
    const unsigned int num_dims_in = obj->params->num_dims_in;
    const unsigned int num_dims_out = obj->params->num_dims_out;

    // Parameters are saved:
    //
    // W_ih = [ W_ii | W_if | W_ig | W_io ]
    // W_hh = [ W_hi | W_hf | W_hg | W_ho ]
    //
    // b_ih = [ b_ii | b_if | b_ig | b_io ]
    // b_hh = [ b_hi | b_hf | b_hg | b_ho ]
    //
    // And inference goes this way:
    //
    // i_t = \sigma(W_{ii} x_t + b_{ii} + W_{hi} h_{(t-1)} + b_{hi})
    // f_t = \sigma(W_{if} x_t + b_{if} + W_{hf} h_{(t-1)} + b_{hf})
    // g_t = \tanh(W_{ig} x_t + b_{ig} + W_{hg} h_{(t-1)} + b_{hg})
    // o_t = \sigma(W_{io} x_t + b_{io} + W_{ho} h_{(t-1)} + b_{ho})
    // c_t = f_t \odot c_{(t-1)} + i_t \odot g_t
    // h_t = o_t \odot \tanh(c_t)

    float * Wm_ii = &(obj->params->W_ih[0 * num_dims_in * num_dims_out]);
    float * Wm_if = &(obj->params->W_ih[1 * num_dims_in * num_dims_out]);
    float * Wm_ig = &(obj->params->W_ih[2 * num_dims_in * num_dims_out]);
    float * Wm_io = &(obj->params->W_ih[3 * num_dims_in * num_dims_out]);
    float * Wm_hi = &(obj->params->W_hh[0 * num_dims_out * num_dims_out]);
    float * Wm_hf = &(obj->params->W_hh[1 * num_dims_out * num_dims_out]);
    float * Wm_hg = &(obj->params->W_hh[2 * num_dims_out * num_dims_out]);
    float * Wm_ho = &(obj->params->W_hh[3 * num_dims_out * num_dims_out]);
    float * bv_ii = &(obj->params->b_ih[0 * num_dims_out]);
    float * bv_if = &(obj->params->b_ih[1 * num_dims_out]);
    float * bv_ig = &(obj->params->b_ih[2 * num_dims_out]);
    float * bv_io = &(obj->params->b_ih[3 * num_dims_out]);
    float * bv_hi = &(obj->params->b_hh[0 * num_dims_out]);
    float * bv_hf = &(obj->params->b_hh[1 * num_dims_out]);
    float * bv_hg = &(obj->params->b_hh[2 * num_dims_out]);
    float * bv_ho = &(obj->params->b_hh[3 * num_dims_out]);    

    for (unsigned int index_dim_batch = 0; index_dim_batch < num_dims_batch; index_dim_batch++) {

        float * xv = &(in->data[index_dim_batch * num_dims_in]);
        float * hv = &(hidden_in->data[index_dim_batch * num_dims_out]);
        float * cv = &(cell_in->data[index_dim_batch * num_dims_out]);

        for (unsigned int index_dim_out = 0; index_dim_out < num_dims_out; index_dim_out++) {

            float b_ii = bv_ii[index_dim_out];
            float b_hi = bv_hi[index_dim_out];

            float b_if = bv_if[index_dim_out];
            float b_hf = bv_hf[index_dim_out];

            float b_ig = bv_ig[index_dim_out];
            float b_hg = bv_hg[index_dim_out];

            float b_io = bv_io[index_dim_out];
            float b_ho = bv_ho[index_dim_out];

            float i_arg = b_ii + b_hi;
            float f_arg = b_if + b_hf;
            float g_arg = b_ig + b_hg;
            float o_arg = b_io + b_ho;

            for (unsigned int index_dim_in = 0; index_dim_in < num_dims_in; index_dim_in++) {

                float W_ii = Wm_ii[index_dim_out * num_dims_in + index_dim_in];
                float W_if = Wm_if[index_dim_out * num_dims_in + index_dim_in];
                float W_ig = Wm_ig[index_dim_out * num_dims_in + index_dim_in];
                float W_io = Wm_io[index_dim_out * num_dims_in + index_dim_in];

                float x = xv[index_dim_in];

                i_arg += W_ii * x;
                f_arg += W_if * x;
                g_arg += W_ig * x;
                o_arg += W_io * x;

            }

            for (unsigned int index_dim_hidden = 0; index_dim_hidden < num_dims_out; index_dim_hidden++) {

                float W_hi = Wm_hi[index_dim_out * num_dims_out + index_dim_hidden];
                float W_hf = Wm_hf[index_dim_out * num_dims_out + index_dim_hidden];
                float W_hg = Wm_hg[index_dim_out * num_dims_out + index_dim_hidden];
                float W_ho = Wm_ho[index_dim_out * num_dims_out + index_dim_hidden];

                float h = hv[index_dim_hidden];

                i_arg += W_hi * h;
                f_arg += W_hf * h;
                g_arg += W_hg * h;
                o_arg += W_ho * h;

            }

            float i = 1.0f / (1.0f + expf(-1.0f * i_arg));
            float f = 1.0f / (1.0f + expf(-1.0f * f_arg));
            float g = tanhf(g_arg);
            float o = 1.0f / (1.0f + expf(-1.0f * o_arg));

            obj->c[index_dim_out] = f * cv[index_dim_out] + i * g;
            obj->h[index_dim_out] = o * tanhf(obj->c[index_dim_out]);

        }

        memcpy(&(hidden_out->data[index_dim_batch * num_dims_out]), obj->h, sizeof(float) * num_dims_out);
        memcpy(&(cell_out->data[index_dim_batch * num_dims_out]), obj->c, sizeof(float) * num_dims_out);

    }   

    return 0;

}