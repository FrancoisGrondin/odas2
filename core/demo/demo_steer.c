#include <odas2/ios/wav.h>
#include <odas2/signals/doas.h>
#include <odas2/signals/freqs.h>
#include <odas2/signals/hops.h>
#include <odas2/signals/tdoas.h>
#include <odas2/systems/beamformer.h>
#include <odas2/systems/delaysum.h>
#include <odas2/systems/ssl.h>
#include <odas2/systems/steering.h>
#include <odas2/systems/stft.h>
#include <odas2/utils/mics.h>

int main(int argc, char * argv[]) {

    //                                                                                
    // +----+   xs   +------+   Xs   +------------+   Ys   +-------+   ys   +-----+
    // | In | -----* | STFT | -----* | Beamformer | -----* | iSTFT | -----* | Out |
    // +----+        +------+        +------------+        +-------+        +-----+
    //                                      *
    //                                      |
    //                                      +---+ Ws
    //                                          |
    //         doas  +----------+  tdoas   +----------+
    // targets ----* | Steering | -------* | DelaySum |
    //               +----------+          +----------+
    //

    //
    // Parameters
    //

    const unsigned int  num_channels    = 4;
    const unsigned int  num_shifts      = 128;
    const unsigned int  num_samples     = 512;
    const unsigned int  num_bins        = 257;
    const unsigned int  sample_rate     = 16000;
    const unsigned int  num_sources     = 1;
    const float         sound_speed     = 343.0f;
    const char          micarray[]      = "respeaker_usb";
    const xyz_t         targets[]       = { { .x = -0.704f, .y = -0.704f, .z = -0.088f } };

    //
    // Allocate memory
    //

    mics_t * mics = mics_construct(micarray);

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels, sample_rate);
    doas_t * doas = doas_construct("doas", num_sources);

    hops_t * hops_in = hops_construct("xs", num_channels, num_shifts); 
    freqs_t * freqs_in = freqs_construct("Xs", num_channels, num_bins);
    tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);
    weights_t * weights = weights_construct("Ws", num_sources, num_channels, num_bins);
    freqs_t * freqs_out = freqs_construct("Ys", num_sources, num_bins);
    hops_t * hops_out = hops_construct("ys", num_sources, num_shifts); 

    stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, num_bins, "hann");
    steering_t * steering = steering_construct(mics, sample_rate, sound_speed, num_sources);
    delaysum_t * delaysum = delaysum_construct(num_sources, num_channels, num_bins);
    beamformer_t * beamformer = beamformer_construct(num_sources, num_channels, num_bins);
    istft_t * istft = istft_construct(num_sources, num_samples, num_shifts, num_bins, "hann");

    wavout_t * wavout = wavout_construct("/dev/stdout", num_shifts, num_sources, sample_rate);

    //
    // Process
    //

    while (wavin_read(wavin, hops_in) == 0) {
        
        doas_target(doas, targets);

        stft_process(stft, hops_in, freqs_in);
        steering_process(steering, doas, tdoas);
        delaysum_process(delaysum, tdoas, weights);
        beamformer_process(beamformer, freqs_in, weights, freqs_out);
        istft_process(istft, freqs_out, hops_out);
        wavout_write(wavout, hops_out);

    }

    //
    // Free memory
    //

    mics_destroy(mics);

    wavin_destroy(wavin);
    doas_destroy(doas);

    hops_destroy(hops_in);
    freqs_destroy(freqs_in);
    tdoas_destroy(tdoas);
    weights_destroy(weights);
    freqs_destroy(freqs_out);
    hops_destroy(hops_out);

    stft_destroy(stft);
    steering_destroy(steering);
    delaysum_destroy(delaysum);
    beamformer_destroy(beamformer);
    istft_destroy(istft);

    wavout_destroy(wavout);

    return 0;


}