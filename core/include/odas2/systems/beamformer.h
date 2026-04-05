/**
 * @file    beamformer.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a beamformer system.
 *
 * This file contains the functions to construct, destroy and process
 * a beamformer system.
 */

#ifndef __BEAMFORMER_H
#define __BEAMFORMER_H

#include "../signals/freqs.h"
#include "../signals/weights.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    Beamformer.
 *
 * This structure holds the content of a beamformer system. This
 * applies the weights to each channel in the frequency domain
 * and sum the products.
 */
typedef struct beamformer_t {

    unsigned int num_sources;           /**< Number of sources. */
    unsigned int num_channels;          /**< Number of channels. */
    unsigned int num_bins;              /**< Number of bins in the frequency domain. */

} beamformer_t;

/**
 * @brief   Beamformer system constructor.
 *
 * This function allocates memory for a beamformer system.
 *
 * @param   num_sources             Number of sources
 * @param   num_channels            Number of channels
 * @param   num_bins                Number of bins in the frequency domain
 *
 * @return  A pointer to the structure with allocated memory.
 */
beamformer_t * beamformer_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins);

/**
 * @brief   Beamformer system destructor.
 *
 * This function frees memory allocated to a beamformer system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void beamformer_destroy(beamformer_t * obj);

/**
 * @brief   Beamformer system processing.
 * 
 * This function applies the beamformer weights to the multi-channel
 * input signals in the frequency domain, and returns the sum of these
 * products.
 * 
 * @param   obj                     Pointer to the beamformer system.
 * @param   freqs_in                Pointer to the input freqs signal.
 * @param   weights_in              Pointer to the input weights signal.
 * @param   freqs_out               Pointer to the output freqs signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int beamformer_process(beamformer_t * obj, const freqs_t * freqs_in, const weights_t * weights_in, freqs_t * freqs_out);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __BEAMFORMER_H
