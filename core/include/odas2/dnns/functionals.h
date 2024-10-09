#ifndef __FUNCTIONALS
#define __FUNCTIONALS

#include "tensors.h"

int fcns_relu(const tensor * in, tensor * out);

int fcns_sigmoid(const tensor * in, tensor * out);

int fcns_softmax(const tensor * in, tensor * out);

#endif // __FUNCTIONALS