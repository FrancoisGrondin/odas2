#ifndef __SST_H
#define __SST_H

#include "../types/xyz.h"
#include "../signals/doas.h"
#include "../utils/points.h"

typedef struct sst_t {

    unsigned int num_tracks;
    unsigned int num_directions;
    unsigned int num_pasts;

    float delta_time;
    float energy_threshold;

    float score_min;
    float energy_new_threshold;
    float energy_delete_threshold;
    float energy_decay;
    unsigned int id_counter;

    pot_t * pasts;
    pot_t * tracks;

} sst_t;

sst_t * sst_construct(const unsigned int num_tracks, const unsigned int num_directions, const float delta_time, const float energy_threshold);

void sst_destroy(sst_t * obj);

int sst_process(sst_t * obj, doas_t * in, doas_t * out);

void sst_printf(const sst_t * obj);

#endif // __SST_H