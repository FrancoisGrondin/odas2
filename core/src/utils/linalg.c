#include <utils/linalg.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

rmat_t * rmat_construct(const unsigned int num_rows, const unsigned int num_cols) {

    rmat_t * obj = (rmat_t *) malloc(sizeof(rmat_t));

    obj->num_rows = num_rows;
    obj->num_cols = num_cols;

    obj->elements = (float *) calloc(num_rows * num_cols, sizeof(float));

    return obj;

}

void rmat_destroy(rmat_t * obj) {

    free(obj->elements);
    free(obj);

}

int rmat_cst(rmat_t * obj, const float * elements) {

    memcpy(obj->elements, elements, sizeof(float) * obj->num_rows * obj->num_cols);

    return 0;

}

int rmat_add(rmat_t * dst, const rmat_t * op1, const rmat_t * op2) {

    unsigned int num_rows = dst->num_rows;
    unsigned int num_cols = dst->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {
        
        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {
        
            unsigned int index = index_row * num_cols + index_col;
            dst->elements[index] = op1->elements[index] + op2->elements[index];
        
        }

    }

    return 0;

}

int rmat_sub(rmat_t * dst, const rmat_t * op1, const rmat_t * op2) {

    unsigned int num_rows = dst->num_rows;
    unsigned int num_cols = dst->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {
        
        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {
        
            unsigned int index = index_row * num_cols + index_col;
            dst->elements[index] = op1->elements[index] - op2->elements[index];
        
        }
    
    }

    return 0;

}

int rmat_mul(rmat_t * dst, const rmat_t * op1, const rmat_t * op2) {

    unsigned int num_rows = dst->num_rows;
    unsigned int num_cols = dst->num_cols;
    unsigned int num_elements = op1->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {

            float total = 0.0f;

            for (unsigned int index_element = 0; index_element < num_elements; index_element++) {

                unsigned int index1 = index_row * num_elements + index_element;
                unsigned int index2 = index_element * num_cols + index_col;

                total += op1->elements[index1] * op2->elements[index2];

            }

            unsigned int index = index_row * num_cols + index_col;
            dst->elements[index] = total;

        }

    }

    return 0;

}

int rmat_had(rmat_t * dst, const rmat_t * op1, const rmat_t * op2) {

    unsigned int num_elements = dst->num_rows * dst->num_cols;
    
    for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
    
        dst->elements[index_element] = op1->elements[index_element] * op2->elements[index_element];
    
    }

    return 0;

}

int rmat_transpose(rmat_t * dst, const rmat_t * op) {

    unsigned int num_rows = dst->num_rows;
    unsigned int num_cols = dst->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {

            unsigned int index1 = index_row * num_cols + index_col;
            unsigned int index2 = index_col * num_rows + index_row;

            dst->elements[index1] = op->elements[index2];

        }

    }

    return 0;

}

int rmat_scale(rmat_t * dst, const rmat_t * op1, const float op2) {

    unsigned int num_rows = dst->num_rows;
    unsigned int num_cols = dst->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {

            unsigned int index_element = index_row * num_cols + index_col;

            dst->elements[index_element] = op1->elements[index_element] * op2;

        }

    }

    return 0;

}

int rmat_sum(float * dst, const rmat_t * op1) {

    unsigned int num_rows = op1->num_rows;
    unsigned int num_cols = op1->num_cols;

    float total = 0.0f;

    for (unsigned int index_element = 0; index_element < (num_rows * num_cols); index_element++) {
        total += op1->elements[index_element];
    }

    *dst = total;

    return 0;

}

void rmat_printf(const rmat_t * obj) {

    unsigned int num_rows = obj->num_rows;
    unsigned int num_cols = obj->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {

            unsigned int index = index_row * num_cols + index_col;
            float value = obj->elements[index];

            printf("%+8.2f ", value);

        }

        printf("\n");

    }

}

cmat_t * cmat_construct(const unsigned int num_rows, const unsigned int num_cols) {

    cmat_t * obj = (cmat_t *) malloc(sizeof(cmat_t));

    obj->num_rows = num_rows;
    obj->num_cols = num_cols;

    obj->elements = (cplx_t *) calloc(num_rows * num_cols, sizeof(cplx_t));

    return obj;

}

void cmat_destroy(cmat_t * obj) {

    free(obj->elements);
    free(obj);

}

int cmat_cst(cmat_t * obj, cplx_t * elements) {

    memcpy(obj->elements, elements, sizeof(cplx_t) * obj->num_rows * obj->num_cols);

    return 0;

}

int cmat_add(cmat_t * dst, const cmat_t * op1, const cmat_t * op2) {

    unsigned int num_rows = dst->num_rows;
    unsigned int num_cols = dst->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {
        
        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {
        
            unsigned int index = index_row * num_cols + index_col;
            dst->elements[index] = cplx_add(op1->elements[index], op2->elements[index]);
        
        }

    }

    return 0;

}

int cmat_sub(cmat_t * dst, const cmat_t * op1, const cmat_t * op2) {

    unsigned int num_rows = dst->num_rows;
    unsigned int num_cols = dst->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {
        
        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {
        
            unsigned int index = index_row * num_cols + index_col;
            dst->elements[index] = cplx_sub(op1->elements[index], op2->elements[index]);
        
        }

    }

    return 0;


}

int cmat_mul(cmat_t * dst, const cmat_t * op1, const cmat_t * op2) {

    unsigned int num_rows = dst->num_rows;
    unsigned int num_cols = dst->num_cols;
    unsigned int num_elements = op1->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {

            cplx_t total = cplx_cst(0.0f, 0.0f);

            for (unsigned int index_element = 0; index_element < num_elements; index_element++) {

                unsigned int index1 = index_row * num_elements + index_element;
                unsigned int index2 = index_element * num_cols + index_col;

                total = cplx_add(total, cplx_mul(op1->elements[index1], op2->elements[index2]));

            }

            unsigned int index = index_row * num_cols + index_col;
            dst->elements[index] = total;

        }

    }

    return 0;
}

int cmat_had(cmat_t * dst, const cmat_t * op1, const cmat_t * op2) {

    unsigned int num_elements = dst->num_rows * dst->num_cols;
    
    for (unsigned int index_element = 0; index_element < num_elements; index_element++) {
    
        dst->elements[index_element] = cplx_mul(op1->elements[index_element], op2->elements[index_element]);
    
    }

    return 0;

}

int cmat_htranspose(cmat_t * dst, const cmat_t * op) {

    unsigned int num_rows = dst->num_rows;
    unsigned int num_cols = dst->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {

            unsigned int index1 = index_row * num_cols + index_col;
            unsigned int index2 = index_col * num_rows + index_row;

            dst->elements[index1] = cplx_conj(op->elements[index2]);

        }

    }

    return 0;

}

int cmat_scale(cmat_t * dst, const cmat_t * op1, const cplx_t op2) {

    unsigned int num_rows = dst->num_rows;
    unsigned int num_cols = dst->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {

            unsigned int index_element = index_row * num_cols + index_col;

            dst->elements[index_element] = cplx_mul(op1->elements[index_element], op2);

        }

    }

    return 0;

}

int cmat_sum(cplx_t * dst, const cmat_t * op1) {

    unsigned int num_rows = op1->num_rows;
    unsigned int num_cols = op1->num_cols;

    cplx_t total = { .real = 0.0f, .imag = 0.0f };

    for (unsigned int index_element = 0; index_element < (num_rows * num_cols); index_element++) {
        total = cplx_add(total, op1->elements[index_element]);
    }

    *dst = total;

    return 0;

}

void cmat_printf(const cmat_t * obj) {

    unsigned int num_rows = obj->num_rows;
    unsigned int num_cols = obj->num_cols;

    for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

        for (unsigned int index_col = 0; index_col < num_cols; index_col++) {

            unsigned int index = index_row * num_cols + index_col;
            cplx_t value = obj->elements[index];

            printf("(%+8.2f + j %+8.2f) ", value.real, value.imag);

        }

        printf("\n");

    }    

}


smat_t * smat_construct(const unsigned int num_rowscols) {

    smat_t * obj = (smat_t *) malloc(sizeof(smat_t));

    obj->num_rowscols = num_rowscols;

    obj->diag = (float *) calloc(num_rowscols, sizeof(float));
    obj->utri = (float *) calloc(num_rowscols * (num_rowscols-1) / 2, sizeof(float));

    return obj;    

}

void smat_destroy(smat_t * obj) {

    free(obj->diag);
    free(obj->utri);
    free(obj);

}

int smat_cst(smat_t * obj, const float * diag, const float * utri) {

    memcpy(obj->diag, diag, sizeof(float) * obj->num_rowscols);
    memcpy(obj->utri, utri, sizeof(float) * obj->num_rowscols * (obj->num_rowscols - 1) / 2);

    return 0;

}

int smat_eye(smat_t * obj) {

    for (unsigned int index_diag_element = 0; index_diag_element < obj->num_rowscols; index_diag_element++) {
        obj->diag[index_diag_element] = 1.0f;
    }
    for (unsigned int index_utri_element = 0; index_utri_element < obj->num_rowscols * (obj->num_rowscols - 1) / 2; index_utri_element++) {
        obj->utri[index_utri_element] = 0.0f;
    }

    return 0;

}

int smat_add(smat_t * dst, const smat_t * op1, const smat_t * op2) {

    unsigned int num_diag_elements = dst->num_rowscols;
    unsigned int num_utri_elements = dst->num_rowscols * (dst->num_rowscols - 1) / 2;

    for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
        dst->diag[index_diag_element] = op1->diag[index_diag_element] + op2->diag[index_diag_element];
    }
    for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
        dst->utri[index_utri_element] = op1->utri[index_utri_element] + op2->utri[index_utri_element];
    }

    return 0;

}

int smat_sub(smat_t * dst, const smat_t * op1, const smat_t * op2) {

    unsigned int num_diag_elements = dst->num_rowscols;
    unsigned int num_utri_elements = dst->num_rowscols * (dst->num_rowscols - 1) / 2;

    for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
        dst->diag[index_diag_element] = op1->diag[index_diag_element] - op2->diag[index_diag_element];
    }
    for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
        dst->utri[index_utri_element] = op1->utri[index_utri_element] - op2->utri[index_utri_element];
    }

    return 0;

}

int smat_had(smat_t * dst, const smat_t * op1, const smat_t * op2) {

    unsigned int num_diag_elements = dst->num_rowscols;
    unsigned int num_utri_elements = dst->num_rowscols * (dst->num_rowscols - 1) / 2;

    for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
        dst->diag[index_diag_element] = op1->diag[index_diag_element] * op2->diag[index_diag_element];
    }
    for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
        dst->utri[index_utri_element] = op1->utri[index_utri_element] * op2->utri[index_utri_element];
    }

    return 0;

}

int smat_scale(smat_t * dst, const smat_t * op1, const float op2) {

    unsigned int num_diag_elements = dst->num_rowscols;
    unsigned int num_utri_elements = dst->num_rowscols * (dst->num_rowscols - 1) / 2;

    for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
        dst->diag[index_diag_element] = op1->diag[index_diag_element] * op2;
    }
    for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
        dst->utri[index_utri_element] = op1->utri[index_utri_element] * op2;
    }

    return 0;

}

int smat_rank1(smat_t * dst, const rmat_t * op) {

    unsigned int index_utri_element = 0;

    for (unsigned int index_row = 0; index_row < dst->num_rowscols; index_row++) {

        dst->diag[index_row] = op->elements[index_row] * op->elements[index_row];

        for (unsigned int index_col = (index_row+1); index_col < dst->num_rowscols; index_col++) {

            dst->utri[index_utri_element] = op->elements[index_row] * op->elements[index_col];
            index_utri_element++;

        }

    }

    return 0;

}

int smat_sum(float * dst, const smat_t * op1) {

    unsigned int num_diag_elements = op1->num_rowscols;
    unsigned int num_utri_elements = op1->num_rowscols * (op1->num_rowscols - 1) / 2;

    float total = 0.0f;

    for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
        total += op1->diag[index_diag_element];
    }
    for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
        total += 2 * op1->utri[index_utri_element];
    }

    *dst = total;

    return 0;

}

void smat_printf(const smat_t * obj) {

    for (unsigned int index_row = 0; index_row < obj->num_rowscols; index_row++) {
        
        for (unsigned int index_col = 0; index_col < obj->num_rowscols; index_col++) {

            float value;

            if (index_row == index_col) {

                unsigned int index_diag = index_row;
                value = obj->diag[index_diag];

            }
            if (index_row < index_col) {

                unsigned int index_utri = index_row * obj->num_rowscols - (index_row * (index_row + 1)) / 2 + (index_col - index_row - 1);
                value = obj->utri[index_utri];

            }
            if (index_row > index_col) {

                unsigned int index_utri = index_col * obj->num_rowscols - (index_col * (index_col + 1)) / 2 + (index_row - index_col - 1);
                value = obj->utri[index_utri];

            }

            printf("%+8.2f ", value);

        }

        printf("\n");

    }

}


hmat_t * hmat_construct(const unsigned int num_rowscols) {

    hmat_t * obj = (hmat_t *) malloc(sizeof(hmat_t));

    obj->num_rowscols = num_rowscols;

    obj->diag = (float *) calloc(num_rowscols, sizeof(float));
    obj->utri = (cplx_t *) calloc(num_rowscols * (num_rowscols-1) / 2, sizeof(cplx_t));

    return obj;

}

void hmat_destroy(hmat_t * obj) {

    free(obj->diag);
    free(obj->utri);
    free(obj);

}

int hmat_cst(hmat_t * obj, const float * diag, const cplx_t * utri) {

    memcpy(obj->diag, diag, sizeof(float) * obj->num_rowscols);
    memcpy(obj->utri, utri, sizeof(cplx_t) * obj->num_rowscols * (obj->num_rowscols - 1) / 2);    

    return 0;

}

int hmat_eye(hmat_t * obj) {

    for (unsigned int index_diag_element = 0; index_diag_element < obj->num_rowscols; index_diag_element++) {
        obj->diag[index_diag_element] = 1.0f;
    }
    for (unsigned int index_utri_element = 0; index_utri_element < obj->num_rowscols * (obj->num_rowscols - 1) / 2; index_utri_element++) {
        obj->utri[index_utri_element] = cplx_cst(0.0f, 0.0f);
    }

    return 0;

}

int hmat_add(hmat_t * dst, const hmat_t * op1, const hmat_t * op2) {

    unsigned num_diag_elements = dst->num_rowscols;
    unsigned num_utri_elements = dst->num_rowscols * (dst->num_rowscols - 1) / 2;

    for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
        dst->diag[index_diag_element] = op1->diag[index_diag_element] + op2->diag[index_diag_element];
    }
    for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
        dst->utri[index_utri_element] = cplx_add(op1->utri[index_utri_element], op2->utri[index_utri_element]);
    }

    return 0;

}

int hmat_sub(hmat_t * dst, const hmat_t * op1, const hmat_t * op2) {

    unsigned num_diag_elements = dst->num_rowscols;
    unsigned num_utri_elements = dst->num_rowscols * (dst->num_rowscols - 1) / 2;

    for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
        dst->diag[index_diag_element] = op1->diag[index_diag_element] - op2->diag[index_diag_element];
    }
    for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
        dst->utri[index_utri_element] = cplx_sub(op1->utri[index_utri_element], op2->utri[index_utri_element]);
    }

    return 0;

}

int hmat_had(hmat_t * dst, const hmat_t * op1, const hmat_t * op2) {

    unsigned num_diag_elements = dst->num_rowscols;
    unsigned num_utri_elements = dst->num_rowscols * (dst->num_rowscols - 1) / 2;

    for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
        dst->diag[index_diag_element] = op1->diag[index_diag_element] * op2->diag[index_diag_element];
    }
    for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
        dst->utri[index_utri_element] = cplx_mul(op1->utri[index_utri_element], op2->utri[index_utri_element]);
    }

    return 0;

}

int hmat_scale(hmat_t * dst, const hmat_t * op1, const float op2) {

    unsigned num_diag_elements = dst->num_rowscols;
    unsigned num_utri_elements = dst->num_rowscols * (dst->num_rowscols - 1) / 2;

    for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
        dst->diag[index_diag_element] = op1->diag[index_diag_element] * op2;
    }
    for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
        dst->utri[index_utri_element] = cplx_mul(op1->utri[index_utri_element], cplx_cst(op2, 0.0f));
    }

    return 0;

}

int hmat_rank1(hmat_t * dst, const cmat_t * op) {

    unsigned int index_utri_element = 0;

    for (unsigned int index_row = 0; index_row < dst->num_rowscols; index_row++) {

        dst->diag[index_row] = cplx_l2(op->elements[index_row]);

        for (unsigned int index_col = (index_row+1); index_col < dst->num_rowscols; index_col++) {

            dst->utri[index_utri_element] = cplx_mul(op->elements[index_row], cplx_conj(op->elements[index_col]));
            index_utri_element++;

        }

    }

    return 0;

}

int hmat_sum(float * dst, const hmat_t * op1) {

    unsigned int num_diag_elements = op1->num_rowscols;
    unsigned int num_utri_elements = op1->num_rowscols * (op1->num_rowscols - 1) / 2;

    float total = 0.0f;

    for (unsigned int index_diag_element = 0; index_diag_element < num_diag_elements; index_diag_element++) {
        total += op1->diag[index_diag_element];
    }
    for (unsigned int index_utri_element = 0; index_utri_element < num_utri_elements; index_utri_element++) {
        total += 2 * op1->utri[index_utri_element].real;
    }

    *dst = total;

    return 0;

}

void hmat_printf(const hmat_t * obj) {

    for (unsigned int index_row = 0; index_row < obj->num_rowscols; index_row++) {
        
        for (unsigned int index_col = 0; index_col < obj->num_rowscols; index_col++) {

            cplx_t value;

            if (index_row == index_col) {

                unsigned int index_diag = index_row;
                value = cplx_cst(obj->diag[index_diag], 0.0f);

            }
            if (index_row < index_col) {

                unsigned int index_utri = index_row * obj->num_rowscols - (index_row * (index_row + 1)) / 2 + (index_col - index_row - 1);
                value = obj->utri[index_utri];

            }
            if (index_row > index_col) {

                unsigned int index_utri = index_col * obj->num_rowscols - (index_col * (index_col + 1)) / 2 + (index_row - index_col - 1);
                value = cplx_conj(obj->utri[index_utri]);

            }

            printf("(%+8.2f + j %+8.2f) ", value.real, value.imag);

        }

        printf("\n");

    }    

}

int linalg_smat2rmat(rmat_t * dst, smat_t * src) {

    for (unsigned int index_diag_element = 0; index_diag_element < src->num_rowscols; index_diag_element++) {
        dst->elements[index_diag_element * dst->num_cols + index_diag_element] = src->diag[index_diag_element];
    }
    
    unsigned int index_utri_element = 0;

    for (unsigned int index_row = 0; index_row < dst->num_rows; index_row++) {
        for (unsigned int index_col = (index_row + 1); index_col < dst->num_cols; index_col++) {
            dst->elements[index_row * dst->num_cols + index_col] = src->utri[index_utri_element];
            dst->elements[index_col * dst->num_cols + index_row] = src->utri[index_utri_element];
            index_utri_element++;
        }
    }

    return 0;

}

int linalg_rmat2smat(smat_t * dst, rmat_t * src) {

    for (unsigned int index_diag_element = 0; index_diag_element < dst->num_rowscols; index_diag_element++) {
        dst->diag[index_diag_element] = src->elements[index_diag_element * src->num_cols + index_diag_element];
    }

    unsigned int index_utri_element = 0;

    for (unsigned int index_row = 0; index_row < src->num_rows; index_row++) {
        for (unsigned int index_col = (index_row +1); index_col < src->num_cols; index_col++) {
            dst->utri[index_utri_element] = src->elements[index_row * src->num_cols + index_col];
            index_utri_element++;
        }
    }

    return 0;

}

int linalg_hmat2cmat(cmat_t * dst, hmat_t * src) {

    for (unsigned int index_diag_element = 0; index_diag_element < src->num_rowscols; index_diag_element++) {
        dst->elements[index_diag_element * dst->num_cols + index_diag_element] = cplx_cst(src->diag[index_diag_element], 0.0f);
    }
    
    unsigned int index_utri_element = 0;

    for (unsigned int index_row = 0; index_row < dst->num_rows; index_row++) {
        for (unsigned int index_col = (index_row + 1); index_col < dst->num_cols; index_col++) {
            dst->elements[index_row * dst->num_cols + index_col] = src->utri[index_utri_element];
            dst->elements[index_col * dst->num_cols + index_row] = cplx_conj(src->utri[index_utri_element]);
            index_utri_element++;
        }
    }

    return 0;

}

int linalg_cmat2hmat(hmat_t * dst, cmat_t * src) {

    for (unsigned int index_diag_element = 0; index_diag_element < dst->num_rowscols; index_diag_element++) {
        dst->diag[index_diag_element] = src->elements[index_diag_element * src->num_cols + index_diag_element].real;
    }

    unsigned int index_utri_element = 0;

    for (unsigned int index_row = 0; index_row < src->num_rows; index_row++) {
        for (unsigned int index_col = (index_row +1); index_col < src->num_cols; index_col++) {
            dst->utri[index_utri_element] = src->elements[index_row * src->num_cols + index_col];
            index_utri_element++;
        }
    }

    return 0;

}