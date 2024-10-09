#include <odas2/ios/wav.h>
#include <odas2/signals/covs.h>
#include <odas2/signals/freqs.h>
#include <odas2/signals/hops.h>
#include <odas2/signals/masks.h>
#include <odas2/signals/weights.h>
#include <odas2/systems/beamformer.h>
#include <odas2/systems/mvdr.h>
#include <odas2/systems/scm.h>
#include <odas2/systems/stft.h>

#include <string.h>

int main(int argc, char * argv[]) {

    //
    // Parameters
    //

    const unsigned int  num_channels    = 4;
    const unsigned int  num_shifts      = 128;
    const unsigned int  num_samples     = 512;
    const unsigned int  num_bins        = 257;
    const unsigned int  sample_rate     = 16000;
    const float         alpha           = 0.001;
    const unsigned int  num_sources     = 1;

    //
    // Pipeline
    //
    //                            Ms (all 1's)
    //                                  |
    //                                  * 
    // +----+   xs   +------+   Xs   +-----+   XXs   +------+   Ws   +-------+   Ys   +-------+   ys   +-----+
    // | In | -----* | STFT | -----* | SCM | ------* | MVDR | -----* | Bfmer | -----* | iSTFT | -----* | Out |
    // +----+        +------+   |    +-----+         +------+        +-------+        +-------+        +-----+
    //                          |                                        *
    //                          |                                        |
    //                          +----------------------------------------+
    //
    
    //
    // Allocate memory
    //

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels, sample_rate);

    hops_t * hops_in = hops_construct("xs", num_channels, num_shifts); 
    freqs_t * freqs_in = freqs_construct("Xs", num_channels, num_bins);
    masks_t * masks = masks_construct("Ms", num_channels, num_bins);
    covs_t * covs = covs_construct("XXs", num_channels, num_bins);
    weights_t * weights = weights_construct("Ws", num_sources, num_channels, num_bins);
    freqs_t * freqs_out = freqs_construct("Ys", num_sources, num_bins);
    hops_t * hops_out = hops_construct("ys", num_sources, num_shifts);

    stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, num_bins, "hann");
    scm_t * scm = scm_construct(num_channels, num_bins, alpha);
    mvdr_t * mvdr = mvdr_construct(num_channels, num_bins);
    beamformer_t * beamformer = beamformer_construct(num_sources, num_channels, num_bins);
    istft_t * istft = istft_construct(num_sources, num_samples, num_shifts, num_bins, "hann");

    wavout_t * wavout = wavout_construct("/dev/stdout", num_shifts, num_sources, sample_rate);	

    //
    // Process
    //

    masks_ones(masks); 

    while (wavin_read(wavin, hops_in) == 0) {
        
        stft_process(stft, hops_in, freqs_in);
        scm_process(scm, freqs_in, masks, covs);
        mvdr_process(mvdr, covs, weights);
        beamformer_process(beamformer, freqs_in, weights, freqs_out);
        istft_process(istft, freqs_out, hops_out);

        wavout_write(wavout, hops_out);

    }

    //
    // Free memory
    //

    wavin_destroy(wavin);

    hops_destroy(hops_in);
    freqs_destroy(freqs_in);
    masks_destroy(masks);
    covs_destroy(covs);
    weights_destroy(weights);
    freqs_destroy(freqs_out);
    hops_destroy(hops_out);

    stft_destroy(stft);
    scm_destroy(scm);
    mvdr_destroy(mvdr);
    beamformer_destroy(beamformer);
    istft_destroy(istft);

    wavout_destroy(wavout);

	return 0;

}