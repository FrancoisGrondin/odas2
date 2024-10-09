#ifndef __BEAMFORMER_H
#define __BEAMFORMER_H

#include "../signals/freqs.h"
#include "../signals/weights.h"

typedef struct beamformer_t {

    unsigned int num_sources;
    unsigned int num_channels;
    unsigned int num_bins;

} beamformer_t;

beamformer_t * beamformer_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins);

void beamformer_destroy(beamformer_t * obj);

int beamformer_process(beamformer_t * obj, const freqs_t * in, const weights_t * weights, freqs_t * out);

#endif // __BEAMFORMER_H