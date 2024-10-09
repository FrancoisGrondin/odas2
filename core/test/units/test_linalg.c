#include "test_linalg.h"

int test_linalg(void) {

    const float eps = 0.001f;

    {

        unsigned int num_rows = 4;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;
        
        float elements1[8] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f, +7.0f, +8.0f };
        float elements2[8] = { +10.0f, +20.0f, +30.0f, +40.0f, +50.0f, +60.0f, +70.0f, +80.0f };
        float rtn_elements[8] = { +11.0f, +22.0f, +33.0f, +44.0f, +55.0f, +66.0f, +77.0f, +88.0f };

        rmat_t * matrix1 = rmat_construct(num_rows, num_cols);
        rmat_t * matrix2 = rmat_construct(num_rows, num_cols);
        rmat_t * rtn = rmat_construct(num_rows, num_cols);

        rmat_cst(matrix1, elements1);
        rmat_cst(matrix2, elements2);

        rmat_add(rtn, matrix1, matrix2);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!(fabsf(rtn->elements[index_element] - rtn_elements[index_element]) < eps)) {
                return -1;
            } 
        }

        rmat_destroy(matrix1);
        rmat_destroy(matrix2);
        rmat_destroy(rtn);

    }

    {

        unsigned int num_rows = 4;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;
        
        float elements1[8] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f, +7.0f, +8.0f };
        float elements2[8] = { +10.0f, +20.0f, +30.0f, +40.0f, +50.0f, +60.0f, +70.0f, +80.0f };
        float rtn_elements[8] = { -9.0f, -18.0f, -27.0f, -36.0f, -45.0f, -54.0f, -63.0f, -72.0f };

        rmat_t * matrix1 = rmat_construct(num_rows, num_cols);
        rmat_t * matrix2 = rmat_construct(num_rows, num_cols);
        rmat_t * rtn = rmat_construct(num_rows, num_cols);

        rmat_cst(matrix1, elements1);
        rmat_cst(matrix2, elements2);

        rmat_sub(rtn, matrix1, matrix2);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!(fabsf(rtn->elements[index_element] - rtn_elements[index_element]) < eps)) {
                return -2;
            } 
        }

        rmat_destroy(matrix1);
        rmat_destroy(matrix2);
        rmat_destroy(rtn);        

    }

    {

        unsigned int num_rows1 = 3;
        unsigned int num_cols1 = 4;
        unsigned int num_elements1 = num_rows1 * num_cols1;
        unsigned int num_rows2 = 4;
        unsigned int num_cols2 = 2;
        unsigned int num_elements2 = num_rows2 * num_cols2;
        unsigned int num_elements = num_rows1 * num_cols2;

        float elements1[12] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f, +7.0f, +8.0f, +9.0f, +10.0f, +11.0f, +12.0f };
        float elements2[8] = { +10.0f, +20.0f, +30.0f, +40.0f, +50.0f, +60.0f, +70.0f, +80.0f };
        float rtn_elements[6] = { +500.0f, +600.0f, +1140.0f, +1400.0f, +1780.0f, +2200.0f };

        rmat_t * matrix1 = rmat_construct(num_rows1, num_cols1);
        rmat_t * matrix2 = rmat_construct(num_rows2, num_cols2);
        rmat_t * rtn = rmat_construct(num_rows1, num_cols2);

        rmat_cst(matrix1, elements1);
        rmat_cst(matrix2, elements2);

        rmat_mul(rtn, matrix1, matrix2);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!(fabsf(rtn->elements[index_element] - rtn_elements[index_element]) < eps)) {
                return -3;
            }             
        }

        rmat_destroy(matrix1);
        rmat_destroy(matrix2);
        rmat_destroy(rtn);        

    }

    {

        unsigned int num_rows = 4;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;
        
        float elements1[8] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f, +7.0f, +8.0f };
        float elements2[8] = { +10.0f, +20.0f, +30.0f, +40.0f, +50.0f, +60.0f, +70.0f, +80.0f };
        float rtn_elements[8] = { +10.0f, +40.0f, +90.0f, +160.0f, +250.0f, +360.0f, +490.0f, +640.0f };

        rmat_t * matrix1 = rmat_construct(num_rows, num_cols);
        rmat_t * matrix2 = rmat_construct(num_rows, num_cols);
        rmat_t * rtn = rmat_construct(num_rows, num_cols);

        rmat_cst(matrix1, elements1);
        rmat_cst(matrix2, elements2);

        rmat_had(rtn, matrix1, matrix2);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!(fabsf(rtn->elements[index_element] - rtn_elements[index_element]) < eps)) {
                return -4;
            } 
        }

        rmat_destroy(matrix1);
        rmat_destroy(matrix2);
        rmat_destroy(rtn);        

    }

    {

        unsigned int num_rows = 4;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;

        float elements[8] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f, +7.0f, +8.0f };
        float rtn_elements[8] = { +1.0f, +3.0f, +5.0f, +7.0f, +2.0f, +4.0f, +6.0f, +8.0f };

        rmat_t * matrix = rmat_construct(num_rows, num_cols);
        rmat_t * rtn = rmat_construct(num_cols, num_rows);

        rmat_cst(matrix, elements);

        rmat_transpose(rtn, matrix);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!(fabsf(rtn->elements[index_element] - rtn_elements[index_element]) < eps)) {
                return -5;
            } 
        }

        rmat_destroy(matrix);
        rmat_destroy(rtn);

    }

    {

        unsigned int num_rows = 4;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;

        float elements[8] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f, +7.0f, +8.0f };
        float scale = 2.0f;
        float rtn_elements[8] = { +2.0f, +4.0f, +6.0f, +8.0f, +10.0f, +12.0f, +14.0f, +16.0f };

        rmat_t * matrix = rmat_construct(num_rows, num_cols);
        rmat_t * rtn = rmat_construct(num_rows, num_cols);

        rmat_cst(matrix, elements);

        rmat_scale(rtn, matrix, scale);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!(fabsf(rtn->elements[index_element] - rtn_elements[index_element]) < eps)) {
                return -6;
            } 
        }

        rmat_destroy(matrix);
        rmat_destroy(rtn);

    }

    {

        unsigned int num_rows = 4;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;

        float elements[8] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f, +7.0f, +8.0f };
        float rtn;
        float rtn_sum = 36.0f;

        rmat_t * matrix = rmat_construct(num_rows, num_cols);

        rmat_cst(matrix, elements);

        rmat_sum(&rtn, matrix);

        if (!(fabsf(rtn - rtn_sum) < eps)) {
            return -7;
        }

        rmat_destroy(matrix);

    }

    {

        unsigned int num_rows = 3;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;

        cplx_t elements1[6] = { { .real = +1.0f, .imag = +2.0f }, 
                                { .real = +3.0f, .imag = +4.0f }, 
                                { .real = +5.0f, .imag = +2.0f }, 
                                { .real = +7.0f, .imag = -2.0f }, 
                                { .real = -1.0f, .imag = +2.0f }, 
                                { .real = -2.0f, .imag = +1.0f } };

        cplx_t elements2[6] = { { .real = +0.0f, .imag = +1.0f }, 
                                { .real = +1.0f, .imag = +2.0f }, 
                                { .real = -2.0f, .imag = -1.0f }, 
                                { .real = +3.0f, .imag = +4.0f }, 
                                { .real = +2.0f, .imag = -2.0f }, 
                                { .real = +1.0f, .imag = -3.0f } };

        cplx_t rtn_elements[6] = { { .real =  +1.0f, .imag = +3.0f },
                                   { .real =  +4.0f, .imag = +6.0f },
                                   { .real =  +3.0f, .imag = +1.0f },
                                   { .real = +10.0f, .imag = +2.0f },
                                   { .real =  +1.0f, .imag = +0.0f },
                                   { .real =  -1.0f, .imag = -2.0f } };

        cmat_t * matrix1 = cmat_construct(num_rows, num_cols);
        cmat_t * matrix2 = cmat_construct(num_rows, num_cols);
        cmat_t * rtn = cmat_construct(num_rows, num_cols);

        cmat_cst(matrix1, elements1);
        cmat_cst(matrix2, elements2);

        cmat_add(rtn, matrix1, matrix2);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!((fabsf(rtn->elements[index_element].real - rtn_elements[index_element].real) < eps) &&
                  (fabsf(rtn->elements[index_element].imag - rtn_elements[index_element].imag) < eps))) {
                return -8;
            } 
        }    

        cmat_destroy(matrix1);
        cmat_destroy(matrix2);
        cmat_destroy(rtn);

    }

    {

        unsigned int num_rows = 3;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;

        cplx_t elements1[6] = { { .real = +1.0f, .imag = +2.0f }, 
                                { .real = +3.0f, .imag = +4.0f }, 
                                { .real = +5.0f, .imag = +2.0f }, 
                                { .real = +7.0f, .imag = -2.0f }, 
                                { .real = -1.0f, .imag = +2.0f }, 
                                { .real = -2.0f, .imag = +1.0f } };

        cplx_t elements2[6] = { { .real = +0.0f, .imag = +1.0f }, 
                                { .real = +1.0f, .imag = +2.0f }, 
                                { .real = -2.0f, .imag = -1.0f }, 
                                { .real = +3.0f, .imag = +4.0f }, 
                                { .real = +2.0f, .imag = -2.0f }, 
                                { .real = +1.0f, .imag = -3.0f } };

        cplx_t rtn_elements[6] = { { .real = +1.0f, .imag = +1.0f },
                                   { .real = +2.0f, .imag = +2.0f },
                                   { .real = +7.0f, .imag = +3.0f },
                                   { .real = +4.0f, .imag = -6.0f },
                                   { .real = -3.0f, .imag = +4.0f },
                                   { .real = -3.0f, .imag = +4.0f } };

        cmat_t * matrix1 = cmat_construct(num_rows, num_cols);
        cmat_t * matrix2 = cmat_construct(num_rows, num_cols);
        cmat_t * rtn = cmat_construct(num_rows, num_cols);

        cmat_cst(matrix1, elements1);
        cmat_cst(matrix2, elements2);

        cmat_sub(rtn, matrix1, matrix2);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!((fabsf(rtn->elements[index_element].real - rtn_elements[index_element].real) < eps) &&
                  (fabsf(rtn->elements[index_element].imag - rtn_elements[index_element].imag) < eps))) {
                return -9;
            } 
        }

        cmat_destroy(matrix1);
        cmat_destroy(matrix2);
        cmat_destroy(rtn);

    }

    {

        unsigned int num_rows1 = 3;
        unsigned int num_cols1 = 4;
        unsigned int num_elements1 = num_rows1 * num_cols1;
        unsigned int num_rows2 = 4;
        unsigned int num_cols2 = 2;
        unsigned int num_elements2 = num_rows2 * num_cols2;
        unsigned int num_elements = num_rows1 * num_cols2;

        cplx_t elements1[12] = { { .real = +1.0f, .imag = +2.0f }, 
                                 { .real = +3.0f, .imag = +4.0f }, 
                                 { .real = +5.0f, .imag = +2.0f }, 
                                 { .real = +7.0f, .imag = -2.0f }, 
                                 { .real = -1.0f, .imag = +2.0f }, 
                                 { .real = -2.0f, .imag = +1.0f },
                                 { .real = +0.0f, .imag = +1.0f }, 
                                 { .real = +1.0f, .imag = +2.0f }, 
                                 { .real = -2.0f, .imag = -1.0f }, 
                                 { .real = +3.0f, .imag = +4.0f }, 
                                 { .real = +2.0f, .imag = -2.0f }, 
                                 { .real = +1.0f, .imag = -3.0f } };

        cplx_t elements2[8] = { { .real = +0.0f, .imag = +2.0f },
                                { .real = +1.0f, .imag = +2.0f },
                                { .real = -2.0f, .imag = -3.0f },
                                { .real = -1.0f, .imag = +1.0f },
                                { .real = +3.0f, .imag = +0.0f },
                                { .real = +2.0f, .imag = +1.0f },
                                { .real = -3.0f, .imag = -3.0f },
                                { .real = +2.0f, .imag = +1.0f } };

        cplx_t rtn_elements[6] = { { .real = -10.0f, .imag = -24.0f },
                                   { .real = +14.0f, .imag = +15.0f },
                                   { .real =  +6.0f, .imag =  -4.0f },
                                   { .real =  -5.0f, .imag =  +4.0f },
                                   { .real =  +2.0f, .imag = -21.0f },
                                   { .real =  +4.0f, .imag = -13.0f } };

        cmat_t * matrix1 = cmat_construct(num_rows1, num_cols1);
        cmat_t * matrix2 = cmat_construct(num_rows2, num_cols2);
        cmat_t * rtn = cmat_construct(num_rows1, num_cols2);

        cmat_cst(matrix1, elements1);
        cmat_cst(matrix2, elements2);

        cmat_mul(rtn, matrix1, matrix2);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!((fabsf(rtn->elements[index_element].real - rtn_elements[index_element].real) < eps) &&
                  (fabsf(rtn->elements[index_element].imag - rtn_elements[index_element].imag) < eps))) {
                return -10;
            } 
        }    

        cmat_destroy(matrix1);
        cmat_destroy(matrix2);
        cmat_destroy(rtn);

    }

    {

        unsigned int num_rows = 3;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;

        cplx_t elements1[6] = { { .real = +2.0f, .imag = -2.0f },
                                { .real = -1.0f, .imag = +1.0f },
                                { .real = +1.0f, .imag = -2.0f },
                                { .real = +3.0f, .imag = +0.0f },
                                { .real = +0.0f, .imag = -1.0f },
                                { .real = +1.0f, .imag = +2.0f } };

        cplx_t elements2[6] = { { .real = +1.0f, .imag = -1.0f },
                                { .real = -2.0f, .imag = +2.0f },
                                { .real = +0.0f, .imag = +3.0f },
                                { .real = -3.0f, .imag = +1.0f },
                                { .real = +2.0f, .imag = +2.0f },
                                { .real = -2.0f, .imag = -1.0f } };

        cplx_t rtn_elements[6] = { { .real = +0.0f, .imag = -4.0f },
                                   { .real = +0.0f, .imag = -4.0f },
                                   { .real = +6.0f, .imag = +3.0f },
                                   { .real = -9.0f, .imag = +3.0f },
                                   { .real = +2.0f, .imag = -2.0f },
                                   { .real = +0.0f, .imag = -5.0f } };

        cmat_t * matrix1 = cmat_construct(num_rows, num_cols);
        cmat_t * matrix2 = cmat_construct(num_rows, num_cols);
        cmat_t * rtn = cmat_construct(num_rows, num_cols);

        cmat_cst(matrix1, elements1);
        cmat_cst(matrix2, elements2);

        cmat_had(rtn, matrix1, matrix2);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!((fabsf(rtn->elements[index_element].real - rtn_elements[index_element].real) < eps) &&
                  (fabsf(rtn->elements[index_element].imag - rtn_elements[index_element].imag) < eps))) {
                return -11;
            } 
        }    

        cmat_destroy(matrix1);
        cmat_destroy(matrix2);
        cmat_destroy(rtn);

    }

    {

        unsigned int num_rows = 3;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;

        cplx_t elements[6] = { { .real = +2.0f, .imag = -2.0f },
                               { .real = -1.0f, .imag = +1.0f },
                               { .real = +1.0f, .imag = -2.0f },
                               { .real = +3.0f, .imag = +0.0f },
                               { .real = +0.0f, .imag = -1.0f },
                               { .real = +1.0f, .imag = +2.0f } };

        cplx_t rtn_elements[6] = { { .real = +2.0f, .imag = +2.0f },
                                   { .real = +1.0f, .imag = +2.0f },
                                   { .real = +0.0f, .imag = +1.0f },
                                   { .real = -1.0f, .imag = -1.0f },
                                   { .real = +3.0f, .imag = +0.0f },
                                   { .real = +1.0f, .imag = -2.0f } };

        cmat_t * matrix = cmat_construct(num_rows, num_cols);
        cmat_t * rtn = cmat_construct(num_cols, num_rows);

        cmat_cst(matrix, elements);

        cmat_htranspose(rtn, matrix);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!((fabsf(rtn->elements[index_element].real - rtn_elements[index_element].real) < eps) &&
                  (fabsf(rtn->elements[index_element].imag - rtn_elements[index_element].imag) < eps))) {
                return -12;
            } 
        }

        cmat_destroy(matrix);
        cmat_destroy(rtn);

    }

    {

        unsigned int num_rows = 3;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;

        cplx_t elements[6] = { { .real = +2.0f, .imag = -2.0f },
                               { .real = -1.0f, .imag = +1.0f },
                               { .real = +1.0f, .imag = -2.0f },
                               { .real = +3.0f, .imag = +0.0f },
                               { .real = +0.0f, .imag = -1.0f },
                               { .real = +1.0f, .imag = +2.0f } };

        cplx_t scale = { .real = +2.0f, .imag = -1.0f };

        cplx_t rtn_elements[6] = { { .real = +2.0f, .imag = -6.0f },
                                   { .real = -1.0f, .imag = +3.0f },
                                   { .real = +0.0f, .imag = -5.0f },
                                   { .real = +6.0f, .imag = -3.0f },
                                   { .real = -1.0f, .imag = -2.0f },
                                   { .real = +4.0f, .imag = +3.0f } };

        cmat_t * matrix = cmat_construct(num_rows, num_cols);
        cmat_t * rtn = cmat_construct(num_rows, num_cols);

        cmat_cst(matrix, elements);

        cmat_scale(rtn, matrix, scale);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!((fabsf(rtn->elements[index_element].real - rtn_elements[index_element].real) < eps) &&
                  (fabsf(rtn->elements[index_element].imag - rtn_elements[index_element].imag) < eps))) {
                return -13;
            } 
        }        

        cmat_destroy(matrix);
        cmat_destroy(rtn);

    }

    {

        unsigned int num_rows = 4;
        unsigned int num_cols = 2;
        unsigned int num_elements = num_rows * num_cols;

        cplx_t elements[8] = { { .real = +1.0f, .imag = +2.0f }, 
                               { .real = +3.0f, .imag = +4.0f }, 
                               { .real = +5.0f, .imag = +6.0f }, 
                               { .real = +7.0f, .imag = +8.0f }, 
                               { .real = +2.0f, .imag = +4.0f },
                               { .real = +6.0f, .imag = +8.0f },
                               { .real = +1.0f, .imag = +3.0f },
                               { .real = +5.0f, .imag = +7.0f } };
        cplx_t rtn;
        cplx_t rtn_sum = { .real = +30.0f, .imag = +42.0f };

        cmat_t * matrix = cmat_construct(num_rows, num_cols);

        cmat_cst(matrix, elements);
        cmat_sum(&rtn, matrix);

        if (!((fabsf(rtn.real - rtn_sum.real) < eps) && (fabsf(rtn.imag - rtn_sum.imag) < eps))) {
            return -14;
        } 

        cmat_destroy(matrix);

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;

        float diag1[4] = { +10.0f, +20.0f, +30.0f, +40.0f };
        float utri1[6] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f };
        float diag2[4] = { +5.0f, +15.0f, +25.0f, +35.0f };
        float utri2[6] = { +0.0f, +1.0f, -2.0f, -2.0f, +3.0f, +1.0f };

        float rtn_diag[4] = { +15.0f, +35.0f, +55.0f, +75.0f };
        float rtn_utri[6] = { +1.0f, +3.0f, +1.0f, +2.0f, +8.0f, +7.0f };

        smat_t * matrix1 = smat_construct(num_rowscols);
        smat_t * matrix2 = smat_construct(num_rowscols);
        smat_t * rtn = smat_construct(num_rowscols);

        smat_cst(matrix1, diag1, utri1);
        smat_cst(matrix2, diag2, utri2);

        smat_add(rtn, matrix1, matrix2);

        for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
            if (!(fabsf(rtn->diag[index_diag_element] - rtn_diag[index_diag_element]) < eps)) {
                return -15;
            } 
        }
        for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
            if (!(fabsf(rtn->utri[index_utri_element] - rtn_utri[index_utri_element]) < eps)) {
                return -15;
            } 
        }          

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;

        float diag1[4] = { +10.0f, +20.0f, +30.0f, +40.0f };
        float utri1[6] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f };
        float diag2[4] = { +5.0f, +15.0f, +25.0f, +35.0f };
        float utri2[6] = { +0.0f, +1.0f, -2.0f, -2.0f, +3.0f, +1.0f };

        float rtn_diag[4] = { +5.0f, +5.0f, +5.0f, +5.0f };
        float rtn_utri[6] = { +1.0f, +1.0f, +5.0f, +6.0f, +2.0f, +5.0f };

        smat_t * matrix1 = smat_construct(num_rowscols);
        smat_t * matrix2 = smat_construct(num_rowscols);
        smat_t * rtn = smat_construct(num_rowscols);

        smat_cst(matrix1, diag1, utri1);
        smat_cst(matrix2, diag2, utri2);

        smat_sub(rtn, matrix1, matrix2);

        for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
            if (!(fabsf(rtn->diag[index_diag_element] - rtn_diag[index_diag_element]) < eps)) {
                return -16;
            } 
        }
        for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
            if (!(fabsf(rtn->utri[index_utri_element] - rtn_utri[index_utri_element]) < eps)) {
                return -16;
            } 
        }          

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;

        float diag1[4] = { +2.0f, +4.0f, +3.0f, +4.0f };
        float utri1[6] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f };
        float diag2[4] = { +3.0f, +2.0f, +1.0f, +2.0f };
        float utri2[6] = { +0.0f, +1.0f, -2.0f, -2.0f, +3.0f, +1.0f };

        float rtn_diag[4] = { +6.0f, +8.0f, +3.0f, +8.0f };
        float rtn_utri[6] = { +0.0f, +2.0f, -6.0f, -8.0f, +15.0f, +6.0f };

        smat_t * matrix1 = smat_construct(num_rowscols);
        smat_t * matrix2 = smat_construct(num_rowscols);
        smat_t * rtn = smat_construct(num_rowscols);

        smat_cst(matrix1, diag1, utri1);
        smat_cst(matrix2, diag2, utri2);

        smat_had(rtn, matrix1, matrix2);

        for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
            if (!(fabsf(rtn->diag[index_diag_element] - rtn_diag[index_diag_element]) < eps)) {
                return -17;
            } 
        }
        for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
            if (!(fabsf(rtn->utri[index_utri_element] - rtn_utri[index_utri_element]) < eps)) {
                return -17;
            } 
        }               

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;

        float diag[4] = { +2.0f, +4.0f, +3.0f, +4.0f };
        float utri[6] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f };
        float scale = +3.0f;

        float rtn_diag[4] = { +6.0f, +12.0f, +9.0f, +12.0f };
        float rtn_utri[6] = { +3.0f, +6.0f, +9.0f, +12.0f, +15.0f, +18.0f };

        smat_t * matrix = smat_construct(num_rowscols);
        smat_t * rtn = smat_construct(num_rowscols);

        smat_cst(matrix, diag, utri);
        smat_scale(rtn, matrix, scale);

        for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
            if (!(fabsf(rtn->diag[index_diag_element] - rtn_diag[index_diag_element]) < eps)) {
                return -18;
            } 
        }
        for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
            if (!(fabsf(rtn->utri[index_utri_element] - rtn_utri[index_utri_element]) < eps)) {
                return -18;
            } 
        }               

    }    

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;

        float diag[4] = { +2.0f, +4.0f, +3.0f, +4.0f };
        float utri[6] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f };

        float rtn;
        float rtn_sum = +55.0f;

        smat_t * matrix = smat_construct(num_rowscols);

        smat_cst(matrix, diag, utri);
        smat_sum(&rtn, matrix);

        if (!(fabsf(rtn - rtn_sum) < eps)) {
            return -19;
        } 

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;

        float diag1[4] = { +1.0f, +2.0f, +3.0f, +4.0f };
        cplx_t utri1[6] = { { .real = +1.0f, .imag = +2.0f }, 
                            { .real = +3.0f, .imag = +4.0f },
                            { .real = +5.0f, .imag = +6.0f },
                            { .real = +7.0f, .imag = +8.0f },
                            { .real = +4.0f, .imag = +0.0f },
                            { .real = +1.0f, .imag = +2.0f } };                           

        float diag2[4] = { +5.0f, +6.0f, +7.0f, +8.0f };
        cplx_t utri2[6] = { { .real = -1.0f, .imag = +2.0f }, 
                            { .real = +1.0f, .imag = -4.0f },
                            { .real = +2.0f, .imag = +1.0f },
                            { .real = -3.0f, .imag = +0.0f },
                            { .real = +2.0f, .imag = +1.0f },
                            { .real = +2.0f, .imag = -2.0f } };    

        float rtn_diag[4] = { +6.0f, +8.0f, +10.0f, +12.0f };
        cplx_t rtn_utri[6] = { { .real = +0.0f, .imag = +4.0f }, 
                               { .real = +4.0f, .imag = +0.0f },
                               { .real = +7.0f, .imag = +7.0f },
                               { .real = +4.0f, .imag = +8.0f },
                               { .real = +6.0f, .imag = +1.0f },
                               { .real = +3.0f, .imag = +0.0f } };   

        hmat_t * matrix1 = hmat_construct(num_rowscols);
        hmat_t * matrix2 = hmat_construct(num_rowscols);
        hmat_t * rtn = hmat_construct(num_rowscols);

        hmat_cst(matrix1, diag1, utri1);
        hmat_cst(matrix2, diag2, utri2);

        hmat_add(rtn, matrix1, matrix2);

        for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
            if (!(fabsf(rtn->diag[index_diag_element] - rtn_diag[index_diag_element]) < eps)) {
                return -20;
            } 
        }
        for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
            if (!((fabsf(rtn->utri[index_utri_element].real - rtn_utri[index_utri_element].real) < eps) &&
                  (fabsf(rtn->utri[index_utri_element].real - rtn_utri[index_utri_element].real) < eps))) {
                return -20;
            } 
        }         

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;

        float diag1[4] = { +1.0f, +2.0f, +3.0f, +4.0f };
        cplx_t utri1[6] = { { .real = +1.0f, .imag = +2.0f }, 
                            { .real = +3.0f, .imag = +4.0f },
                            { .real = +5.0f, .imag = +6.0f },
                            { .real = +2.0f, .imag = +8.0f },
                            { .real = +4.0f, .imag = +0.0f },
                            { .real = +1.0f, .imag = +2.0f } };                           

        float diag2[4] = { +5.0f, +6.0f, +7.0f, +3.0f };
        cplx_t utri2[6] = { { .real = -1.0f, .imag = +2.0f }, 
                            { .real = +1.0f, .imag = -4.0f },
                            { .real = +2.0f, .imag = +1.0f },
                            { .real = -3.0f, .imag = +0.0f },
                            { .real = +2.0f, .imag = +1.0f },
                            { .real = +2.0f, .imag = -2.0f } };    

        float rtn_diag[4] = { -4.0f, -4.0f, -4.0f, +1.0f };
        cplx_t rtn_utri[6] = { { .real = +2.0f, .imag = +0.0f }, 
                               { .real = +2.0f, .imag = +8.0f },
                               { .real = +3.0f, .imag = +5.0f },
                               { .real = +5.0f, .imag = +8.0f },
                               { .real = +2.0f, .imag = -1.0f },
                               { .real = -1.0f, .imag = +4.0f } };   

        hmat_t * matrix1 = hmat_construct(num_rowscols);
        hmat_t * matrix2 = hmat_construct(num_rowscols);
        hmat_t * rtn = hmat_construct(num_rowscols);

        hmat_cst(matrix1, diag1, utri1);
        hmat_cst(matrix2, diag2, utri2);

        hmat_sub(rtn, matrix1, matrix2);

        for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
            if (!(fabsf(rtn->diag[index_diag_element] - rtn_diag[index_diag_element]) < eps)) {
                return -21;
            } 
        }
        for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
            if (!((fabsf(rtn->utri[index_utri_element].real - rtn_utri[index_utri_element].real) < eps) &&
                  (fabsf(rtn->utri[index_utri_element].real - rtn_utri[index_utri_element].real) < eps))) {
                return -21;
            } 
        }         

    }    

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;

        float diag[4] = { +1.0f, +2.0f, +3.0f, +4.0f };
        cplx_t utri[6] = { { .real = +1.0f, .imag = +2.0f }, 
                           { .real = +3.0f, .imag = +4.0f },
                           { .real = +5.0f, .imag = +6.0f },
                           { .real = +2.0f, .imag = +8.0f },
                           { .real = +4.0f, .imag = +0.0f },
                           { .real = +1.0f, .imag = +2.0f } };

        float scale = 2.0f;                      

        float rtn_diag[4] = { +2.0f, +4.0f, +6.0f, +8.0f };
        cplx_t rtn_utri[6] = { { .real = +2.0f, .imag = +4.0f }, 
                               { .real = +6.0f, .imag = +8.0f },
                               { .real = +10.0f, .imag = +12.0f },
                               { .real = +4.0f, .imag = +16.0f },
                               { .real = +8.0f, .imag = +0.0f },
                               { .real = +2.0f, .imag = +4.0f } };   

        hmat_t * matrix = hmat_construct(num_rowscols);
        hmat_t * rtn = hmat_construct(num_rowscols);

        hmat_cst(matrix, diag, utri);

        hmat_scale(rtn, matrix, scale);

        for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
            if (!(fabsf(rtn->diag[index_diag_element] - rtn_diag[index_diag_element]) < eps)) {
                return -22;
            } 
        }
        for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
            if (!((fabsf(rtn->utri[index_utri_element].real - rtn_utri[index_utri_element].real) < eps) &&
                  (fabsf(rtn->utri[index_utri_element].real - rtn_utri[index_utri_element].real) < eps))) {
                return -22;
            } 
        }         

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;
        unsigned int num_elements = 16;

        float diag[4] = { +2.0f, +4.0f, +3.0f, +4.0f };
        float utri[6] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f };

        float rtn_elements[16] = { +2.0f, +1.0f, +2.0f, +3.0f,
                                   +1.0f, +4.0f, +4.0f, +5.0f,
                                   +2.0f, +4.0f, +3.0f, +6.0f,
                                   +3.0f, +5.0f, +6.0f, +4.0f };

        smat_t * matrix = smat_construct(num_rowscols);
        rmat_t * rtn = rmat_construct(num_rowscols, num_rowscols);

        smat_cst(matrix, diag, utri);
        linalg_smat2rmat(rtn, matrix);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!(fabsf(rtn->elements[index_element] - rtn_elements[index_element]) < eps)) {
                return -23;
            } 
        }

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;
        unsigned int num_elements = 16;

        float elements[16] = { +2.0f, +1.0f, +2.0f, +3.0f,
                               +1.0f, +4.0f, +4.0f, +5.0f,
                               +2.0f, +4.0f, +3.0f, +6.0f,
                               +3.0f, +5.0f, +6.0f, +4.0f };      

        float rtn_diag[4] = { +2.0f, +4.0f, +3.0f, +4.0f };
        float rtn_utri[6] = { +1.0f, +2.0f, +3.0f, +4.0f, +5.0f, +6.0f };
  
        rmat_t * matrix = rmat_construct(num_rowscols, num_rowscols);
        smat_t * rtn = smat_construct(num_rowscols);

        rmat_cst(matrix, elements);
        linalg_rmat2smat(rtn, matrix);

        for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
            if (!(fabsf(rtn->diag[index_diag_element] - rtn_diag[index_diag_element]) < eps)) {
                return -24;
            } 
        }
        for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
            if (!(fabsf(rtn->utri[index_utri_element] - rtn_utri[index_utri_element]) < eps)) {
                return -24;
            } 
        }       

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;
        unsigned int num_elements = 16;

        float diag[4] = { +2.0f, +4.0f, +3.0f, +4.0f };
        cplx_t utri[6] = { { .real = +1.0f, .imag = +2.0f }, 
                           { .real = +3.0f, .imag = +4.0f },
                           { .real = +5.0f, .imag = +6.0f },
                           { .real = -1.0f, .imag = +3.0f },
                           { .real = +2.0f, .imag = -4.0f },
                           { .real = -3.0f, .imag = +2.0f } };

        cplx_t rtn_elements[16] = { { .real = +2.0f, .imag = +0.0f }, { .real = +1.0f, .imag = +2.0f }, { .real = +3.0f, .imag = +4.0f }, { .real = +5.0f, .imag = +6.0f }, 
                                    { .real = +1.0f, .imag = -2.0f }, { .real = +4.0f, .imag = +0.0f }, { .real = -1.0f, .imag = +3.0f }, { .real = +2.0f, .imag = -4.0f },
                                    { .real = +3.0f, .imag = -4.0f }, { .real = -1.0f, .imag = -3.0f }, { .real = +3.0f, .imag = +0.0f }, { .real = -3.0f, .imag = +2.0f },
                                    { .real = +5.0f, .imag = -6.0f }, { .real = +2.0f, .imag = +4.0f }, { .real = -3.0f, .imag = -2.0f }, { .real = +4.0f, .imag = +0.0f } };

        hmat_t * matrix = hmat_construct(num_rowscols);
        cmat_t * rtn = cmat_construct(num_rowscols, num_rowscols);

        hmat_cst(matrix, diag, utri);
        linalg_hmat2cmat(rtn, matrix);

        for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
            if (!((fabsf(rtn->elements[index_element].real - rtn_elements[index_element].real) < eps) &&
                  (fabsf(rtn->elements[index_element].imag - rtn_elements[index_element].imag) < eps))) {
                return -25;
            } 
        }

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;

        float elements[4] = { +1.0f, +2.0f, +3.0f, +4.0f };
        float rtn_diag[4] = { +1.0f, +4.0f, +9.0f, +16.0f };
        float rtn_utri[6] = { +2.0f, +3.0f, +4.0f, +6.0f, +8.0f, +12.0f };

        rmat_t * vector = rmat_construct(num_rowscols, 1);
        smat_t * rtn = smat_construct(num_rowscols);

        rmat_cst(vector, elements);
        smat_rank1(rtn, vector);

        for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
            if (!(fabsf(rtn->diag[index_diag_element] - rtn_diag[index_diag_element]) < eps)) {
                return -26;
            } 
        }
        for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
            if (!(fabsf(rtn->utri[index_utri_element] - rtn_utri[index_utri_element]) < eps)) {
                return -26;
            } 
        }       

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;

        cplx_t elements[4] = { { .real = +1.0f, .imag = +2.0f },
                               { .real = +3.0f, .imag = +4.0f },
                               { .real = +2.0f, .imag = +3.0f },
                               { .real = +1.0f, .imag = -2.0f } };
        float rtn_diag[4] = { +5.0f, +25.0f, +13.0f, +5.0f };
        cplx_t rtn_utri[6] = { { .real = +11.0f, .imag = +2.0f },
                               { .real = +8.0f, .imag = +1.0f },
                               { .real = -3.0f, .imag = +4.0f },
                               { .real = +18.0f, .imag = -1.0f },
                               { .real = -5.0f, .imag = +10.0f },
                               { .real = -4.0f, .imag = +7.0f } };

        cmat_t * vector = cmat_construct(num_rowscols, 1);
        hmat_t * rtn = hmat_construct(num_rowscols);

        cmat_cst(vector, elements);
        hmat_rank1(rtn, vector);

        for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
            if (!(fabsf(rtn->diag[index_diag_element] - rtn_diag[index_diag_element]) < eps)) {
                return -27;
            } 
        }
        for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
            if (!((fabsf(rtn->utri[index_utri_element].real - rtn_utri[index_utri_element].real) < eps) &&
                  (fabsf(rtn->utri[index_utri_element].imag - rtn_utri[index_utri_element].imag) < eps))) {
                return -27;
            } 
        }       

    }

    {

        unsigned int num_rowscols = 4;
        unsigned int num_diag_elements = 4;
        unsigned int num_utri_elements = 6;

        float diag[4] = { +1.0f, +2.0f, +3.0f, +4.0f };
        cplx_t utri[6] = { { .real = +1.0f, .imag = +2.0f }, 
                           { .real = +3.0f, .imag = +4.0f },
                           { .real = +5.0f, .imag = +6.0f },
                           { .real = +2.0f, .imag = +8.0f },
                           { .real = +4.0f, .imag = +0.0f },
                           { .real = +1.0f, .imag = +2.0f } };

        hmat_t * matrix = hmat_construct(num_rowscols);

        float rtn;
        float rtn_sum = 42.0f;

        hmat_cst(matrix, diag, utri);
        hmat_sum(&rtn, matrix);

        if (!(fabsf(rtn - rtn_sum) < eps)) {
            return -28;
        } 

        hmat_destroy(matrix);         

    }

    return 0;

}