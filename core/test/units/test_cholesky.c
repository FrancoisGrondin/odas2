#include "test_cholesky.h"

int test_cholesky(void) {

    const float eps = 0.001f;

    {

        const unsigned int num_diags = 3;
        const unsigned int num_trius = 3;

        const float in_diag[3] = { 4.0f, 37.0f, 98.0f };
        const float in_triu[3] = { 12.0f, -16.0f, -43.0f };

        float out_diag[3];
        float out_triu[3];

        const float rtn_diag[3] = { +49.361111f, +3.777777f, +0.111111f };
        const float rtn_triu[3] = { -13.555556f, +2.111111f, -0.555556f };

        choleskyreal_t * choleskyreal = choleskyreal_construct(num_diags);

        choleskyreal_process(choleskyreal, in_diag, in_triu, out_diag, out_triu);

        for (unsigned int index_diag = 0; index_diag < num_diags; index_diag++) {
            if (!(fabsf(out_diag[index_diag] - rtn_diag[index_diag]) < eps)) {
                return -1;
            }
        }

        for (unsigned int index_triu = 0; index_triu < num_trius; index_triu++) {
            if (!(fabsf(out_triu[index_triu] - rtn_triu[index_triu]) < eps)) {
                return -1;
            }            
        }

        choleskyreal_destroy(choleskyreal);

    }

    {

        const unsigned int num_diags = 3;
        const unsigned int num_trius = 3;

        const float in_diag[3] = { 16.0f, 44.0f, 84.0f };
        const cplx_t in_triu[3] = { { .real = -8.000f, .imag = -8.000f }, 
                                    { .real = +4.000f, .imag = +4.000f }, 
                                    { .real = +14.000f, .imag = +18.000f } };

        float out_diag[3];
        cplx_t out_triu[3];

        const float rtn_diag[3] = { +0.086155f, +0.035590f, +0.015625f };
        const cplx_t rtn_triu[3] = { { .real = +0.021701f, .imag = +0.017795f },
                                     { .real = -0.003906f, .imag = -0.011719f },
                                     { .real = -0.007813f, .imag = -0.007813f } };

        choleskycplx_t * choleskycplx = choleskycplx_construct(num_diags);

        choleskycplx_process(choleskycplx, in_diag, in_triu, out_diag, out_triu);

        for (unsigned int index_diag = 0; index_diag < num_diags; index_diag++) {
            if (!(fabsf(out_diag[index_diag] - rtn_diag[index_diag]) < eps)) {
                return -1;
            }
        }

        for (unsigned int index_triu = 0; index_triu < num_trius; index_triu++) {
            if (!((fabsf(out_triu[index_triu].real - rtn_triu[index_triu].real) < eps) &&
                  (fabsf(out_triu[index_triu].imag - rtn_triu[index_triu].imag) < eps))) {
                return -1;
            }            
        }

        choleskycplx_destroy(choleskycplx);

    }

    return 0;

}