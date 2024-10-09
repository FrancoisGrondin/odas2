#ifndef __MSG_H
#define __MSG_H

#include "../signals/covs.h"
#include "../signals/doas.h"
#include "../signals/freqs.h"
#include "../signals/hops.h"
#include "../signals/masks.h"
#include "../signals/tdoas.h"

#include <stdio.h>

typedef struct msgout_t {

    FILE * file_pointer;

} msgout_t;

msgout_t * msgout_construct(const char * file_name);

void msgout_destroy(msgout_t * obj);

int msgout_write_hops(msgout_t * obj, const hops_t * hops);

int msgout_write_freqs(msgout_t * obj, const freqs_t * freqs);

int msgout_write_covs(msgout_t * obj, const covs_t * covs);

int msgout_write_masks(msgout_t * obj, const masks_t * masks);

int msgout_write_doas(msgout_t * obj, const doas_t * doas);

int msgout_write_tdoas(msgout_t * obj, const tdoas_t * tdoas); 

#endif // __BIN_H