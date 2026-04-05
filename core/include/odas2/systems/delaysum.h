/**
 * @file    delaysum.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a delay and sum system.
 *
 * This file contains the functions to construct, destroy and process
 * a delay and sum system.
 */

#ifndef __DELAYSUM_H
#define __DELAYSUM_H

#include "../signals/tdoas.h"
#include "../signals/weights.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    Delay and sum.
 *
 * This structure holds the content of a delay and sum system. This
 * computes the weights to apply to each channel in the frequency
 * domain for beamforming.
 */
typedef struct delaysum_t {

    unsigned int num_sources;           /**< Number of sources. */
    unsigned int num_channels;          /**< Number of channels. */
    unsigned int num_bins;              /**< Number of bins in the frequency domain. */

} delaysum_t;

/**
 * @brief   Delay and sum system constructor.
 *
 * This function allocates memory for a delay and sum system.
 *
 * @param   num_sources             Number of sources
 * @param   num_channels            Number of channels
 * @param   num_bins                Number of bins in the frequency domain
 *
 * @return  A pointer to the structure with allocated memory.
 */
delaysum_t * delaysum_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins);

/**
 * @brief   Delay and sum system destructor.
 *
 * This function frees memory allocated to a delay and sum system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void delaysum_destroy(delaysum_t * obj);

/**
 * @brief   Delay and sum system processing.
 * 
 * This function generates the beamformer weights based on the input
 * tdoas, assuming a freefield propagation model.
 * 
 * @param   obj                     Pointer to the delay and sum system.
 * @param   tdoas                   Pointer to the input tdoas signal.
 * @param   weights                 Pointer to the output weights signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int delaysum_process(delaysum_t * obj, const tdoas_t * tdoas, weights_t * weights);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __DELAYSUM_H
