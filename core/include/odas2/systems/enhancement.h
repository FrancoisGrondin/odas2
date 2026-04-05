/**
 * @file    enhancement.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for time-frequency signal enhancement
 *
 * This file contains the functions to construct, destroy and process
 * an enhancement system.
 */

#ifndef __ENHANCEMENT_H
#define __ENHANCEMENT_H

#include "../dnns/functionals.h"
#include "../dnns/layers.h"
#include "../dnns/tensors.h"

#include "../signals/freqs.h"
#include "../signals/masks.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    Enhancement.
 *
 * This structure holds the content of an enhancement system. This
 * computes the frequency-wise mask to improve signal quality.
 */
typedef struct enhancement_t {

    unsigned int num_channels;          /**< Number of channels. */
    unsigned int num_bins;              /**< Number of frequency bins. */

    float beta;                         /**< Exponent of the input signal (|X|^beta). */
    ugru * rnn;                         /**< Pointer to the recurrent neural network (GRU). */
    linear * fc;                        /**< Pointer to the fully connecter (linear) layer. */

    float * spec;                       /**< Pointer to the spectrum amplitude. */
    float * mask;                       /**< Pointer to the mask. */

    tensor * x;                         /**< Input tensor (C, F), where C is the number of channels and F the number of frequency bins. */
    tensor * h;                         /**< Hidden state tensor (C, H), where C is the number of channels and H the dimension of hidden state. */
    tensor * y;                         /**< Output tensor (C, F) before sigmoid, where C is the number of channels and F the number of frequency bins. */
    tensor * m;                         /**< Output tensor (C, F) after sigmoid, where C is the number of channels and F the number of frequency bins. */

} enhancement_t;

/**
 * @brief   Enhancement system constructor.
 *
 * This function allocates memory for an enhancement system.
 *
 * @param   num_channels            Number of channels
 * @param   num_bins                Number of bins in the frequency domain
 *
 * @return  A pointer to the structure with allocated memory.
 */
enhancement_t * enhancement_construct(const unsigned int num_channels, const unsigned int num_bins);

/**
 * @brief   Enhancement system destructor.
 *
 * This function frees memory allocated to an enhancement system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void enhancement_destroy(enhancement_t * obj);

/**
 * @brief   Enhancement system processing.
 * 
 * This function computes the frequency  mask using a neural network.
 * 
 * @param   obj                     Pointer to the enhancement system.
 * @param   freqs                   Pointer to the input freqs signal.
 * @param   masks                   Pointer to the output masks signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int enhancement_process(enhancement_t * obj, const freqs_t * freqs, masks_t * masks);

static const unsigned int num_dims_in;
static const unsigned int num_dims_hidden;
static const unsigned int num_dims_out;
static const float beta;
static const ugru_params rnn_params;
static const linear_params fc_params;

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __ENHANCEMENT_H
