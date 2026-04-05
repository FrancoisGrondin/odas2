/**
 * @file    mixer.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a mixer system.
 *
 * This file contains the functions to construct, destroy and process
 * a mixer system.
 */

#ifndef __MIXER_H
#define __MIXER_H

#include "../signals/hops.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    Mixer.
 *
 * This structure holds the content of a mixer system. This
 * remixes the channels according to a given mapping.
 */
typedef struct mixer_t {

    unsigned int * map;                 /**< Mapping of signals. Indexes start at 0. */
    unsigned int num_channels;          /**< Number of output channels. */
    unsigned int max_map_index;         /**< Maximum index of the input channel. */

} mixer_t;

/**
 * @brief   Mixer system constructor.
 *
 * This function allocates memory for a mixer system.
 *
 * @param   channels                String with the channel mapping.
 *
 * @return  A pointer to the structure with allocated memory.
 */
mixer_t * mixer_construct(const char * channels);

/**
 * @brief   Mixer system destructor.
 *
 * This function frees memory allocated to a mixer system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void mixer_destroy(mixer_t * obj);

/**
 * @brief   Mixer system processing.
 * 
 * This function mixes the input signals in the time-domain to the
 * output signals according to a specific mapping.
 * 
 * @param   obj                     Pointer to the mixer system.
 * @param   hops_in                 Pointer to the input hops signal.
 * @param   hops_out                Pointer to the output hops signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int mixer_process(mixer_t * obj, const hops_t * hops_in, hops_t * hops_out);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __MIXER_H
