#ifndef __RAW_H
#define __RAW_H

#include "../signals/covs.h"
#include "../signals/doas.h"
#include "../signals/freqs.h"
#include "../signals/hops.h"
#include "../signals/masks.h"
#include "../signals/tdoas.h"

#include <stdio.h>

typedef struct rawout_t {

    FILE * file_pointer;

} rawout_t;

rawout_t * rawout_construct(const char * file_name);

void rawout_destroy(rawout_t * obj);

int rawout_write_hops(rawout_t * obj, const hops_t * hops);

int rawout_write_freqs(rawout_t * obj, const freqs_t * freqs);

int rawout_write_covs(rawout_t * obj, const covs_t * covs);

int rawout_write_masks(rawout_t * obj, const masks_t * masks);

int rawout_write_doas(rawout_t * obj, const doas_t * doas);

int rawout_write_tdoas(rawout_t * obj, const tdoas_t * tdoas); 

#endif // __RAW_H