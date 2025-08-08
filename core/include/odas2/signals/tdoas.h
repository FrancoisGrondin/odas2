/**
 * @file    tdoas.h
 * @author  Francois Grondin
 * @brief   Contains structure and basic functions for a tdoas signal.
 *
 * This file contains the functions to construct, destroy and print
 * the content of a tdoas signal. This signal holds the delays and in 
 * the amplitudes with float precision.
 */

#ifndef __TDOAS_H
#define __TDOAS_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Tau component.
 * 
 * This structure hold the content of a delay, which corresponds to a
 * delay (in fractional number of samples) and an amplitude.
 */
typedef struct tau_t {

    float delay;
    float amplitude;

} tau_t;

/**
 * @brief   TDoAs signal.
 *
 * This structure holds the content of a tdoas signal. The signal first
 * contains a label which is a string that can be used to provide a unique
 * id to this type of signal. This comes handy when results are exported
 * to a json file. The signal contains \p num_sources sources, \p num_channels 
 * channels, each of which contains a number of pairs denoted by \p num_pairs.
 */
typedef struct tdoas_t {

    char label[64];

    unsigned int num_sources;   /**< Signal label. Limited to 64 characters, including null character. */
    unsigned int num_channels;  /**< Number of channels. */
    unsigned int num_pairs;     /**< Number of pairs. */

    tau_t ** taus;              /**< Structures that contain the delay and amplitude for TDoAs. */

} tdoas_t;

/**
 * @brief   TDoAs signal constructor.
 *
 * This function allocates memory for a TDoAs signal.
 *
 * @param   label               String that contains the unique name of this signal (max 64 characters).
 * @param   num_channels        Number of channels.
 * @param   num_sources         Number of sources.
 * @param   num_shifts          Number of samples per channel in each frame.
 *
 * @return  A pointer to the structure with allocated memory.
 */
tdoas_t * tdoas_construct(const char * label, const unsigned int num_channels, const unsigned int num_sources);

/**
 * @brief   TDoAs signal destructor.
 *
 * This function frees memory allocated to a TDoAs signal.
 *
 * @param   obj                 Pointer to the structure to be destroyed.
 */
void tdoas_destroy(tdoas_t * obj);

/**
 * @brief   Print formatted content of TDoAs signal in file.
 *
 * Print the samples contained in each channel in floating point format that
 * to read easily in console or file the content of this signal.
 *
 * @param   obj                 Pointer to the TDoAs signal.
 * @param   fp                  Pointer to the file where to print content.
 */
void tdoas_fprintf(const tdoas_t * obj, FILE * fp);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __TDOAS_H
