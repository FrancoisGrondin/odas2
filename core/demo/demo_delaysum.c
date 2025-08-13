#include <odas2/ios/wav.h>
#include <odas2/signals/covs.h>
#include <odas2/signals/freqs.h>
#include <odas2/signals/hops.h>
#include <odas2/signals/masks.h>
#include <odas2/signals/tdoas.h>
#include <odas2/signals/weights.h>
#include <odas2/systems/beamformer.h>
#include <odas2/systems/delaysum.h>
#include <odas2/systems/fcc.h>
#include <odas2/systems/gcc.h>
#include <odas2/systems/phat.h>
#include <odas2/systems/scm.h>
#include <odas2/systems/stft.h>
#include <odas2/utils/mics.h>
#include <odas2/utils/points.h>
#include <odas2/utils/error.h>

#include <string.h>


int main(int argc, char * argv[]) {

    //
    //                            Ms (all 1's)
    //                                  |
    //                                  *
    // +----+   xs   +------+   Xs   +-----+   XXs   +------+   XXps   +---------+
    // | In | -----* | STFT | -----* | SCM | ------* | PHAT | -------* | GCC/FCC |
    // +----+        +------+   |    +-----+         +------+          +---------+
    //                          |                                           |
    //                          |             Ws      +----+      tdoas     |
    //                          |        +------------| DS | *--------------+
    //                          |        |            +----*
    //                          |        *
    //                          |    +-------+   Ys   +-------+   ys   +-----+
    //                          +--* | Bfmer | -----* | iSTFT | -----* | Out |
    //                               +-------+        +-------+        +-----+
    //

    //
    // Parameters
    //

    const unsigned int  num_channels    = 4;
    const unsigned int  num_shifts      = 128;
    const unsigned int  num_samples     = 512;
    const unsigned int  num_bins        = 257;
    const unsigned int  sample_rate     = 16000;
    const float         alpha           = 0.1f;
    const unsigned int  num_sources     = 1;
    const char          method[]        = "gcc";

    //
    // Allocate memory
    //

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels, sample_rate);
    ODAS2_CHECK_PTR(wavin);

    hops_t * hops_in = hops_construct("xs", num_channels, num_shifts);
    ODAS2_CHECK_PTR(hops_in);
    freqs_t * freqs_in = freqs_construct("Xs", num_channels, num_bins);
    ODAS2_CHECK_PTR(freqs_in);
    masks_t * masks = masks_construct("Ms", num_channels, num_bins);
    ODAS2_CHECK_PTR(masks);
    covs_t * covs = covs_construct("XXs", num_channels, num_bins);
    ODAS2_CHECK_PTR(covs);
    covs_t * covs_phat = covs_construct("XXps", num_channels, num_bins);
    ODAS2_CHECK_PTR(covs_phat);
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
    scm_t * scm = scm_construct(num_channels, num_bins, alpha);
    ODAS2_CHECK_PTR(scm);
    phat_t * phat = phat_construct(num_channels, num_bins);
    ODAS2_CHECK_PTR(phat);
    fcc_t * fcc = fcc_construct(num_sources, num_channels, num_bins);
    //ODAS2_CHECK_PTR(fcc);
    gcc_t * gcc = gcc_construct(num_sources, num_channels, num_bins);
    ODAS2_CHECK_PTR(gcc);
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

    masks_ones(masks);

    while (wavin_read(wavin, hops_in) == 0) {

        ODAS2_CHECK_CODE(stft_process(stft, hops_in, freqs_in));
        ODAS2_CHECK_CODE(scm_process(scm, freqs_in, masks, covs));
        ODAS2_CHECK_CODE(phat_process(phat, covs, covs_phat));

        if (strcmp(method, "gcc") == 0) {
            ODAS2_CHECK_CODE(gcc_process(gcc, covs_phat, tdoas));
        }
        if (strcmp(method, "fcc") == 0) {
            ODAS2_CHECK_CODE(fcc_process(fcc, covs_phat, tdoas));
        }

        ODAS2_CHECK_CODE(delaysum_process(delaysum, tdoas, weights));
        ODAS2_CHECK_CODE(beamformer_process(beamformer, freqs_in, weights, freqs_out));
        ODAS2_CHECK_CODE(istft_process(istft, freqs_out, hops_out));

        ODAS2_CHECK_CODE(wavout_write(wavout, hops_out));

    }

    //
    // Free memory
    //

    wavin_destroy(wavin);

    hops_destroy(hops_in);
    freqs_destroy(freqs_in);
    masks_destroy(masks);
    covs_destroy(covs);
    covs_destroy(covs_phat);
    tdoas_destroy(tdoas);
    weights_destroy(weights);
    freqs_destroy(freqs_out);
    hops_destroy(hops_out);

    stft_destroy(stft);
    scm_destroy(scm);
    phat_destroy(phat);
    fcc_destroy(fcc);
    gcc_destroy(gcc);
    delaysum_destroy(delaysum);
    beamformer_destroy(beamformer);
    istft_destroy(istft);

    wavout_destroy(wavout);

	return 0;

}
