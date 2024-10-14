#include <utils/fft.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

static void fft_compute_forward(fft_t * obj);
static void fft_compute_reverse(fft_t * obj);
static cplx_t * fft_twiddle(const unsigned int num_samples);
static cplx_t * fft_coefficients(const unsigned int num_samples);
static unsigned int * fft_revbits(const unsigned int num_samples);

fft_t * fft_construct(const unsigned int num_samples) {

    fft_t * obj = (fft_t *) malloc(sizeof(fft_t));

    if (ceilf(log2f(num_samples)) != floorf(log2f(num_samples))) {
        return NULL;
    }

    obj->num_samples = num_samples;
    obj->num_levels = (unsigned int) (floorf(log2f(num_samples/2)));

    obj->twiddle = fft_twiddle(num_samples/2);
    obj->coefficients = fft_coefficients(num_samples);
    obj->revbits = fft_revbits(num_samples/2);

    obj->working = (cplx_t *) calloc(sizeof(cplx_t), (obj->num_samples/2));
    obj->reverse = (cplx_t *) calloc(sizeof(cplx_t), (obj->num_samples/2));

    return obj;

}

void fft_destroy(fft_t * obj) {

    free(obj->twiddle);
    free(obj->coefficients);
    free(obj->revbits);

    free(obj->working);
    free(obj->reverse);

    free(obj);

}

void fft_rfft(fft_t * obj, const float * in, cplx_t * out) {

    // z[n] = x_e[n] + j x_o[n]
    for (unsigned int index_sample = 0; index_sample < (obj->num_samples/2); index_sample++) {

        obj->working[index_sample].real = in[index_sample * 2 + 0];
        obj->working[index_sample].imag = in[index_sample * 2 + 1];

    }

    // Z[k] = fft_(N/2)(z[n])
    fft_compute_forward(obj);

    // Compute X[k] for k = 1, 2, ..., N/2-1
    for (int index_sample = 1; index_sample < (obj->num_samples/2); index_sample++) {

        // W[k]
        cplx_t Wk = obj->coefficients[index_sample];

        // Z[k]
        cplx_t Zk = obj->working[index_sample];

        // Z[N/2-k]
        cplx_t ZN2k = obj->working[obj->num_samples/2 - index_sample];

        // Xe[k] = 0.5 * (Z[k] + Z^*[N/2-k])
        cplx_t Xek = cplx_mul(cplx_cst(0.5f, 0.0f), cplx_add(Zk, cplx_conj(ZN2k)));

        // Xo[k] = -0.5j (Z[k] - Z^*[N/2-k])
        cplx_t Xok = cplx_mul(cplx_cst(0.0f, -0.5f), cplx_sub(Zk, cplx_conj(ZN2k)));

        // X[k] = Xe[k] + Xo[k] * W[k]
        out[index_sample] = cplx_add(Xek, cplx_mul(Xok, Wk));

    }

    // Compute X[0] and X[N/2]

    // Z[0]
    cplx_t Z0 = obj->working[0];

    // Z[N/2] = Z[0] (Periodicity)
    cplx_t ZN2 = obj->working[0];

    // Xe[0] = 0.5 * (Z[0] + Z^*[N/2])
    cplx_t Xe0 = cplx_mul(cplx_cst(0.5f, 0.0f), cplx_add(Z0, cplx_conj(ZN2)));
    // Xe[N/2] = 0.5 * (Z[N/2] + Z^*[0])
    cplx_t XeN2 = cplx_mul(cplx_cst(0.5f, 0.0f), cplx_add(ZN2, cplx_conj(Z0)));

    // Xo[0] = -0.5j * (Z[0] - Z^*[N/2])
    cplx_t Xo0 = cplx_mul(cplx_cst(0.0f, -0.5f), cplx_sub(Z0, cplx_conj(ZN2)));
    // Xo[N/2] = -0.5j * (Z[N/2] - Z^*[0])
    cplx_t XoN2 = cplx_mul(cplx_cst(0.0f, -0.5f), cplx_sub(ZN2, cplx_conj(Z0)));

    // X[0] = Xe[0] + Xo[0]
    out[0] = cplx_add(Xe0, Xo0);

    // X[N/2] = Xe[N/2] - Xo[N/2]
    out[obj->num_samples/2] = cplx_sub(XeN2, XoN2);

}

void fft_irfft(fft_t * obj, const cplx_t * in, float * out) {

    // Compute Z[k] for k = 0, 1, 2, ... N/2-1
    for (int index_sample = 0; index_sample < (obj->num_samples/2); index_sample++) {

        // W[k]
        cplx_t Wk = obj->coefficients[index_sample];

        // X[k]
        cplx_t Xk = in[index_sample];

        // X[N/2-k]
        cplx_t XN2k = in[obj->num_samples/2 - index_sample];

        // Xe[k] = 0.5 * (X[k] + X^*[N/2-k])
        cplx_t Xek = cplx_mul(cplx_cst(0.5f, 0.0f), cplx_add(Xk, cplx_conj(XN2k)));

        // Xo[k] = 0.5 * (X[k] - X^*[N/2-k]) * W^*[k]
        cplx_t Xok = cplx_mul(cplx_mul(cplx_cst(0.5f, 0.0f), cplx_conj(Wk)), cplx_sub(Xk, cplx_conj(XN2k)));

        // Z[k] = Xe[k] + jXo[k]
        obj->working[index_sample] = cplx_add(Xek, cplx_mul(cplx_cst(0.0f, 1.0f), Xok));

    }

    // z[n] = ifft_(N/2)(Z[k])
    fft_compute_reverse(obj);

    // Compute x[n]
    for (int index_sample = 0; index_sample < (obj->num_samples/2); index_sample++) {

        // x[2n] = Re{z[n]}
        out[index_sample * 2 + 0] = obj->working[index_sample].real;

        // x[2n+1] = Im{z[n]}
        out[index_sample * 2 + 1] = obj->working[index_sample].imag;

    }

}

static void fft_compute_forward(fft_t * obj) {

    // Samples in bit reverse order
    for (unsigned int index_sample = 0; index_sample < (obj->num_samples/2); index_sample++) {

        // Copy with reverse bit order
        obj->reverse[obj->revbits[index_sample]] = obj->working[index_sample];

    }

    // Copy back to working array
    memcpy(obj->working, obj->reverse, sizeof(cplx_t) * (obj->num_samples/2));
    
    // Number of groups starts at N/2, and will reduce
    // to N/4, N/8, ..., 1 at each next level
    // For example, with N = 256, we have:
    // {128, 64, 32, 16, 8, 4, 2, 1}
    unsigned int num_groups = obj->num_samples / 4;

    // Number of butterflies per group starts at 1, and
    // will increase to 2, 4, ..., N/2
    // For example, with N = 256, we have:
    // {1, 2, 4, 8, 16, 32, 64, 128}
    unsigned int num_butterflies = 1;

    // Loop for each level in the FFT
    for (unsigned int index_level = 0; index_level < obj->num_levels; index_level++) {

        // Loop in each group of butterflies
        for (unsigned int index_group = 0; index_group < num_groups; index_group++) {

            // In each group, loop for each butterfly
            for (unsigned int index_butterfly = 0; index_butterfly < num_butterflies; index_butterfly++) {

                // Butterfly has the following architecture:
                //
                // x1 +-----+-----+ y1
                //           \   /
                //            \ /
                //             X
                //            / \
                //       T   /   \
                // x2 +-----+-----+ y2
                //            -1

                // Get twiddle factor value
                cplx_t twiddle = obj->twiddle[index_butterfly * num_groups];

                // Get input samples (x1 and x2 from butterfly)
                cplx_t x1 = obj->working[index_group * num_butterflies * 2 + index_butterfly];
                cplx_t x2 = obj->working[index_group * num_butterflies * 2 + index_butterfly + num_butterflies];

                // Compute output values (y1 and y2 from butterfly)
                cplx_t y1 = cplx_add(x1, cplx_mul(x2, twiddle));
                cplx_t y2 = cplx_sub(x1, cplx_mul(x2, twiddle));

                // Save results
                obj->working[index_group * num_butterflies * 2 + index_butterfly] = y1;
                obj->working[index_group * num_butterflies * 2 + index_butterfly + num_butterflies] = y2;

            }

        }

        // Update number of groups and butterflies for next iteration
        num_groups /= 2;
        num_butterflies *= 2;

    }

}

static void fft_compute_reverse(fft_t * obj) {

    // An inverse FFT can be obtained using a FFT as follows:
    //
    // IFFT(X) = (1/N) * (FFT(X^*))^*

    // Compute complex conjugate
    for (int index_sample = 0; index_sample < (obj->num_samples/2); index_sample++) {
        obj->working[index_sample] = cplx_conj(obj->working[index_sample]);
    }

    // Compute FFT
    fft_compute_forward(obj);

    // Compute complex conjugate and scale
    for (int index_sample = 0; index_sample < (obj->num_samples/2); index_sample++) {
        obj->working[index_sample] = cplx_mul(cplx_cst(1.0f/(obj->num_samples/2), 0.0f), cplx_conj(obj->working[index_sample]));
    }

}

static cplx_t * fft_twiddle(const unsigned int num_samples) {

    cplx_t * rtn = (cplx_t *) malloc(sizeof(cplx_t) * num_samples / 2);

    // Wn = exp(-j * 2 * pi * n / N)
    unsigned int N = num_samples;
    for (unsigned int n = 0; n < (N/2); n++) {
        rtn[n] = cplx_cst(cosf(-2.0 * M_PI * n / N), sinf(-2.0f * M_PI * n / N));
    }

    return rtn;

}

static cplx_t * fft_coefficients(const unsigned int num_samples) {

    cplx_t * rtn = (cplx_t *) malloc(sizeof(cplx_t) * num_samples);

    // Wn = exp(-j * 2 * pi * n / N)
    unsigned int N = num_samples;
    for (unsigned int n = 0; n < (N/2); n++) {
        rtn[n] = cplx_cst(cosf(-2.0 * M_PI * n / N), sinf(-2.0f * M_PI * n / N));
    }

    return rtn;
    
}

static unsigned int * fft_revbits(const unsigned int num_samples) {

    unsigned int * rtn = (unsigned int *) malloc(sizeof(unsigned int) * num_samples);

    unsigned int num_bits = ceilf(log2f(num_samples));

    for (unsigned int n = 0; n < num_samples; n++) {

        unsigned int nrev = 0;

        for (unsigned int shift = 0; shift < 16; shift++) {
            nrev += (n & (0x00000001 << shift)) << (31 - 2 * shift);
            nrev += (n & (0x00010000 << shift)) >> (1 + 2 * shift);
        }

        rtn[n] = nrev >> (32 - num_bits);

    }

    return rtn;

}