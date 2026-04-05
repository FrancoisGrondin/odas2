/**
 * @file    steering.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a steering system.
 *
 * This file contains the functions to construct, destroy and process
 * a steering system.
 */

#ifndef __STEERING_H
#define __STEERING_H

#include "../signals/doas.h"
#include "../signals/tdoas.h"
#include "../utils/mics.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    Steering.
 *
 * This structure holds the content of a steering system. This
 * computes the time difference of arrivals (TDoAs) between each
 * pair of microphones based on the microphone array geometry, 
 * the speed of sound and the sample rate for ideal free field
 * propagation, based on the provided directions of arrival (DoAs).
 */
typedef struct steering_t {

    unsigned int num_channels;      /**< Number of channels. */
    unsigned int num_pairs;         /**< Number of pairs of channels. */
    unsigned int num_sources;       /**< Number of sources. */

    const mics_t * mics;            /**< Array of microphones that describe geometry. */
    float sample_rate;              /**< Sample rate (samples/sec). */
    float sound_speed;              /**< Speed of sound (meters/sec). */

} steering_t;

/**
 * @brief   Steering system constructor.
 *
 * This function allocates memory for a steering system.
 *
 * @param   mics                    Array of microphones that describe the geometry
 * @param   sample_rate             Sample rate (samples/sec)
 * @param   sound_speed             Speed of sound (meters/sec)
 *
 * @return  A pointer to the structure with allocated memory.
 */
steering_t * steering_construct(const mics_t * mics, const float sample_rate, const float sound_speed, const unsigned int num_sources);

/**
 * @brief   Steering system destructor.
 *
 * This function frees memory allocated to an enhancement system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void steering_destroy(steering_t * obj);

/**
 * @brief   Steering system processing.
 * 
 * This function computes the TDoAs based on the DoAs.
 * 
 * @param   obj                     Pointer to the steering system.
 * @param   freqs                   Pointer to the input doas signal.
 * @param   masks                   Pointer to the output tdoas signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int steering_process(const steering_t * obj, const doas_t * doas, tdoas_t * tdoas);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __STEERING_H
