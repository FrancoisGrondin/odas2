#ifndef __TENSORS
#define __TENSORS

typedef struct tensor {

    unsigned int num_dims1;
    unsigned int num_dims2;
    unsigned int num_dims3;
    unsigned int num_dims4;
    float * data;

} tensor;

tensor * tensor_construct(unsigned int num_dims1, unsigned int num_dims2, unsigned int num_dims3, unsigned int num_dims4);

void tensor_destroy(tensor * obj);

void tensor_zeros(tensor * obj);

void tensor_copy(tensor * dst, const tensor * src);

void tensor_load(tensor * obj, const float * data);

void tensor_save(const tensor * obj, float * data);

void tensor_load_1d(tensor * obj, const unsigned int index_dim1, const unsigned int index_dim2, const unsigned int index_dim3, const float * data);

void tensor_save_1d(const tensor * obj, const unsigned int index_dim1, const unsigned int index_dim2, const unsigned int index_dim3, float * data);

void tensor_printf(const tensor * obj);

int tensors_compare(const tensor * tensor1, const tensor * tensor2, const float eps);

#endif // __TENSORS