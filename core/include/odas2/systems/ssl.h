/**
 * @file    ssl.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a ssl system.
 *
 * This file contains the functions to construct, destroy and process
 * a ssl system.
 */

#ifndef __SSL_H
#define __SSL_H

#include "../signals/doas.h"
#include "../signals/tdoas.h"
#include "../signals/imgs.h"
#include "../utils/mics.h"
#include "../utils/points.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Angle of arrival component.
 *
 * This structure hold the content of a degree on an arc
 * (from 0 to 180), and the corresponding amplitude.
 */
typedef struct aoa_t {

    float degree;
    float amplitude;

} aoa_t;

/**
 * @brief   SSL system.
 *
 * This structure holds the content of a sound source localization system.
 * This will scan the surface and find the directions of arrivals with the
 * most likely active sound sources. It can also generate acoustic images.
 */
typedef struct ssl_t {

    unsigned int num_channels;      /**< Number of channels. */
    unsigned int num_pairs;         /**< Number of pairs. */
    unsigned int num_sources;       /**< Number of sources. */
    unsigned int num_directions;    /**< Number of directions to scan. */
    unsigned int num_points;        /**< Number of points on the surface. */

    float sample_rate;              /**< Sample rate (in samples/sec). */
    float sound_speed;              /**< Speed of sound (in m/sec). */

    float * distances;              /**< Array that holds the distances (in m) between each pair of microphones. */
    aoa_t ** aoas;                  /**< Array that holds the angle of arrivals for each source (in the tdoas) and each pair. */

    const points_t * points;        /**< Pointer to the points that make the surface. */
    const mics_t * mics;            /**< Pointer to the microphones in the array. */
    unsigned char * table;          /**< Table to link each point and each pair to each angle of arrival. */
    float * norms;                  /**< Normalization factor to deal with pairs that are ignored. */

    unsigned int kernel_size;       /**< Size of kernel to spread the angle of arrival. */
    const float * kernel;           /**< Pointer to the array of numbers that makes the kernel. */

    float ** synthesis;             /**< Pointer to an array of numbers that represent the projected angle of arrivals with kernels. */

    float * projections;            /**< Pointer to an array of numbers that holds the energy for each point on the surface. */

} ssl_t;

/**
 * @brief   SSL system constructor.
 *
 * This function allocates memory for a ssl system.
 *
 * @param   mics                    Pointer to the microphones that make the microphone array.
 * @param   points                  Pointer to the points that make the surface.
 * @param   sample_rate             Sample rate (in samples/sec).
 * @param   sound_speed             Speed of sound (in m/sec).
 * @param   num_sources             Number of sources from the tdoas.
 * @param   num_directions          Number of scans to perform during localization.
 *
 * @return  A pointer to the structure with allocated memory.
 */
ssl_t * ssl_construct(const mics_t * mics, const points_t * points, const float sample_rate, const float sound_speed, const unsigned int num_sources, const unsigned int num_directions);

/**
 * @brief   SSL system destructor.
 *
 * This function frees memory allocated to a ssl system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void ssl_destroy(ssl_t * obj);

/**
 * @brief   SSL system processing.
 * 
 * This function scans the surface and computes the potential directions of arrival
 * for each direction. If the \p imgs pointer is not NULL, also generate the
 * acoustic image for each scan.
 * 
 * @param   obj                     Pointer to the ssl system.
 * @param   tdoas                   Pointer to the input tdoas signal.
 * @param   doas                    Pointer to the output doas signal.
 * @param   imgs                    Pointer to the output imgs signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int ssl_process(ssl_t * obj, const tdoas_t * tdoas, doas_t * doas, imgs_t * imgs);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __SSL_H
