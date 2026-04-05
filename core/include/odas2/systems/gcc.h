/**
 * @file    gcc.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a gcc system.
 *
 * This file contains the functions to construct, destroy and process
 * a generalized cross-correlation operation.
 */

#ifndef __GCC_H
#define __GCC_H

#include "../utils/fft.h"
#include "../utils/openmp.h"
#include "../signals/covs.h"
#include "../signals/tdoas.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    Generalized Cross-Correlation.
 *
 * This structure holds the content of a GCC system. This
 * computes the generalized cross-correlation online for each
 * pair of channels.
 */
typedef struct gcc_t {

    unsigned int num_sources;           /**< Number of sources. */
    unsigned int num_channels;          /**< Number of channels. */
    unsigned int num_pairs;             /**< Number of pairs. */
    unsigned int num_bins;              /**< Number of bins in the frequency domain. */
    unsigned int num_samples;           /**< Number of samples per frame in the time-domain. */
    unsigned int interpolation_factor;  /**< Interpolation factor in the time-domain. */

    fft_t ** ffts;                      /**< Pointers to FFT operators. */

    cplx_t ** XXs;                      /**< Pointers to arrays of complex numbers. */
    float ** xxs;                       /**< Pointers to arrays of real numbers. */

} gcc_t;

/**
 * @brief   Gcc system constructor.
 *
 * This function allocates memory for a GCC system.
 *
 * @param   num_sources             Number of sources.
 * @param   num_channels            Number of channels.
 * @param   num_bins                Number of bins in the frequency domain.
 *
 * @return  A pointer to the structure with allocated memory.
 */
gcc_t * gcc_construct(const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins);

/**
 * @brief   Gcc system destructor.
 *
 * This function frees memory allocated to a GCC system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void gcc_destroy(gcc_t * obj);

/**
 * @brief   Gcc system processing.
 *
 * This function performs genralized cross-correlation. For each source, the
 * maximum value is extracted and corresponds to the TDoA, and then removed,
 * and the next maximum value is found, and so on. These values are stored in
 * the tdoas signal.
 *
 * @param   obj                     Pointer to the gcc system.
 * @param   covs                    Pointer to the input covs signals.
 * @param   tdoas                   Pointer to the output tdoas signals.
 */
int gcc_process(gcc_t * obj, const covs_t * covs, tdoas_t * tdoas);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __GCC_H
