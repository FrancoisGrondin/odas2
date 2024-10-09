#include <systems/stft.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

stft_t * stft_construct(const unsigned int num_channels, const unsigned int num_samples, const unsigned int num_shifts, const unsigned int num_bins, const char * window) {

    stft_t * obj = (stft_t *) malloc(sizeof(stft_t));

    obj->num_channels = num_channels;
    obj->num_samples = num_samples;
    obj->num_shifts = num_shifts;
    obj->num_bins = num_bins;

    obj->window = NULL;

    if (strcmp(window, "hann") == 0) {
        obj->window = window_hann(num_samples);
    }
    if (strcmp(window, "sine") == 0) {
        obj->window = window_sine(num_samples);
    }

    obj->frames = (float **) malloc(sizeof(float *) * num_channels);
    for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
        obj->frames[index_channel] = (float *) calloc(sizeof(float), num_samples);
    }

    obj->fft = fft_construct(num_samples);

    obj->frame_real = (float *) calloc(sizeof(float), num_samples);
    obj->frame_cplx = (cplx_t *) calloc(sizeof(cplx_t), num_bins);

    return obj;

}

void stft_destroy(stft_t * obj) {

    free(obj->frame_real);
    free(obj->frame_cplx);

    fft_destroy(obj->fft);

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        free(obj->frames[index_channel]);
    }
    free(obj->frames);

    free(obj->window);

    free(obj);

}

int stft_process(stft_t * obj, const hops_t * hops, freqs_t * freqs) {

    // Process each channel individually
    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

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
            obj->frame_real[index_sample] = obj->window[index_sample] * obj->frames[index_channel][index_sample];
        }

        //
        // Perform FFT
        // 
        // Frame:  [ a*3 | b*4 | c*5 | d*6 | e*7 | f*8 | g*A | h*B ]
        // Result: [  X  |  X  |  X  |  X  |  X  ]
        //
        fft_rfft(obj->fft, obj->frame_real, obj->frame_cplx);

        //
        // Copy result to signal
        //
        memcpy(freqs->bins[index_channel], obj->frame_cplx, sizeof(cplx_t) * obj->num_bins);

    }

    return 0;

}

istft_t * istft_construct(const unsigned int num_channels, const unsigned int num_samples, const unsigned int num_shifts, const unsigned int num_bins, const char * window) {

    istft_t * obj = (istft_t *) malloc(sizeof(istft_t));

    obj->num_channels = num_channels;
    obj->num_samples = num_samples;
    obj->num_shifts = num_shifts;
    obj->num_bins = num_bins;
    
    obj->window = NULL;

    if (strcmp(window, "hann") == 0) {
        obj->window = window_hann(num_samples);
    }
    if (strcmp(window, "sine") == 0) {
        obj->window = window_sine(num_samples);
    }

    obj->frames = (float **) malloc(sizeof(float *) * num_channels);
    for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
        obj->frames[index_channel] = (float *) calloc(sizeof(float), num_samples);
    }

    obj->fft = fft_construct(num_samples);

    obj->frame_real = (float *) calloc(sizeof(float), num_samples);
    obj->frame_cplx = (cplx_t *) calloc(sizeof(cplx_t), num_bins);

    return obj;

}

void istft_destroy(istft_t * obj) {

    free(obj->frame_real);
    free(obj->frame_cplx);

    fft_destroy(obj->fft);

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        free(obj->frames[index_channel]);
    }
    free(obj->frames);

    free(obj->window);

    free(obj);    

}

int istft_process(istft_t * obj, const freqs_t * freqs, hops_t * hops) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

        //
        // For the sake of explanation, let 8 be the number of samples, and 2 the number of shifts:
        //

        //
        // Copy freq from signal
        //
        memcpy(obj->frame_cplx, freqs->bins[index_channel], sizeof(cplx_t) * obj->num_bins);

        //
        // Perform inverse FFT
        //
        // Freq:   [  X  |  X  |  X  |  X  |  X  ]
        // Result: [  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  ]
        //
        fft_irfft(obj->fft, obj->frame_cplx, obj->frame_real);

        //
        // Window and overlap-add this frame to prev frames
        //
        // Window: [  a  |  b  |  c  |  d  |  e  |  f  |  g  |  h  ]
        // Prev:   [  A  |  B  |  C  |  D  |  E  |  F  |  G  |  H  ]
        // Result: [a*1+A|b*2+B|c*3+C|d*4+D|e*5+E|f*6+F|g*7+G|h*8+H]
        //
        for (unsigned int index_sample = 0; index_sample < obj->num_samples; index_sample++) {
            obj->frames[index_channel][index_sample] += obj->frame_real[index_sample] * obj->window[index_sample];
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
        window[n] = 0.5f * (1.0f - cosf(2 * M_PI * n / N));
    }

    return window;

}

static float * window_sine(const unsigned int num_samples) {

    float * window = (float *) calloc(sizeof(float), num_samples);

    unsigned int N = num_samples - 1;
    for (unsigned int n = 0; n <= N; n++) {
        window[n] = sinf(M_PI * n / N);
    }

    return window;

}
