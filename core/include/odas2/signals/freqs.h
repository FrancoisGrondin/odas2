/**
 * @file    freqs.h
 * @author  Francois Grondin
 * @brief   Contains structure and basic functions for a freqs signal.
 * 
 * This file contains the functions to construct, destroy and print
 * the content of a freqs signal. This signal holds the samples in the
 * frequency domain with complex numbers, and support multiple channels.
 */

#ifndef __FREQS_H
#define __FREQS_H

#include "../types/cplx.h"
#include <stdio.h>

/**
 * @brief   Freqs signal.
 * 
 * This structure holds the content of a freqs signal. The signal first
 * contains a label which is a string that can be used to provide a unique
 * id to this type of signal. This comes handy when results are exported
 * to a json file. The signal contains \p num_channels channels, each of which
 * contains a number of bins denoted by \p num_bins.
 */
typedef struct freqs_t {

    char label[64];             /**< Signal label. Limited to 64 characters, including null character. */

    unsigned int num_channels;  /**< Number of channels. */
    unsigned int num_bins;      /**< Number of bins per channel. */

    cplx_t ** bins;             /**< Array of arrays of bins (complex numbers). */

} freqs_t;

/**
 * @brief   Freqs signal constructor.
 * 
 * This function allocates memory for a freqs signal.
 * 
 * @param   label               String that contains the unique name of this signal (max 64 characters).
 * @param   num_channels        Number of channels.
 * @param   num_bins            Number of bins per channel in each frame in the frequency domain.
 * 
 * @return  A pointer to the structure with allocated memory.
 */
freqs_t * freqs_construct(const char * label, const unsigned int num_channels, const unsigned int num_bins);

/**
 * @brief   Freqs signal destructor.
 * 
 * This function frees memory allocated to a freqs signal.
 * 
 * @param   obj                 Pointer to the structure to be destroyed.
 */
void freqs_destroy(freqs_t * obj);

/**
 * @brief   Print formatted content of freqs signal in file.
 * 
 * Print the bins contained in each channel in complex number format that
 * to read easily in console or file the content of this signal.
 * 
 * @param   obj                 Pointer to the freqs signal.
 * @param   fp                  Pointer to the file where to print content.
 */
void freqs_fprintf(const freqs_t * obj, FILE * fp);

#endif // __FREQS_H