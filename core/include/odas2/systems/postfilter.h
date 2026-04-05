/**
 * @file    enhancement.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for time-frequency signal enhancement
 *
 * This file contains the functions to construct, destroy and process
 * an postfilter system.
 */

#ifndef __POSTFILTER_H
#define __POSTFILTER_H

#include "../signals/freqs.h"
#include "../signals/masks.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    Enhancement.
 *
 * This structure holds the content of an postfilter system. This
 * applies the frequency-wise mask to improve signal quality.
 */
typedef struct postfilter_t {

    unsigned int num_channels;          /**< Number of channels. */
    unsigned int num_bins;              /**< Number of bins in the frequency domain. */

} postfilter_t;

/**
 * @brief   Postfiler system constructor.
 *
 * This function allocates memory for a postfilter system.
 *
 * @param   num_channels            Number of channels
 * @param   num_bins                Number of bins in the frequency domain
 *
 * @return  A pointer to the structure with allocated memory.
 */
postfilter_t * postfilter_construct(const unsigned int num_channels, const unsigned int num_bins);

/**
 * @brief   Postfiler system destructor.
 *
 * This function frees memory allocated to a postfilter system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void postfilter_destroy(postfilter_t * obj);

/**
 * @brief   Postfilter system processing.
 * 
 * This function applies the frequency mask for postfiltering.
 * 
 * @param   obj                     Pointer to the delay and sum system.
 * @param   freqs_in                Pointer to the input freqs signal.
 * @param   masks_in                Pointer to the input masks signal.
 * @param   masks_out               Pointer to the output freqs signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int postfilter_process(postfilter_t * obj, const freqs_t * freqs_in, const masks_t * masks_in, freqs_t * freqs_out);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __PHAT_H
