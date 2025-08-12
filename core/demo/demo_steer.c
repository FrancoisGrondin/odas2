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
#include <odas2/utils/error.h>

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

    const unsigned int    num_channels    = 4;
    const unsigned int    num_shifts      = 128;
    const unsigned int    num_samples     = 512;
    const unsigned int    num_bins        = 257;
    const unsigned int    sample_rate     = 16000;
    const unsigned int    num_sources     = 1;
    const float           sound_speed     = 343.0f;
    const mics_hardware_t micarray        = MICS_HARDWARE_RESPEAKER_USB_4;
    const xyz_t           targets[]       = { { .x = -0.704f, .y = -0.704f, .z = -0.088f } };

    //
    // Allocate memory
    //

    mics_t * mics = mics_construct(micarray);
    ODAS2_CHECK_PTR(mics);

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels, sample_rate);
    ODAS2_CHECK_PTR(wavin);
    doas_t * doas = doas_construct("doas", num_sources);
    ODAS2_CHECK_PTR(doas);

    hops_t * hops_in = hops_construct("xs", num_channels, num_shifts);
    ODAS2_CHECK_PTR(hops_in);
    freqs_t * freqs_in = freqs_construct("Xs", num_channels, num_bins);
    ODAS2_CHECK_PTR(freqs_in);
    tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);
    ODAS2_CHECK_PTR(tdoas);
    weights_t * weights = weights_construct("Ws", num_sources, num_channels, num_bins);
    ODAS2_CHECK_PTR(weights);
    freqs_t * freqs_out = freqs_construct("Ys", num_sources, num_bins);
    ODAS2_CHECK_PTR(freqs_out);
    hops_t * hops_out = hops_construct("ys", num_sources, num_shifts);
    ODAS2_CHECK_PTR(hops_out);

    stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, STFT_WINDOW_HANN);
    ODAS2_CHECK_PTR(stft);
    steering_t * steering = steering_construct(mics, (float)sample_rate, sound_speed, num_sources);
    ODAS2_CHECK_PTR(steering);
    delaysum_t * delaysum = delaysum_construct(num_sources, num_channels, num_bins);
    ODAS2_CHECK_PTR(delaysum);
    beamformer_t * beamformer = beamformer_construct(num_sources, num_channels, num_bins);
    ODAS2_CHECK_PTR(beamformer);
    istft_t * istft = istft_construct(num_sources, num_samples, num_shifts, STFT_WINDOW_HANN);
    ODAS2_CHECK_PTR(istft);

    wavout_t * wavout = wavout_construct("/dev/stdout", num_shifts, num_sources, sample_rate);
    ODAS2_CHECK_PTR(wavout);

    //
    // Process
    //

    while (wavin_read(wavin, hops_in) == 0) {

        doas_target(doas, targets);

        ODAS2_CHECK_CODE(stft_process(stft, hops_in, freqs_in));
        ODAS2_CHECK_CODE(steering_process(steering, doas, tdoas));
        ODAS2_CHECK_CODE(delaysum_process(delaysum, tdoas, weights));
        ODAS2_CHECK_CODE(beamformer_process(beamformer, freqs_in, weights, freqs_out));
        ODAS2_CHECK_CODE(istft_process(istft, freqs_out, hops_out));
        ODAS2_CHECK_CODE(wavout_write(wavout, hops_out));

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
