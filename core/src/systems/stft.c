#include <systems/stft.h>
#include <utils/pi.h>
#include <utils/openmp.h>
#include <utils/error.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

static float * window_hann(const unsigned int num_samples);
static float * window_sine(const unsigned int num_samples);

stft_t * stft_construct(const unsigned int num_channels, const unsigned int num_samples, const unsigned int num_shifts, const stft_window_t window) {

    if (num_channels < 1) {
        odas2_set_error_number(ODAS2_ERROR_STFT_CONSTRUCT_NUM_CHANNELS);
        return NULL;
    }
    if (num_samples < 2 || (num_samples & (num_samples - 1)) != 0) {
        odas2_set_error_number(ODAS2_ERROR_STFT_CONSTRUCT_NUM_SAMPLES);
        return NULL;
    }
    if (num_shifts < 1 || num_shifts > num_samples / 2) {
        odas2_set_error_number(ODAS2_ERROR_STFT_CONSTRUCT_NUM_SHIFTS);
        return NULL;
    }

    stft_t * obj = (stft_t *) malloc(sizeof(stft_t));

    obj->num_channels = num_channels;
    obj->num_samples = num_samples;
    obj->num_shifts = num_shifts;
    obj->num_bins = (num_samples / 2) + 1;

    obj->window = NULL;

    if (window == STFT_WINDOW_HANN) {
        obj->window = window_hann(num_samples);
    }
    else if (window == STFT_WINDOW_SINE) {
        obj->window = window_sine(num_samples);
    }
    else {
        odas2_set_error_number(ODAS2_ERROR_STFT_CONSTRUCT_INVALID_WINDOW);
        free(obj);
        return NULL;
    }

    obj->frames = (float **) malloc(sizeof(float *) * num_channels);
    for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
        obj->frames[index_channel] = (float *) calloc(sizeof(float), num_samples);
    }

    uint16_t thread_count = get_thread_count();

    obj->ffts = malloc(sizeof(fft_t *) * thread_count);
    obj->frames_real = malloc(sizeof(float *) * thread_count);
    obj->frames_cplx = malloc(sizeof(cplx_t *) * thread_count);
    for (unsigned int index_thread = 0; index_thread < thread_count; index_thread++) {
        obj->ffts[index_thread] = fft_construct(num_samples);
        obj->frames_real[index_thread] = (float *) calloc(sizeof(float), num_samples);
        obj->frames_cplx[index_thread] = (cplx_t *) calloc(sizeof(cplx_t), obj->num_bins);
    }

    return obj;

}

void stft_destroy(stft_t * obj) {

    for (unsigned int index_thread = 0; index_thread < get_thread_count(); index_thread++) {
        fft_destroy(obj->ffts[index_thread]);
        free(obj->frames_real[index_thread]);
        free(obj->frames_cplx[index_thread]);
    }
    free(obj->ffts);
    free(obj->frames_real);
    free(obj->frames_cplx);

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        free(obj->frames[index_channel]);
    }
    free(obj->frames);

    free(obj->window);

    free(obj);

}

int stft_process(stft_t * obj, const hops_t * hops, freqs_t * freqs) {

    if (obj->num_channels != hops->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_STFT_PROCESS_HOPS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_channels != freqs->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_STFT_PROCESS_FREQS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_shifts != hops->num_shifts) {
        odas2_set_error_number(ODAS2_ERROR_STFT_PROCESS_HOPS_NUM_SHIFTS);
        return -1;
    }
    if (obj->num_bins != freqs->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_STFT_PROCESS_FREQS_NUM_BINS);
        return -1;
    }

    // Process each channel individually
    #pragma omp parallel for num_threads(get_thread_count())
    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

        unsigned int index_thread = omp_get_thread_num();

        //
        // For the sake of explanation, let 8 be the number of samples, and 2 the number of shifts:
        //

        //
        // Shift the samples to the left
        //
        // Frame:  [  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  ]
        // Result: [  3  |  4  |  5  |  6  |  7  |  8  |  -  |  -  ]
        //
        memmove(&(obj->frames[index_channel][0]), &(obj->frames[index_channel][obj->num_shifts]), sizeof(float) * (obj->num_samples - obj->num_shifts));

        //
        // Copy the hop to the available space after shifting
        //
        // Hop:    [  A  |  B  ]
        // Result: [  3  |  4  |  5  |  6  |  7  |  8  |  A  |  B  ]
        //
        memcpy(&(obj->frames[index_channel][obj->num_samples - obj->num_shifts]), &(hops->samples[index_channel][0]), sizeof(float) * obj->num_shifts);

        //
        // Apply window
        //
        // Window: [  a  |  b  |  c  |  d  |  e  |  f  |  g  |  h  ]
        // Result: [ a*3 | b*4 | c*5 | d*6 | e*7 | f*8 | g*A | h*B ]
        //
        for (unsigned int index_sample = 0; index_sample < obj->num_samples; index_sample++) {
            obj->frames_real[index_thread][index_sample] = obj->window[index_sample] * obj->frames[index_channel][index_sample];
        }

        //
        // Perform FFT
        //
        // Frame:  [ a*3 | b*4 | c*5 | d*6 | e*7 | f*8 | g*A | h*B ]
        // Result: [  X  |  X  |  X  |  X  |  X  ]
        //
        fft_rfft(obj->ffts[omp_get_thread_num()], obj->frames_real[index_thread], obj->frames_cplx[index_thread]);

        //
        // Copy result to signal
        //
        memcpy(freqs->bins[index_channel], obj->frames_cplx[index_thread], sizeof(cplx_t) * obj->num_bins);

    }

    return 0;

}

istft_t * istft_construct(const unsigned int num_channels, const unsigned int num_samples, const unsigned int num_shifts, const stft_window_t window) {

    if (num_channels < 1) {
        odas2_set_error_number(ODAS2_ERROR_ISTFT_CONSTRUCT_NUM_CHANNELS);
        return NULL;
    }
    if (num_samples < 2 || (num_samples & (num_samples - 1)) != 0) {
        odas2_set_error_number(ODAS2_ERROR_ISTFT_CONSTRUCT_NUM_SAMPLES);
        return NULL;
    }
    if (num_shifts < 1 || num_shifts > num_samples / 2) {
        odas2_set_error_number(ODAS2_ERROR_ISTFT_CONSTRUCT_NUM_SHIFTS);
        return NULL;
    }

    istft_t * obj = (istft_t *) malloc(sizeof(istft_t));

    obj->num_channels = num_channels;
    obj->num_samples = num_samples;
    obj->num_shifts = num_shifts;
    obj->num_bins = (num_samples / 2) + 1;

    obj->window = NULL;

    if (window == STFT_WINDOW_HANN) {
        obj->window = window_hann(num_samples);
    }
    else if (window == STFT_WINDOW_SINE) {
        obj->window = window_sine(num_samples);
    }
    else {
        odas2_set_error_number(ODAS2_ERROR_ISTFT_CONSTRUCT_INVALID_WINDOW);
        free(obj);
        return NULL;
    }

    obj->frames = (float **) malloc(sizeof(float *) * num_channels);
    for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
        obj->frames[index_channel] = (float *) calloc(sizeof(float), num_samples);
    }

    uint16_t thread_count = get_thread_count();

    obj->ffts = (fft_t **) malloc(sizeof(fft_t *) * thread_count);
    obj->frames_real = (float **) malloc(sizeof(float *) * thread_count);
    obj->frames_cplx = (cplx_t **) malloc(sizeof(cplx_t *) * thread_count);
    for (unsigned int index_thread = 0; index_thread < thread_count; index_thread++) {
        obj->ffts[index_thread] = fft_construct(num_samples);
        obj->frames_real[index_thread] = (float *) calloc(sizeof(float), num_samples);
        obj->frames_cplx[index_thread] = (cplx_t *) calloc(sizeof(cplx_t), obj->num_bins);
    }

    return obj;

}

void istft_destroy(istft_t * obj) {

    for (unsigned int index_thread = 0; index_thread < get_thread_count(); index_thread++) {
        fft_destroy(obj->ffts[index_thread]);
        free(obj->frames_real[index_thread]);
        free(obj->frames_cplx[index_thread]);
    }
    free(obj->ffts);
    free(obj->frames_real);
    free(obj->frames_cplx);

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        free(obj->frames[index_channel]);
    }
    free(obj->frames);

    free(obj->window);

    free(obj);

}

int istft_process(istft_t * obj, const freqs_t * freqs, hops_t * hops) {

    if (obj->num_channels != hops->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_ISTFT_PROCESS_HOPS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_channels != freqs->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_ISTFT_PROCESS_FREQS_NUM_CHANNELS);
        return -1;
    }
    if (obj->num_shifts != hops->num_shifts) {
        odas2_set_error_number(ODAS2_ERROR_ISTFT_PROCESS_HOPS_NUM_SHIFTS);
        return -1;
    }
    if (obj->num_bins != freqs->num_bins) {
        odas2_set_error_number(ODAS2_ERROR_ISTFT_PROCESS_FREQS_NUM_BINS);
        return -1;
    }

    #pragma omp parallel for num_threads(get_thread_count())
    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

        unsigned int index_thread = omp_get_thread_num();

        //
        // For the sake of explanation, let 8 be the number of samples, and 2 the number of shifts:
        //

        //
        // Copy freq from signal
        //
        memcpy(obj->frames_cplx[index_thread], freqs->bins[index_channel], sizeof(cplx_t) * obj->num_bins);

        //
        // Perform inverse FFT
        //
        // Freq:   [  X  |  X  |  X  |  X  |  X  ]
        // Result: [  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  ]
        //
        fft_irfft(obj->ffts[index_thread], obj->frames_cplx[index_thread], obj->frames_real[index_thread]);

        //
        // Window and overlap-add this frame to prev frames
        //
        // Window: [  a  |  b  |  c  |  d  |  e  |  f  |  g  |  h  ]
        // Prev:   [  A  |  B  |  C  |  D  |  E  |  F  |  G  |  H  ]
        // Result: [a*1+A|b*2+B|c*3+C|d*4+D|e*5+E|f*6+F|g*7+G|h*8+H]
        //
        for (unsigned int index_sample = 0; index_sample < obj->num_samples; index_sample++) {
            obj->frames[index_channel][index_sample] += obj->frames_real[index_thread][index_sample] * obj->window[index_sample];
        }

        //
        // Extract hop
        //
        // Frame:  [a*1+A|b*2+B|c*3+C|d*4+D|e*5+E|f*6+F|g*7+G|h*8+H]
        // Result: [a*1+A|b*2+B]
        //
        memcpy(&(hops->samples[index_channel][0]), &(obj->frames[index_channel][0]), sizeof(float) * obj->num_shifts);

        //
        // Shift to the left
        //
        // Frame:  [a*1+A|b*2+B|c*3+C|d*4+D|e*5+E|f*6+F|g*7+G|h*8+H]
        // Result: [c*3+C|d*4+D|e*5+E|f*6+F|g*7+G|h*8+H|  -  |  -  ]
        //
        memmove(&(obj->frames[index_channel][0]), &(obj->frames[index_channel][obj->num_shifts]), sizeof(float) * (obj->num_samples - obj->num_shifts));

        //
        // Reset hop at the end to zero
        //
        // Frame:  [c*3+C|d*4+D|e*5+E|f*6+F|g*7+G|h*8+H|  -  |  -  ]
        // Result: [c*3+C|d*4+D|e*5+E|f*6+F|g*7+G|h*8+H|  0  |  0  ]
        //
        memset(&(obj->frames[index_channel][obj->num_samples - obj->num_shifts]), 0x00, sizeof(float) * obj->num_shifts);

    }

    return 0;

}

static float * window_hann(const unsigned int num_samples) {

    float * window = (float *) calloc(sizeof(float), num_samples);

    unsigned int N = num_samples - 1;
    for (unsigned int n = 0; n <= N; n++) {
        window[n] = 0.5f * (1.0f - cosf(2 * PI_F * n / N));
    }

    return window;

}

static float * window_sine(const unsigned int num_samples) {

    float * window = (float *) calloc(sizeof(float), num_samples);

    unsigned int N = num_samples - 1;
    for (unsigned int n = 0; n <= N; n++) {
        window[n] = sinf(PI_F * n / N);
    }

    return window;

}
