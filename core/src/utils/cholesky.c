#include <utils/decompose.h>
#include <utils/error.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

static int cholesky_decompose_real(float * dst, const float * src, const unsigned int num_rowscols);
static int invert_tril_real(float * dst, const float * src, const unsigned int num_rowscols);
static int mult_tril_transpose(float * dst, const float * src, const unsigned int num_rowscols);
static int cholesky_decompose_cplx(cplx_t * dst, const cplx_t * src, const unsigned int num_rowscols);
static int invert_tril_cplx(cplx_t * dst, const cplx_t * src, const unsigned int num_rowscols);
static int mult_tril_hermitian(cplx_t * dst, const cplx_t * src, const unsigned int num_rowscols);

invpds_t * invpds_construct(const unsigned int num_rowscols) {

	invpds_t * obj = (invpds_t *) malloc(sizeof(invpds_t));

	obj->num_rowscols = num_rowscols;

	obj->A = (float *) malloc(sizeof(float) * obj->num_rowscols * obj->num_rowscols);
	obj->L = (float *) malloc(sizeof(float) * obj->num_rowscols * obj->num_rowscols);
	obj->Linv = (float *) malloc(sizeof(float) * obj->num_rowscols * obj->num_rowscols);
	obj->Ainv = (float *) malloc(sizeof(float) * obj->num_rowscols * obj->num_rowscols);

	return obj;

}

void invpds_destroy(invpds_t * obj) {

	free((void *) obj->A);
	free((void *) obj->L);
	free((void *) obj->Linv);
	free((void *) obj->Ainv);

	free((void *) obj);

}

int invpds_process(invpds_t * obj, const float * in_diag, const float * in_triu, float * out_diag, float * out_triu) {

	unsigned int num_rowscols = obj->num_rowscols;
	unsigned int num_rows = obj->num_rowscols;
	unsigned int num_cols = obj->num_rowscols;

	memset(obj->A, 0x00, sizeof(float) * num_rows * num_cols);

	unsigned int index_element = 0;

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

		obj->A[index_row * num_cols + index_row] = in_diag[index_row];

		for (unsigned int index_col = (index_row+1); index_col < num_cols; index_col++) {

			obj->A[index_row * num_cols + index_col] = in_triu[index_element];
			obj->A[index_col * num_rows + index_row] = in_triu[index_element++];

		}

	}

	// A = L @ L^T
	cholesky_decompose_real(obj->L, obj->A, num_rowscols);

	// L > L^-1
	invert_tril_real(obj->Linv, obj->L, num_rowscols);

	// A^-1 = (L^-1)^T @ L^-1
	mult_tril_transpose(obj->Ainv, obj->Linv, num_rowscols);

	index_element = 0;

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

		out_diag[index_row] = obj->Ainv[index_row * num_cols + index_row];

		for (unsigned int index_col = (index_row+1); index_col < num_cols; index_col++) {

			out_triu[index_element++] = obj->Ainv[index_row * num_cols + index_col];

		}

	}

	return 0;

}

invpdh_t * invpdh_construct(const unsigned int num_rowscols) {

	invpdh_t * obj = (invpdh_t *) malloc(sizeof(invpdh_t));

	obj->num_rowscols = num_rowscols;

	obj->A = (cplx_t *) malloc(sizeof(cplx_t) * obj->num_rowscols * obj->num_rowscols);
	obj->L = (cplx_t *) malloc(sizeof(cplx_t) * obj->num_rowscols * obj->num_rowscols);
	obj->Linv = (cplx_t *) malloc(sizeof(cplx_t) * obj->num_rowscols * obj->num_rowscols);
	obj->Ainv = (cplx_t *) malloc(sizeof(cplx_t) * obj->num_rowscols * obj->num_rowscols);

	return obj;

}

void invpdh_destroy(invpdh_t * obj) {

	free((void *) obj->A);
	free((void *) obj->L);
	free((void *) obj->Linv);
	free((void *) obj->Ainv);

	free((void *) obj);

}

int invpdh_process(invpdh_t * obj, const float * in_diag, const cplx_t * in_triu, float * out_diag, cplx_t * out_triu) {

	unsigned int num_rowscols = obj->num_rowscols;
	unsigned int num_rows = obj->num_rowscols;
	unsigned int num_cols = obj->num_rowscols;

	memset(obj->A, 0x00, sizeof(cplx_t) * num_rows * num_cols);

	unsigned int index_element = 0;

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

		obj->A[index_row * num_cols + index_row] = cplx_cst(in_diag[index_row], 0.0f);

		for (unsigned int index_col = (index_row+1); index_col < num_cols; index_col++) {

			obj->A[index_row * num_cols + index_col] = in_triu[index_element];
			obj->A[index_col * num_rows + index_row] = cplx_conj(in_triu[index_element++]);

		}

	}

	// A = L @ L^T
	cholesky_decompose_cplx(obj->L, obj->A, num_rowscols);

	// L > L^-1
	invert_tril_cplx(obj->Linv, obj->L, num_rowscols);

	// A^-1 = (L^-1)^T @ L^-1
	mult_tril_hermitian(obj->Ainv, obj->Linv, num_rowscols);

	index_element = 0;

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

		out_diag[index_row] = obj->Ainv[index_row * num_cols + index_row].real;

		for (unsigned int index_col = (index_row+1); index_col < num_cols; index_col++) {

			out_triu[index_element++] = obj->Ainv[index_row * num_cols + index_col];

		}

	}

	return 0;

}

static int cholesky_decompose_real(float * dst, const float * src, const unsigned int num_rowscols) {

	//
	// A = L @ L^T
	//
	// where:
	//
	//     +-                        -+             +-                        -+
	//     | a_11 a_12 a_13  ... a_1N |             | l_11   0    0   ...   0  |
	//     | a_21 a_22 a_23  ... a_2N |             | l_21 l_22   0   ...   0  |
	// A = | a_31 a_32 a_33  ... a_3N |   and   L = | l_31 l_32 l_33  ...   0 |
	//     |  ...  ...  ...  ...  ... |             |  ...  ...  ...  ...  ... |
	//     | a_N1 a_N2 a_N3  ... a_NN |             | l_N1 l_N2 l_N3  ... l_NN |
	//     +-                        -+             +-                        -+
	//

	unsigned int num_rows = num_rowscols;
	unsigned int num_cols = num_rowscols;
	memset(dst, 0x00, sizeof(float) * num_rowscols * num_rowscols);

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {
		
		for (unsigned int index_col = 0; index_col <= index_row; index_col++) {

			float sum = 0.0f;

			for (unsigned int index_element = 0; index_element < index_col; index_element++) {
				sum += dst[index_row*num_cols+index_element] * dst[index_col*num_cols+index_element];
			}

			if (index_row == index_col) {
				float value = src[index_row*num_cols+index_row] - sum;
				if (value <= 0.0f) {
					return -1;
				}
				else{
					dst[index_row*num_cols+index_col] = sqrtf(value);
				}
			}
			else {
				dst[index_row*num_cols+index_col] = (src[index_row*num_cols+index_col] - sum) / dst[index_col*num_cols+index_col];
			}

		}

	}

	return 0;

}

static int invert_tril_real(float * dst, const float * src, const unsigned int num_rowscols) {

	//
	// L' = L^-1
	//
	// where:
	//
	//      +-                             -+             +-                        -+
	//      | l'_11   0     0    ...    0   |             | l_11   0    0   ...   0  |
	//      | l'_21 l'_22   0    ...    0   |             | l_21 l_22   0   ...   0  |
	// L' = | l'_31 l'_32 l'_33  ...    0   |   and   L = | l_31 l_32 l_33  ...   0  |
	//      |  ...   ...   ...   ...   ...  |             |  ...  ...  ...  ...  ... |
	//      | l'_N1 l'_N2 l'_N3  ...  l'_NN |             | l_N1 l_N2 l_N3  ... l_NN |
	//      +-                             -+             +-                        -+
	//	

	unsigned int num_rows = num_rowscols;
	unsigned int num_cols = num_rowscols;
	memset(dst, 0x00, sizeof(float) * num_rowscols * num_rowscols);

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {
		dst[index_row*num_cols+index_row] = 1.0f / src[index_row*num_cols+index_row];
	}

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

		for (unsigned int index_col = 0; index_col < index_row; index_col++) {

			float sum = 0.0f;

			for (unsigned int index_element = index_col; index_element < index_row; index_element++) {
				sum += src[index_row*num_cols+index_element] * dst[index_element*num_cols+index_col];
			}

			dst[index_row*num_cols+index_col] = -sum / src[index_row*num_cols+index_row];

		}
	}

	return 0;

}

static int mult_tril_transpose(float * dst, const float * src, const unsigned int num_rowscols) {

	//
	// B = L^T @ L
	//
	// where:
	//
	//     +-                        -+           +-                        -+
	//     | l_11   0    0   ...   0  |           | b_11 b_12 b_13  ... b_1N |
	//     | l_21 l_22   0   ...   0  |           | b_21 b_22 b_23  ... b_2N |
	// L = | l_31 l_32 l_33  ...   0  |  and  B = | b_31 b_32 b_33  ... b_3N |
	//     |  ...  ...  ...  ...  ... |           |  ...  ...  ...  ...  ... |
	//     | l_N1 l_N2 l_N3  ... l_NN |           | b_N1 b_N2 b_N3  ... b_NN |
	//     +-                        -+           +-                        -+
	//

	unsigned int num_rows = num_rowscols;
	unsigned int num_cols = num_rowscols;
	unsigned int num_elements = num_rowscols;
	memset(dst, 0x00, sizeof(float) * num_rowscols * num_rowscols);

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

		for (unsigned int index_col = index_row; index_col < num_cols; index_col++) {

			float sum = 0.0f;

			for (unsigned int index_element = index_col; index_element < num_elements; index_element++) {
				sum += src[index_element*num_cols+index_row] * src[index_element*num_rows+index_col];
			}

			dst[index_row*num_cols+index_col] = sum;
			dst[index_col*num_rows+index_row] = sum;

		}

	}

	return 0;

}

static int cholesky_decompose_cplx(cplx_t * dst, const cplx_t * src, const unsigned int num_rowscols) {

	//
	// A = L @ L^H
	//
	// where:
	//
	//     +-                        -+             +-                        -+
	//     | a_11 a_12 a_13  ... a_1N |             | l_11   0    0   ...   0  |
	//     | a_21 a_22 a_23  ... a_2N |             | l_21 l_22   0   ...   0  |
	// A = | a_31 a_32 a_33  ... a_3N |   and   L = | l_31 l_32 l_33  ...   0 |
	//     |  ...  ...  ...  ...  ... |             |  ...  ...  ...  ...  ... |
	//     | a_N1 a_N2 a_N3  ... a_NN |             | l_N1 l_N2 l_N3  ... l_NN |
	//     +-                        -+             +-                        -+
	//

	unsigned int num_rows = num_rowscols;
	unsigned int num_cols = num_rowscols;
	memset(dst, 0x00, sizeof(float) * num_rowscols * num_rowscols);

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {
		
		for (unsigned int index_col = 0; index_col <= index_row; index_col++) {

			cplx_t sum = cplx_cst(0.0f, 0.0f);

			for (unsigned int index_element = 0; index_element < index_col; index_element++) {
				sum = cplx_add(sum, cplx_mul(dst[index_row*num_cols+index_element], cplx_conj(dst[index_col*num_cols+index_element])));
			}

			if (index_row == index_col) {
				float value = cplx_sub(src[index_row*num_cols+index_row], sum).real;
				if (value <= 0.0f) {
					return -1;
				}
				else{
					dst[index_row*num_cols+index_col] = cplx_cst(sqrtf(value), 0.0f);
				}
			}
			else {
				dst[index_row*num_cols+index_col] = cplx_div(cplx_sub(src[index_row*num_cols+index_col], sum), dst[index_col*num_cols+index_col]);
			}

		}

	}

	return 0;

}

static int invert_tril_cplx(cplx_t * dst, const cplx_t * src, const unsigned int num_rowscols) {

	//
	// L' = L^-1
	//
	// where:
	//
	//      +-                             -+             +-                        -+
	//      | l'_11   0     0    ...    0   |             | l_11   0    0   ...   0  |
	//      | l'_21 l'_22   0    ...    0   |             | l_21 l_22   0   ...   0  |
	// L' = | l'_31 l'_32 l'_33  ...    0   |   and   L = | l_31 l_32 l_33  ...   0  |
	//      |  ...   ...   ...   ...   ...  |             |  ...  ...  ...  ...  ... |
	//      | l'_N1 l'_N2 l'_N3  ...  l'_NN |             | l_N1 l_N2 l_N3  ... l_NN |
	//      +-                             -+             +-                        -+
	//	

	unsigned int num_rows = num_rowscols;
	unsigned int num_cols = num_rowscols;
	memset(dst, 0x00, sizeof(float) * num_rowscols * num_rowscols);

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {
		dst[index_row*num_cols+index_row] = cplx_cst(1.0f / src[index_row*num_cols+index_row].real, 0.0f);
	}

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

		for (unsigned int index_col = 0; index_col < index_row; index_col++) {

			cplx_t sum = cplx_cst(0.0f, 0.0f);

			for (unsigned int index_element = index_col; index_element < index_row; index_element++) {
				sum = cplx_add(sum, cplx_mul(src[index_row*num_cols+index_element], dst[index_element*num_cols+index_col]));
			}

			dst[index_row*num_cols+index_col] = cplx_mul(cplx_cst(-1.0f, 0.0f), cplx_div(sum, src[index_row*num_cols+index_row]));

		}
	}

	return 0;

}

static int mult_tril_hermitian(cplx_t * dst, const cplx_t * src, const unsigned int num_rowscols) {

	//
	// B = L^H @ L
	//
	// where:
	//
	//     +-                        -+           +-                        -+
	//     | l_11   0    0   ...   0  |           | b_11 b_12 b_13  ... b_1N |
	//     | l_21 l_22   0   ...   0  |           | b_21 b_22 b_23  ... b_2N |
	// L = | l_31 l_32 l_33  ...   0  |  and  B = | b_31 b_32 b_33  ... b_3N |
	//     |  ...  ...  ...  ...  ... |           |  ...  ...  ...  ...  ... |
	//     | l_N1 l_N2 l_N3  ... l_NN |           | b_N1 b_N2 b_N3  ... b_NN |
	//     +-                        -+           +-                        -+
	//

	unsigned int num_rows = num_rowscols;
	unsigned int num_cols = num_rowscols;
	unsigned int num_elements = num_rowscols;
	memset(dst, 0x00, sizeof(float) * num_rowscols * num_rowscols);

	for (unsigned int index_row = 0; index_row < num_rows; index_row++) {

		for (unsigned int index_col = index_row; index_col < num_cols; index_col++) {

			cplx_t sum = cplx_cst(0.0f, 0.0f);

			for (unsigned int index_element = index_col; index_element < num_elements; index_element++) {
				sum = cplx_add(sum, cplx_mul(src[index_element*num_cols+index_row], cplx_conj(src[index_element*num_rows+index_col])));
			}

			dst[index_row*num_cols+index_col] = cplx_conj(sum);
			dst[index_col*num_rows+index_row] = sum;

		}

	}

	return 0;

}