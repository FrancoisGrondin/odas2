#include <odas2/ios/msg.h>
#include <odas2/ios/wav.h>
#include <odas2/signals/covs.h>
#include <odas2/signals/freqs.h>
#include <odas2/signals/hops.h>
#include <odas2/signals/masks.h>
#include <odas2/signals/tdoas.h>
#include <odas2/systems/fcc.h>
#include <odas2/systems/gcc.h>
#include <odas2/systems/phat.h>
#include <odas2/systems/scm.h>
#include <odas2/systems/stft.h>
#include <odas2/utils/error.h>

#include <string.h>

int main(int argc, char * argv[]) {

    //
    //                             Ms (all 1's)
    //                                  |
    //                                  *
    // +----+   xs   +------+   Xs   +-----+   XXs   +------+   XXps   +---------+  tdoas   +-----+
    // | In | -----* | STFT | -----* | SCM | ------* | PHAT | -------* | GCC/FCC | -------* | Out |
    // +----+        +------+        +-----+         +------+          +---------+          +-----+
    //

    //
    // Parameters
    //

    const unsigned int  num_channels    = 4;
    const unsigned int  num_shifts      = 128;
    const unsigned int  num_samples     = 512;
    const unsigned int  num_bins        = 257;
    const unsigned int  sample_rate     = 16000;
    const float         alpha           = 0.5f;
    const unsigned int  num_sources     = 2;
    const char          method[]        = "gcc";

    //
    // Allocate memory
    //

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels, sample_rate);
    ODAS2_CHECK_PTR(wavin);

    hops_t * hops = hops_construct("xs", num_channels, num_shifts);
    ODAS2_CHECK_PTR(hops);
    freqs_t * freqs = freqs_construct("Xs", num_channels, num_bins);
    ODAS2_CHECK_PTR(freqs);
    masks_t * masks = masks_construct("Ms", num_channels, num_bins);
    ODAS2_CHECK_PTR(masks);
    covs_t * covs = covs_construct("XXs", num_channels, num_bins);
    ODAS2_CHECK_PTR(covs);
    covs_t * covs_phat = covs_construct("XXps", num_channels, num_bins);
    ODAS2_CHECK_PTR(covs_phat);
    tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);
    ODAS2_CHECK_PTR(tdoas);

    stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, STFT_WINDOW_HANN);
    ODAS2_CHECK_PTR(stft);
    scm_t * scm = scm_construct(num_channels, num_bins, alpha);
    ODAS2_CHECK_PTR(scm);
    phat_t * phat = phat_construct(num_channels, num_bins);
    ODAS2_CHECK_PTR(phat);
    fcc_t * fcc = fcc_construct(num_sources, num_channels, num_bins);
    ODAS2_CHECK_PTR(fcc);
    gcc_t * gcc = gcc_construct(num_sources, num_channels, num_bins);
    ODAS2_CHECK_PTR(gcc);

    msgout_t * msgout = msgout_construct("/dev/stdout");
    ODAS2_CHECK_PTR(msgout);

    //
    // Process
    //

    masks_ones(masks);

    while (wavin_read(wavin, hops) == 0) {

        ODAS2_CHECK_CODE(stft_process(stft, hops, freqs));
        ODAS2_CHECK_CODE(scm_process(scm, freqs, masks, covs));
        ODAS2_CHECK_CODE(phat_process(phat, covs, covs_phat));

        if (strcmp(method, "gcc") == 0) {
            ODAS2_CHECK_CODE(gcc_process(gcc, covs_phat, tdoas));
        }
        if (strcmp(method, "fcc") == 0) {
            ODAS2_CHECK_CODE(fcc_process(fcc, covs_phat, tdoas));
        }

        ODAS2_CHECK_CODE(msgout_write_tdoas(msgout, tdoas));

    }

    //
    // Free memory
    //

    wavin_destroy(wavin);

    hops_destroy(hops);
    freqs_destroy(freqs);
    masks_destroy(masks);
    covs_destroy(covs);
    covs_destroy(covs_phat);
    tdoas_destroy(tdoas);

    stft_destroy(stft);
    scm_destroy(scm);
    phat_destroy(phat);
    fcc_destroy(fcc);
    gcc_destroy(gcc);

    msgout_destroy(msgout);

    return 0;

}
