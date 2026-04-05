/**
 * @file    sst.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a sst system.
 *
 * This file contains the functions to construct, destroy and process
 * a sst system.
 */

#ifndef __SST_H
#define __SST_H

#include "../types/xyz.h"
#include "../signals/doas.h"
#include "../signals/dsf.h"
#include "../utils/points.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   SST system.
 *
 * This structure holds the content of a sound source tracking system.
 * This will detect new source and track them until they become inactive.
 * This allows for continuous tracking for sparse observations from the
 * sound source localization system.
 */
typedef struct sst_t {

    unsigned int num_tracks;        /**< Number of tracked sources (maximum). */
    unsigned int num_directions;    /**< Number of potential sources from SSL. */
    unsigned int num_pasts;         /**< Number of potential sources from the past observations. */

    dir_t * pasts;                  /**< Array of pointers to past potential sources. */
    dir_t * tracks;                 /**< Array of pointers to tracked sources. */

    unsigned int last_tracking_id;  /**< Keep in memory the last tracking ID to provide unique ID. */

} sst_t;

/**
 * @brief   SST system constructor.
 *
 * This function allocates memory for a sst system.
 *
 * @param   num_tracks              Number of tracked sources (maximum).
 * @param   num_directions          Number of potential sources from SSL.
 * @param   num_pasts               Number of potential sources from the past observations.
 *
 * @return  A pointer to the structure with allocated memory.
 */
sst_t * sst_construct(const unsigned int num_tracks, const unsigned int num_directions, const unsigned int num_pasts);

/**
 * @brief   SST system destructor.
 *
 * This function frees memory allocated to a sst system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void sst_destroy(sst_t * obj);

/**
 * @brief   SST system processing.
 * 
 * This function updates the tracked sources, and can add/remove sources
 * if needed.
 * 
 * @param   obj                     Pointer to the sst system.
 * @param   dsf                     Pointer to the dsf signal with tracking parameters.
 * @param   in                      Pointer to the input doas signal.
 * @param   out                     Pointer to the output doas signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int sst_process(sst_t * obj, const dsf_t * dsf, const doas_t * doas_in, doas_t * doas_out);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __SST_H
