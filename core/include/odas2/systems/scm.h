/**
 * @file    scm.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a scm system.
 *
 * This file contains the functions to construct, destroy and process
 * a scm system.
 */

#ifndef __SCM_H
#define __SCM_H

#include "../signals/covs.h"
#include "../signals/freqs.h"
#include "../signals/masks.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    Spatial Covariance Matrices.
 *
 * This structure holds the content of a SCM system. This
 * computes the spatial covariance matrices online for each
 * frequency bin.
 */
typedef struct scm_t {

    unsigned int num_channels;      /**< Number of channels. */
    unsigned int num_pairs;         /**< Number of pairs. */
    unsigned int num_bins;          /**< Number of bins in the frequency domain. */
    float alpha;                    /**< Adaptation rate. */

    cplx_t ** xcorrs;               /**< Holds the cross-correlation frames. */
    float ** acorrs;                /**< Holds the auto-correlation frames. */

} scm_t;

/**
 * @brief   SCM system constructor.
 *
 * This function allocates memory for a scm system.
 *
 * @param   num_channels            Number of channels.
 * @param   num_bins                Number of bins in the frequency domain.
 * @param   alpha                   Adaptation rate.
 *
 * @return  A pointer to the structure with allocated memory.
 */
scm_t * scm_construct(const unsigned int num_channels, const unsigned int num_bins, const float alpha);

/**
 * @brief   SCM system destructor.
 *
 * This function frees memory allocated to a scm system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void scm_destroy(scm_t * obj);

/**
 * @brief   SCM system processing.
 * 
 * This function takes freqs in the frequency-domain and generate covariance matrices.
 * 
 * @param   obj                     Pointer to the scm system.
 * @param   freqs                   Pointer to the input freqs signal.
 * @param   masks                   Pointer to the input masks signal.
 * @param   covs                    Pointer to the output covs signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int scm_process(scm_t * obj, const freqs_t * freqs, const masks_t * masks, covs_t * covs);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __SCM_H
