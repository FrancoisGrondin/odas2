/**
 * @file    stft.h
 * @author  Francois Grondin <francois.grondin2@usherbrooke.ca>
 * @brief   Contains structure and basic functions for a stft/istft system.
 *
 * This file contains the functions to construct, destroy and process
 * a stft/istft system.
 */

#ifndef __STFT_H
#define __STFT_H

#include "../utils/fft.h"
#include "../signals/hops.h"
#include "../signals/freqs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Window type
 *
 * This enum holds the type of window.
 */
typedef enum stft_window_t {
    STFT_WINDOW_RECT,
    STFT_WINDOW_HANN,
    STFT_WINDOW_SINE
} stft_window_t;

/**
 * @brief    Short-Time Fourier Transform system.
 *
 * This structure holds the content of a STFT system. This computes
 * the forward Short-Time Fourier Transform with overlap.
 * Each frame is windowed (analysis) before the FFT.
 */
typedef struct stft_t {

    unsigned int num_channels;      /**< Number of channels. */
    unsigned int num_samples;       /**< Number of samples per frame in the time-domain. */
    unsigned int num_shifts;        /**< Number of samples between frames in the time-domain. */
    unsigned int num_bins;          /**< Number of bins in the frequency domain. */

    float * window;                 /**< Contains the window coefficients. */
    float ** frames;                /**< Points to the frame for each channel. */

    fft_t ** ffts;                  /**< Points to FFTs structures. */

    float ** frames_real;           /**< Points to the frame in the time domain. */
    cplx_t ** frames_cplx;          /**< Points to the frame in the frequency domain. */

} stft_t;

/**
 * @brief    Inverse Short-Time Fourier Transform system.
 *
 * This structure holds the content of a iSTFT system. This computes
 * the inverse Short-Time Fourier Transform with overlap-add.
 * Each frame is windowed (synthesis) after the iFFT, before overlap-add.
 */
typedef struct istft_t {

    unsigned int num_channels;      /**< Number of channels. */
    unsigned int num_samples;       /**< Number of samples per frame in the time-domain. */
    unsigned int num_shifts;        /**< Number of samples between frames in the time-domain. */
    unsigned int num_bins;          /**< Number of bins in the frequency domain. */

    float * window;                 /**< Contains the window coefficients. */
    float ** frames;                /**< Points to the frame for each channel. */

    fft_t ** ffts;                  /**< Points to FFTs structures. */

    float ** frames_real;           /**< Points to the frame in the time domain. */
    cplx_t ** frames_cplx;          /**< Points to the frame in the frequency domain. */

} istft_t;

/**
 * @brief   STFT system constructor.
 *
 * This function allocates memory for a stft system.
 *
 * @param   num_channels            Number of channels.
 * @param   num_samples             Number of samples per channel in each frame.
 * @param   num_shifts              Number of samples between adjacent frames.
 * @param   window                  Window type.
 *
 * @return  A pointer to the structure with allocated memory.
 */
stft_t * stft_construct(const unsigned int num_channels, const unsigned int num_samples, const unsigned int num_shifts, const stft_window_t window);

/**
 * @brief   STFT system destructor.
 *
 * This function frees memory allocated to a stft system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void stft_destroy(stft_t * obj);

/**
 * @brief   STFT system processing.
 * 
 * This function takes hops in the time-domain and generate spectra.
 * 
 * @param   obj                     Pointer to the stft system.
 * @param   hops                    Pointer to the input hops signal.
 * @param   freqs                   Pointer to the output freqs signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int stft_process(stft_t * obj, const hops_t * hops, freqs_t * freqs);

/**
 * @brief   iSTFT system constructor.
 *
 * This function allocates memory for a istft system.
 *
 * @param   num_channels            Number of channels.
 * @param   num_samples             Number of samples per channel in each frame.
 * @param   num_shifts              Number of samples between adjacent frames.
 * @param   window                  Window type.
 *
 * @return  A pointer to the structure with allocated memory.
 */
istft_t * istft_construct(const unsigned int num_channels, const unsigned int num_samples, const unsigned int num_shifts, const stft_window_t window);

/**
 * @brief   iSTFT system destructor.
 *
 * This function frees memory allocated to a istft system.
 *
 * @param   obj                     Pointer to the structure to be destroyed.
 */
void istft_destroy(istft_t * obj);

/**
 * @brief   iSTFT system processing.
 * 
 * This function takes spectra in the frequency-domain and generate hops.
 * 
 * @param   obj                     Pointer to the istft system.
 * @param   freqs                   Pointer to the input freqs signal.
 * @param   hops                    Pointer to the output hops signal.
 * 
 * @return  Returns 0 if no error, negative value otherwise.
 */
int istft_process(istft_t * obj, const freqs_t * freqs, hops_t * hops);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __STFT_H
