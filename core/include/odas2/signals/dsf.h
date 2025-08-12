#ifndef __DSF_H
#define __DSF_H

#include "signal.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dsf_t {

	char label[SIGNAL_LABEL_SIZE];

	float sigmoid_mean;
	float sigmoid_slope;
	float tracked_source_sigma2;
	float tracked_source_threshold;
	float tracked_source_rate;
	float new_source_sigma2;
    float new_threshold;
    float delete_threshold;
    float delete_decay;

} dsf_t;

dsf_t * dsf_construct(const char * label);

void dsf_destroy(dsf_t * obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __DSF_H
