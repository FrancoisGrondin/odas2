/**
 * @file    hops.h
 * @author  Francois Grondin
 * @brief   Contains structure and basic functions for a hops signal.
 * 
 * This file contains the functions to construct, destroy and print
 * the content of a hops signal. This signal holds the samples in the
 * time domain with float precision, and support multiple channels.
 */

#ifndef __HOPS_H
#define __HOPS_H

#include <stdio.h>

/**
 * @brief   Hops signal.
 * 
 * This structure holds the content of a hops signal. The signal first
 * contains a label which is a string that can be used to provide a unique
 * id to this type of signal. This comes handy when results are exported
 * to a json file. The signal contains \p num_channels channels, each of which
 * contains a number of samples denoted by \p num_shifts.
 */
typedef struct hops_t {

    char label[64];             /**< Signal label. Limited to 64 characters, including null character. */

    unsigned int num_channels;  /**< Number of channels. */
    unsigned int num_shifts;    /**< Number of samples per channel. */

    float ** samples;           /**< Array of arrays of samples. */

} hops_t;

/**
 * @brief   Hops signal constructor.
 * 
 * This function allocates memory for a hops signal.
 * 
 * @param   label               String that contains the unique name of this signal (max 64 characters).
 * @param   num_channels        Number of channels.
 * @param   num_shifts          Number of samples per channel in each frame.
 * 
 * @return  A pointer to the structure with allocated memory.
 */
hops_t * hops_construct(const char * label, const unsigned int num_channels, const unsigned int num_shifts);

/**
 * @brief   Hops signal destructor.
 * 
 * This function frees memory allocated to a hops signal.
 * 
 * @param   obj                 Pointer to the structure to be destroyed.
 */
void hops_destroy(hops_t * obj);

/**
 * @brief   Print formatted content of hops signal in file.
 * 
 * Print the samples contained in each channel in floating point format that
 * to read easily in console or file the content of this signal.
 * 
 * @param   obj                 Pointer to the hops signal.
 * @param   fp                  Pointer to the file where to print content.
 */
void hops_fprintf(const hops_t * obj, FILE * fp);

#endif // __HOPS_H