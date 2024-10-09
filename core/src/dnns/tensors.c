#include <dnns/tensors.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tensor * tensor_construct(unsigned int num_dims1, unsigned int num_dims2, unsigned int num_dims3, unsigned int num_dims4) {

    tensor * obj = (tensor *) malloc(sizeof(tensor));

    obj->num_dims1 = num_dims1;
    obj->num_dims2 = num_dims2;
    obj->num_dims3 = num_dims3;
    obj->num_dims4 = num_dims4;

    obj->data = (float *) calloc(sizeof(float), num_dims1 * num_dims2 * num_dims3 * num_dims4);

    return obj;

}

void tensor_destroy(tensor * obj) {

    free((void *) obj->data);

    free((void *) obj);

}

void tensor_zeros(tensor * obj) {

    memset(obj->data, 0x00, sizeof(float) * obj->num_dims1 * obj->num_dims2 * obj->num_dims3 * obj->num_dims4);

}

void tensor_copy(tensor * dst, const tensor * src) {

    memcpy(dst->data, src->data, sizeof(float) * dst->num_dims1 * dst->num_dims2 * dst->num_dims3 * dst->num_dims4);

}

void tensor_load(tensor * obj, const float * data) {

    memcpy(obj->data, data, sizeof(float) * obj->num_dims1 * obj->num_dims2 * obj->num_dims3 * obj->num_dims4);

}

void tensor_save(const tensor * obj, float * data) {

    memcpy(data, obj->data, sizeof(float) * obj->num_dims1 * obj->num_dims2 * obj->num_dims3 * obj->num_dims4);

}

void tensor_load_1d(tensor * obj, const unsigned int index_dim1, const unsigned int index_dim2, const unsigned int index_dim3, const float * data) {

    const unsigned int index_dim = ((index_dim1 * obj->num_dims2 + index_dim2) * obj->num_dims3 + index_dim3) * obj->num_dims4;

    memcpy(&(obj->data[index_dim]), data, sizeof(float) * obj->num_dims4);

}

void tensor_save_1d(const tensor * obj, const unsigned int index_dim1, const unsigned int index_dim2, const unsigned int index_dim3, float * data) {

    const unsigned int index_dim = ((index_dim1 * obj->num_dims2 + index_dim2) * obj->num_dims3 + index_dim3) * obj->num_dims4;

    memcpy(data, &(obj->data[index_dim]), sizeof(float) * obj->num_dims4);

}

void tensor_printf(const tensor * obj) {

    for (unsigned int index_dim1 = 0; index_dim1 < obj->num_dims1; index_dim1++) {
        for (unsigned int index_dim2 = 0; index_dim2 < obj->num_dims2; index_dim2++) {
            for (unsigned int index_dim3 = 0; index_dim3 < obj->num_dims3; index_dim3++) {
                for (unsigned int index_dim4 = 0; index_dim4 < obj->num_dims4; index_dim4++) {
                    const unsigned int index_dim = ((index_dim1 * obj->num_dims2 + index_dim2) * obj->num_dims3 + index_dim3) * obj->num_dims4 + index_dim4;
                    printf("(%u, %u, %u, %u): %+f\n", index_dim1, index_dim2, index_dim3, index_dim4, obj->data[index_dim]);
                }
            }
        }
    }

}

int tensors_compare(const tensor * tensor1, const tensor * tensor2, const float eps) {

    const unsigned int tensor1_num_dims = tensor1->num_dims1 * tensor1->num_dims2 * tensor1->num_dims3 * tensor1->num_dims4;
    const unsigned int tensor2_num_dims = tensor2->num_dims1 * tensor2->num_dims2 * tensor2->num_dims3 * tensor2->num_dims4;

    if (tensor1_num_dims != tensor2_num_dims) {
        return -1;
    }

    const unsigned int num_dims = tensor1_num_dims;

    for (unsigned int index_dim = 0; index_dim < num_dims; index_dim++) {
        if (fabsf(tensor1->data[index_dim] - tensor2->data[index_dim]) > eps) {
            return -1;
        }
    }

    return 0;

}