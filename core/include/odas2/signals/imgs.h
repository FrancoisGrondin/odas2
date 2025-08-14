/**
 * @file    imgs.h
 * @author  Francois Grondin
 * @brief   Contains structure and basic functions for an image signal.
 *
 * This file contains the functions to construct, destroy and print
 * the content of a hops signal. This signal holds the samples in the
 * time domain with float precision, and support multiple channels.
 */

#ifndef __IMGS_H
#define __IMGS_H

#include "signal.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Image signal.
 *
 * This structure holds the content of an image signal. The signal first
 * contains a label which is a string that can be used to provide a unique
 * id to this type of signal. This comes handy when results are exported
 * to a json file. The signal contains \p num_points points, each of which
 * has the acoustic energy at this direction.
 */
typedef struct imgs_t {

    char label[SIGNAL_LABEL_SIZE];             /**< Signal label. Limited to 64 characters, including null character. */

    unsigned int num_points;        /**< Number of points. */
    unsigned int num_directions;    /**< Number of scans. */

    float * energies_buffer;        /**< Arrays of energies, for all scans. */
    float ** energies;              /**< Arrays of energies, one for each scan. */

} imgs_t;

/**
 * @brief   Imgs signal constructor.
 *
 * This function allocates memory for a imgs signal.
 *
 * @param   label               String that contains the unique name of this signal (max 64 characters).
 * @param   num_points          Number of points.
 *
 * @return  A pointer to the structure with allocated memory.
 */
imgs_t * imgs_construct(const char * label, const unsigned int num_points, const unsigned int num_directions);

/**
 * @brief   Imgs signal destructor.
 *
 * This function frees memory allocated to a imgs signal.
 *
 * @param   obj                 Pointer to the structure to be destroyed.
 */
void imgs_destroy(imgs_t * obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __IMGS_H
