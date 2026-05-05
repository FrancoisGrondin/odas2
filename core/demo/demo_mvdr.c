#include <odas2/ios/wav.h>
#include <odas2/signals/covs.h>
#include <odas2/signals/freqs.h>
#include <odas2/signals/hops.h>
#include <odas2/signals/masks.h>
#include <odas2/signals/weights.h>
#include <odas2/systems/beamformer.h>
#include <odas2/systems/mvdr.h>
#include <odas2/systems/scm.h>
#include <odas2/systems/spinv.h>
#include <odas2/systems/spw.h>
#include <odas2/systems/stft.h>
#include <odas2/utils/error.h>

#include <string.h>

int main(int argc, char * argv[]) {

    //
    //                             Ms (all 1's)                                                       
    //                                  |                                                             
    //                                  *                                                             
    // +----+   xs   +------+   Xs   +-----+   XXs   +-----+   SSs   +------+   Ws    +-------+   Ys    +-------+   ys    +-----+
    // | In | -----* | STFT | --+--* | SCM | ------* | SPW | ------* | MVDR | ------* | Bfmer | ------* | iSTFT | ------* | Out |
    // +----+        +------+   |    +-----+         +-----+         +------+         +-------+         +-------+         +-----+
    //                          |                       *                                 *
    //                          |  Ms (all 1's)         | NNinvs                          |
    //                          |       |               |                                 |
    //                          |       *               |                                 |
    //                          |    +-----+  NNs   +-------+                             |
    //                          +--* | SCM | -----* | SPINV |                             |
    //                          |    +-----+        +-------+                             |
    //                          |                                                         |
    //                          +---------------------------------------------------------+       
    //                           

    //
    // Parameters
    //

    const unsigned int  num_channels    = 4;
    const unsigned int  num_shifts      = 128;
    const unsigned int  num_samples     = 512;
    const unsigned int  num_bins        = 257;
    const unsigned int  sample_rate     = 16000;
    const float         alpha           = 0.01f;
    const float         epsilon         = 1e-10f;
    const float         gamma           = 0.001f;    
    const unsigned int  num_sources     = 1;
    const unsigned int  num_frames_vad  = 100;

    //
    // Allocate memory
    //

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels, sample_rate); ODAS2_CHECK_PTR(wavin);

    hops_t * hops_in = hops_construct("xs", num_channels, num_shifts); ODAS2_CHECK_PTR(hops_in);
    freqs_t * freqs_in = freqs_construct("Xs", num_channels, num_bins); ODAS2_CHECK_PTR(freqs_in);
    masks_t * masks_target = masks_construct("MXs", num_channels, num_bins); ODAS2_CHECK_PTR(masks_target);
    masks_t * masks_noise = masks_construct("MNs", num_channels, num_bins); ODAS2_CHECK_PTR(masks_noise);
    covs_t * covs_target = covs_construct("XXs", num_channels, num_bins); ODAS2_CHECK_PTR(covs_target);
    covs_t * covs_noise = covs_construct("NNs", num_channels, num_bins); ODAS2_CHECK_PTR(covs_noise);
    covs_t * covs_noiseinv = covs_construct("NNinvs", num_channels, num_bins); ODAS2_CHECK_PTR(covs_noiseinv);
    covs_t * covs_whitened = covs_construct("SSs", num_channels, num_bins); ODAS2_CHECK_PTR(covs_whitened);
    weights_t * weights = weights_construct("Ws", num_sources, num_channels, num_bins); ODAS2_CHECK_PTR(weights);
    freqs_t * freqs_out = freqs_construct("Ys", num_sources, num_bins); ODAS2_CHECK_PTR(freqs_out);
    hops_t * hops_out = hops_construct("ys", num_sources, num_shifts); ODAS2_CHECK_PTR(hops_out);

    stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, STFT_WINDOW_HANN); ODAS2_CHECK_PTR(stft);
    scm_t * scm_target = scm_construct(num_channels, num_bins, alpha); ODAS2_CHECK_PTR(scm_target);
    scm_t * scm_noise = scm_construct(num_channels, num_bins, alpha); ODAS2_CHECK_PTR(scm_noise);
    spinv_t * spinv = spinv_construct(num_channels, num_bins, epsilon, gamma); ODAS2_CHECK_PTR(spinv);
    spw_t * spw = spw_construct(num_channels, num_bins); ODAS2_CHECK_PTR(spw);    
    mvdr_t * mvdr = mvdr_construct(num_channels, num_bins); ODAS2_CHECK_PTR(mvdr);
    beamformer_t * beamformer = beamformer_construct(num_sources, num_channels, num_bins); ODAS2_CHECK_PTR(beamformer);
    istft_t * istft = istft_construct(num_sources, num_samples, num_shifts, STFT_WINDOW_HANN); ODAS2_CHECK_PTR(istft);

    wavout_t * wavout = wavout_construct("/dev/stdout", num_shifts, num_sources, sample_rate); ODAS2_CHECK_PTR(wavout);

    //
    // Process
    //

    masks_ones(masks_target);
    masks_ones(masks_noise);
    covs_eyes(covs_noiseinv);

    unsigned int frame_count = 0;

    while (wavin_read(wavin, hops_in) == 0) {

        ODAS2_CHECK_CODE(stft_process(stft, hops_in, freqs_in));
        ODAS2_CHECK_CODE(scm_process(scm_target, freqs_in, masks_target, covs_target));
        ODAS2_CHECK_CODE(scm_process(scm_noise, freqs_in, masks_noise, covs_noise));

        if (frame_count == num_frames_vad) {
            ODAS2_CHECK_CODE(spinv_process(spinv, covs_noise, covs_noiseinv));
        }

        ODAS2_CHECK_CODE(spw_process(spw, covs_target, covs_noiseinv, covs_whitened));
        ODAS2_CHECK_CODE(mvdr_process(mvdr, covs_whitened, weights));
        ODAS2_CHECK_CODE(beamformer_process(beamformer, freqs_in, weights, freqs_out));
        ODAS2_CHECK_CODE(istft_process(istft, freqs_out, hops_out));

        ODAS2_CHECK_CODE(wavout_write(wavout, hops_out));

        frame_count++;

    }

    //
    // Free memory
    //

    wavin_destroy(wavin);

    hops_destroy(hops_in);
    freqs_destroy(freqs_in);
    masks_destroy(masks_target);
    masks_destroy(masks_noise);
    covs_destroy(covs_target);
    covs_destroy(covs_noise);
    covs_destroy(covs_noiseinv);
    covs_destroy(covs_whitened);
    weights_destroy(weights);
    freqs_destroy(freqs_out);
    hops_destroy(hops_out);

    stft_destroy(stft);
    scm_destroy(scm_target);
    scm_destroy(scm_noise);
    mvdr_destroy(mvdr);
    beamformer_destroy(beamformer);
    istft_destroy(istft);

    wavout_destroy(wavout);

	return 0;

}
