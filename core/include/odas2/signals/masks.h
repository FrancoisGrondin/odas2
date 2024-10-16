/**
 * @file    masks.h
 * @author  Francois Grondin
 * @brief   Contains structure and basic functions for a masks signal.
 * 
 * This file contains the functions to construct, destroy and print
 * the content of a masks signal. This signal holds the gains in the
 * frequency domain with float precision, and support multiple channels.
 * Usually we expect the masks to lie in the range [0,1].
 */

#ifndef __MASKS_H
#define __MASKS_H

#include <stdio.h>

/**
 * @brief   Masks signal.
 * 
 * This structure holds the content of a masks signal. The signal first
 * contains a label which is a string that can be used to provide a unique
 * id to this type of signal. This comes handy when results are exported
 * to a json file. The signal contains \p num_channels channels, each of which
 * contains a number of bins denoted by \p num_bins.
 */
typedef struct masks_t {

    char label[64];             /**< Signal label. Limited to 64 characters, including null character. */

    unsigned int num_channels;  /**< Number of channels. */
    unsigned int num_bins;      /**< Number of bins per channel. */

    float ** gains;             /**< Array of arrays of gains. */

} masks_t;

/**
 * @brief   Masks signal constructor.
 * 
 * This function allocates memory for a masks signal.
 * 
 * @param   label               String that contains the unique name of this signal (max 64 characters).
 * @param   num_channels        Number of channels.
 * @param   num_bins            Number of bins per channel in each frame.
 * 
 * @return  A pointer to the structure with allocated memory.
 */
masks_t * masks_construct(const char * label, const unsigned int num_channels, const unsigned int num_bins);

/**
 * @brief   Masks signal destructor.
 * 
 * This function frees memory allocated to a masks signal.
 * 
 * @param   obj                 Pointer to the structure to be destroyed.
 */
void masks_destroy(masks_t * obj);

/**
 * @brief   Set all gains to 1.0.
 * 
 * This function sets all the gains in a mask to 1.0.
 * 
 * @param   obj                 Pointer to the masks signal.
 */
void masks_ones(masks_t * obj);

/**
 * @brief   Set all gains to 0.0.
 * 
 * This function sets all the gains in a mask to 0.0.
 * 
 * @param   obj                 Pointer to the masks signal.
 */
void masks_zeros(masks_t * obj);

/**
 * @brief   Print formatted content of masks signal in file.
 * 
 * Print the samples contained in each channel in floating point format that
 * to read easily in console or file the content of this signal.
 * 
 * @param   obj                 Pointer to the masks signal.
 * @param   fp                  Pointer to the file where to print content.
 */
void masks_fprintf(const masks_t * obj, FILE * fp);


#endif // __MASKS_H