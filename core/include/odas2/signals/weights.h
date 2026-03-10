/**
 * @file    weights.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a weights signal.
 *
 * This file contains the functions to construct, destroy and print
 * the content of a weights signal. This signal holds the samples in the
 * frequency domain with complex numbers, and support multiple channels.
 */

#ifndef __WEIGHTS_H
#define __WEIGHTS_H

#include "signal.h"
#include "../types/cplx.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Weights signal.
 *
 * This structure holds the content of a weights signal. The signal first
 * contains a label which is a string that can be used to provide a unique
 * id to this type of signal. This comes handy when results are exported
 * to a json file. The signal contains \p num_channels channels, each of which
 * contains a number of bins denoted by \p num_bins.
 */
typedef struct weights_t {

    char label[SIGNAL_LABEL_SIZE];  /**< Signal label. */

    unsigned int num_sources;       /**< Number of sources. */
    unsigned int num_channels;      /**< Number of channels. */
    unsigned int num_bins;          /**< Number of bins per channel. */

    cplx_t * buffer;                /**< Buffer that contains all bins, for all source and all channels. */
    cplx_t *** bins;                /**< Array of pointers. Each pointer links to the array of pointers for a specific source. Each pointer in these arrays points to bins for a specific channel.*/

} weights_t;

/**
 * @brief   Weights signal constructor.
 *
 * This function allocates memory for a weights signal.
 *
 * @param   label                   String that contains the unique name of this signal.
 * @param   num_sources             Number of sources.
 * @param   num_channels            Number of channels.
 * @param   num_bins                Number of bins per source and per channel in each frame in the frequency domain.
 *
 * @return  A pointer to the structure with allocated memory.
 */
weights_t * weights_construct(const char * label, const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins);

/**
 * @brief   Weights signal destructor.
 *
 * This function frees memory allocated to a weights signal.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void weights_destroy(weights_t * obj);

/**
 * @brief   Print formatted content of weights signal in file.
 *
 * Print the bins contained in each source and each channel in complex number format that
 * to read easily in console or file the content of this signal.
 *
 * @param   obj                     Pointer to the weights signal.
 * @param   fp                      Pointer to the file where to print content.
 */
void weights_fprintf(const weights_t * obj, FILE * fp);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __WEIGHTS_H
