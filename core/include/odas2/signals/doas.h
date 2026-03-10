/**
 * @file    doas.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for an doa signal.
 *
 * This file contains the functions to construct, destroy and print
 * the content of a doas signal. It can also set a target doa.
 * Each direction has a specific type, a coordinate, and potentially some
 * energy or a tracking id.
 */

#ifndef __DOAS_H
#define __DOAS_H

#include "signal.h"
#include "../types/xyz.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Source type
 *
 * This enum holds the type of source.
 */
typedef enum {

    UNDEFINED,                      /**< Undefined source type. */
    POTENTIAL,                      /**< A potential source, usually generated after localisation. */
    TRACKED,                        /**< A tracked source, usually generated after tracking. */
    TARGET                          /**< A target source, usually defined by an external process. */

} src_t;

/**
 * @brief   Direction component.
 *
 * This structure holds the content of a direction. It has a type,
 * a xyz-coordinate, some energy (for localisation and tracking),
 * and an id for tracking.
 */
typedef struct dir_t {

    src_t type;                     /**< Type of source. */
    xyz_t coord;                    /**< xyz-coordinate. */
    float energy;                   /**< Energy level. */
    unsigned int tracking_id;       /**< Tracking ID. */

} dir_t;

/**
 * @brief   DoAs signal.
 *
 * This structure holds the content of a doas signal. The signal first
 * contains a label which is a string that can be used to provide a unique
 * id to this type of signal. This comes handy when results are exported
 * to a json file. The signal contains \p num_directions directions.
 */
typedef struct doas_t {

    char label[SIGNAL_LABEL_SIZE];  /**< Signal label. */

    unsigned int num_directions;    /**< Number of directions. */
    dir_t * dirs;                   /**< Array of directions. */

} doas_t;

/**
 * @brief   DoAs signal constructor.
 *
 * This function allocates memory for a DoAs signal.
 *
 * @param   label                   String that contains the unique name of this signal.
 * @param   num_directions          Number of directions.
 *
 * @return  A pointer to the structure with allocated memory.
 */
doas_t * doas_construct(const char * label, const unsigned int num_directions);

/**
 * @brief   DoAs signal destructor.
 *
 * This function frees memory allocated to a DoAs signal.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void doas_destroy(doas_t * obj);

/**
 * @brief   Set a specific target in DoAs.
 *
 * This function adds manually a direction to look at.
 *
 * @param   obj                     Pointer to the DoAs signal.
 */
void doas_target(doas_t * obj, const xyz_t * directions);

/**
 * @brief   Print formatted content of DoAs signal in file.
 *
 * Print the samples contained in each direction in floating point format that
 * to read easily in console or file the content of this signal.
 *
 * @param   obj                     Pointer to the DoAs signal.
 * @param   fp                      Pointer to the file where to print content.
 */
void doas_fprintf(const doas_t * obj, FILE * fp);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __DOAS_H
