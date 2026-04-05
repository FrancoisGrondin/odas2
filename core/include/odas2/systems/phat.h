/**
 * @file    phat.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a phat system.
 *
 * This file contains the functions to construct, destroy and process
 * a phase transform system.
 */

#ifndef __PHAT_H
#define __PHAT_H

#include "../signals/covs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    Phase Transform.
 *
 * This structure holds the content of a phat system. This
 * performs a phase transform on the cross-correlation terms
 * of spatial covariance matrices, and set auto-correlation
 * to unit values.
 */
typedef struct phat_t {

    unsigned int num_channels;      /**< Number of channels. */
    unsigned int num_pairs;         /**< Number of pairs. */
    unsigned int num_bins;          /**< Number of bins in the frequency domain. */

} phat_t;

/**
 * @brief   Phat system constructor.
 *
 * This function allocates memory for a phat system.
 *
 * @param   num_channels            Number of channels.
 * @param   num_bins                Number of bins in the frequency domain.
 *
 * @return  A pointer to the structure with allocated memory.
 */
phat_t * phat_construct(const unsigned int num_channels, const unsigned int num_bins);

/**
 * @brief   Phat system destructor.
 *
 * This function frees memory allocated to a phat system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void phat_destroy(phat_t * obj);

/**
 * @brief   Phat system processing.
 * 
 * This function takes cross-correlation values and normalize them by
 * dividing by magnitude. It sets auto-correlation values to one.
 * 
 * @param   obj                     Pointer to the phat system.
 * @param   covs_in                 Pointer to the input covs signal.
 * @param   covs_out                Pointer to the output covs signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int phat_process(phat_t * obj, const covs_t * covs_in, covs_t * covs_out);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __PHAT_H
