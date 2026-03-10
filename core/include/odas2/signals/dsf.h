/**
 * @file    dsf.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for an dsf signal.
 *
 * This file contains the functions to construct and destroy 
 * the content of a dsf (dynamic source filter) signal.
 */

#ifndef __DSF_H
#define __DSF_H

#include "signal.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Dynamic Source Filter signal.
 *
 * This structure holds the content of a dsf signal. The signal first
 * contains a label which is a string that can be used to provide a unique
 * id to this type of signal. This comes handy when results are exported
 * to a json file. The signal contains all parameters that dictates the
 * tracking behavior.
 */
typedef struct dsf_t {

	char label[SIGNAL_LABEL_SIZE];	/**< Signal label. */

	float sigmoid_mean;				/**< Energy decision threshold for a potential source to be tracked. */
	float sigmoid_slope;			/**< Slope of the sigmoid near the decision threshold. */
	float tracked_source_sigma2;	/**< Variance of the position distribution around the source current position. */
	float tracked_source_threshold; /**< Score threshold to assign a potential source to a previously tracked source. */
	float tracked_source_rate;		/**< Rate at which a tracked source is updated. */
	float new_source_sigma2;		/**< Variance of the new source distribution around the observed potential sources. */
    float new_threshold;			/**< Threshold to create a new source. */
    float delete_threshold;			/**< Threshold to delete an inactive source. */
    float delete_decay;				/**< Energy decay of an inactive source. */

} dsf_t;

/**
 * @brief   Dsf signal constructor.
 *
 * This function allocates memory for a Dsf signal.
 *
 * @param   label                   String that contains the unique name of this signal.
 *
 * @return  A pointer to the structure with allocated memory.
 */
dsf_t * dsf_construct(const char * label);

/**
 * @brief   Dsf signal destructor.
 *
 * This function frees memory allocated to a Dsf signal.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void dsf_destroy(dsf_t * obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __DSF_H
