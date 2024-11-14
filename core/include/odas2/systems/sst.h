#ifndef __SST_H
#define __SST_H

#include "../types/xyz.h"
#include "../signals/doas.h"
#include "../signals/dsf.h"
#include "../utils/points.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct sst_t {

    unsigned int num_tracks;
    unsigned int num_directions;
    unsigned int num_pasts;

    dir_t * pasts;
    dir_t * tracks;

} sst_t;

sst_t * sst_construct(const unsigned int num_tracks, const unsigned int num_directions, const unsigned int num_pasts);

void sst_destroy(sst_t * obj);

int sst_process(sst_t * obj, const dsf_t * dsf, const doas_t * in, doas_t * out);

void sst_printf(const sst_t * obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __SST_H
