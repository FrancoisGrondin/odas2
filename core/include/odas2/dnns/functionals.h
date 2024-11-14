#ifndef __FUNCTIONALS
#define __FUNCTIONALS

#include "tensors.h"

#ifdef __cplusplus
extern "C" {
#endif

int fcns_relu(const tensor * in, tensor * out);

int fcns_sigmoid(const tensor * in, tensor * out);

int fcns_softmax(const tensor * in, tensor * out);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __FUNCTIONALS
