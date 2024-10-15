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

    hops_t * hops = hops_construct("xs", num_channels, num_shifts); 
    freqs_t * freqs = freqs_construct("Xs", num_channels, num_bins);
    masks_t * masks = masks_construct("Ms", num_channels, num_bins);
    covs_t * covs = covs_construct("XXs", num_channels, num_bins);
    covs_t * covs_phat = covs_construct("XXps", num_channels, num_bins);
    tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);

    stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, num_bins, "hann");
    scm_t * scm = scm_construct(num_channels, num_bins, alpha);
    phat_t * phat = phat_construct(num_channels, num_bins);
    fcc_t * fcc = fcc_construct(num_sources, num_channels, num_bins);
    gcc_t * gcc = gcc_construct(num_sources, num_channels, num_bins);    

    msgout_t * msgout = msgout_construct("/dev/stdout");

    //
    // Process
    //

    masks_ones(masks); 

    while (wavin_read(wavin, hops) == 0) {
        
        stft_process(stft, hops, freqs);
        scm_process(scm, freqs, masks, covs);    
        phat_process(phat, covs, covs_phat);

        if (strcmp(method, "gcc") == 0) {
            gcc_process(gcc, covs_phat, tdoas);
        }
        if (strcmp(method, "fcc") == 0) {
            fcc_process(fcc, covs_phat, tdoas);
        }

        msgout_write_tdoas(msgout, tdoas);

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