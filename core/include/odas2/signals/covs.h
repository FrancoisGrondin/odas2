/**
 * @file    covs.h
 * @author  Francois Grondin
 * @brief   Contains structure and basic functions for a covs signal.
 *
 * This file contains the functions to construct, destroy and print
 * the content of a covs signal. This signal holds the bins in the
 * frequency domain for hermitian matrices that represent the spatial
 * covariance matrices.
 */

#ifndef __COVS_H
#define __COVS_H

#include "signal.h"
#include "../types/cplx.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Covs signal.
 *
 * This structure holds the content of a covs signal. The signal first
 * contains a label which is a string that can be used to provide a unique
 * id to this type of signal. This comes handy when results are exported
 * to a json file. The signal contains \p num_channels autocorrelation vectors,
 * with \p num_bins elements each. This represents the diagonal terms of the
 * spatial covariance matrices, which are real numbers (given as floats). There
 * are \p num_pairs of channels, each holding \p num_bins for the cross-correlation
 * terms. These correspond to the upper triangle of each spatial covariance matrix.
 * The lower triangle can be ignored as it is implied from the upper triangle for
 * hermitian matrices.
 */
typedef struct covs_t {

    char label[SIGNAL_LABEL_SIZE];             /**< Signal label. Limited to 64 characters, including null character. */

    unsigned int num_channels;  /**< Number of channels. */
    unsigned int num_pairs;     /**< Number of pairs (number of channels * (number of channels - 1) / 2). */
    unsigned int num_bins;      /**< Number of frequency bins. */

    cplx_t * xcorrs_buffer;     /**< Array of arrays of complex numbers, for cross-correlation terms, for all pairs. */
    cplx_t ** xcorrs;           /**< Array of arrays of complex numbers, for cross-correlation terms. */
    float *  acorrs_buffer;     /**< Array of arrays of real numbers, for auto-correlation terms, for all channels. */
    float ** acorrs;            /**< Array of arrays of real numbers, for auto-correlation terms. */

} covs_t;

/**
 * @brief   Covs signal constructor.
 *
 * This function allocates memory for a covs signal.
 *
 * @param   label               String that contains the unique name of this signal (max 64 characters).
 * @param   num_channels        Number of channels.
 * @param   num_bins            Number of bins per channel in each frame.
 *
 * @return  A pointer to the structure with allocated memory.
 */
covs_t * covs_construct(const char * label, const unsigned int num_channels, const unsigned int num_bins);

/**
 * @brief   Covs signal destructor.
 *
 * This function frees memory allocated to a covs signal.
 *
 * @param   obj                 Pointer to the structure to be destroyed.
 */
void covs_destroy(covs_t * obj);

/**
 * @brief   Print formatted content of covs signal in file.
 *
 * Print the content of each spatial covariance matrix in floating
 * point format that to read easily in console or file the content
 * of this signal.
 *
 * @param   obj                 Pointer to the hops signal.
 * @param   fp                  Pointer to the file where to print content.
 */
void covs_fprintf(const covs_t * obj, FILE * fp);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __COVS_H
