#include "test_cplx.h"

int test_cplx(void) {

    const float eps = 0.001f;

    {

        float real = +1.0f;
        float imag = -2.0f;
        float rtn_real = +1.0f;
        float rtn_imag = -2.0f;

        cplx_t rtn = cplx_cst(real, imag);

        if (!((fabsf(rtn.real - rtn_real) < eps) && (fabsf(rtn.imag - rtn_imag) < eps))) {
            return -1;
        }

    }

    {

        float real1 = +1.0f;
        float imag1 = -2.0f;
        float real2 = +3.0f;
        float imag2 = -4.0f;
        float rtn_real = +4.0f;
        float rtn_imag = -6.0f;

        cplx_t rtn = cplx_add(cplx_cst(real1, imag1), cplx_cst(real2, imag2));

        if (!((fabsf(rtn.real - rtn_real) < eps) && (fabsf(rtn.imag - rtn_imag) < eps))) {
            return -2;
        }

    }

    {

        float real1 = +1.0f;
        float imag1 = -2.0f;
        float real2 = +3.0f;
        float imag2 = -4.0f;
        float rtn_real = -2.0f;
        float rtn_imag = +2.0f;

        cplx_t rtn = cplx_sub(cplx_cst(real1, imag1), cplx_cst(real2, imag2));

        if (!((fabsf(rtn.real - rtn_real) < eps) && (fabsf(rtn.imag - rtn_imag) < eps))) {
            return -3;
        }

    }

    {

        float real1 = +1.0f;
        float imag1 = -2.0f;
        float real2 = +3.0f;
        float imag2 = -4.0f;
        float rtn_real = -5.0f;
        float rtn_imag = -10.0f;

        cplx_t rtn = cplx_mul(cplx_cst(real1, imag1), cplx_cst(real2, imag2));

        if (!((fabsf(rtn.real - rtn_real) < eps) && (fabsf(rtn.imag - rtn_imag) < eps))) {
            return -4;
        }

    }

    {

        float real1 = +1.0f;
        float imag1 = -2.0f;
        float real2 = +3.0f;
        float imag2 = -4.0f;
        float rtn_real = +0.44f;
        float rtn_imag = -0.08f;

        cplx_t rtn = cplx_div(cplx_cst(real1, imag1), cplx_cst(real2, imag2));

        if (!((fabsf(rtn.real - rtn_real) < eps) && (fabsf(rtn.imag - rtn_imag) < eps))) {
            return -5;
        }

    }

    {

        float real = +1.0f;
        float imag = -2.0f;
        float rtn_real = +1.0f;
        float rtn_imag = +2.0f;

        cplx_t rtn = cplx_conj(cplx_cst(real, imag));

        if (!((fabsf(rtn.real - rtn_real) < eps) && (fabsf(rtn.imag - rtn_imag) < eps))) {
            return -6;
        }        

    }

    {

        float real = +3.0f;
        float imag = -4.0f;
        float rtn_real = +0.6f;
        float rtn_imag = -0.8f;

        cplx_t rtn = cplx_unit(cplx_cst(real, imag));

        if (!((fabsf(rtn.real - rtn_real) < eps) && (fabsf(rtn.imag - rtn_imag) < eps))) {
            return -7;
        }

    } 

    return 0;

}