/**
 * @file    mvdr.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a mvdr system.
 *
 * This file contains the functions to construct, destroy and process
 * a minimum variance distortionless response beamformer.
 */

#ifndef __MVDR_H
#define __MVDR_H

#include "../signals/covs.h"
#include "../signals/freqs.h"
#include "../signals/weights.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    Minimum variance distortionless response
 *
 * This structure holds the content of a mvdr system. This
 * computes the weights to apply to each channel in the frequency
 * domain for beamforming.
 */
typedef struct mvdr_t {

    unsigned int num_channels;          /**< Number of channels. */
    unsigned int num_bins;              /**< Number of bins in the frequency domain. */

} mvdr_t;

/**
 * @brief   MVDR system constructor.
 *
 * This function allocates memory for a mvdr system.
 *
 * @param   num_channels            Number of channels
 * @param   num_bins                Number of bins in the frequency domain
 *
 * @return  A pointer to the structure with allocated memory.
 */
mvdr_t * mvdr_construct(const unsigned int num_channels, const unsigned int num_bins);

/**
 * @brief   MVDR system destructor.
 *
 * This function frees memory allocated to a delay and sum system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void mvdr_destroy(mvdr_t * obj);

/**
 * @brief   MVDR system processing.
 * 
 * This function generates the beamformer weights based on the target
 * spatial covariance matrices.
 * 
 * @param   obj                     Pointer to the mvdr system.
 * @param   covs                    Pointer to the target covs signal.
 * @param   weights                 Pointer to the output weights signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int mvdr_process(mvdr_t * obj, const covs_t * covs, weights_t * weights);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __MVDR_H
