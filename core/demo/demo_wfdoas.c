#include <odas2/ios/msg.h>
#include <odas2/ios/wav.h>
#include <odas2/signals/covs.h>
#include <odas2/signals/doas.h>
#include <odas2/signals/dsf.h>
#include <odas2/signals/freqs.h>
#include <odas2/signals/hops.h>
#include <odas2/signals/masks.h>
#include <odas2/signals/tdoas.h>
#include <odas2/systems/fcc.h>
#include <odas2/systems/gcc.h>
#include <odas2/systems/phat.h>
#include <odas2/systems/scm.h>
#include <odas2/systems/spinv.h>
#include <odas2/systems/spw.h>
#include <odas2/systems/ssl.h>
#include <odas2/systems/sst.h>
#include <odas2/systems/stft.h>
#include <odas2/utils/mics.h>
#include <odas2/utils/points.h>
#include <odas2/utils/error.h>

#include <string.h>

int main(int argc, char * argv[]) {

    //
    //                             Ms (all 1's)                                                       
    //                                  |                                                             
    //                                  *                                                             
    // +----+   xs   +------+   Xs   +-----+   XXs   +-----+   SSs   +------+   SSps   +-----+  tdoas   +-----+  doas   +-----+
    // | In | -----* | STFT | --+--* | SCM | ------* | SPW | ------* | PHAT | -------* | GCC | -------* | SSL | ------* | Out |
    // +----+        +------+   |    +-----+         +-----+         +------+          +-----+          +-----+         +-----+
    //                          |                       *
    //                          |                       | NNinvs
    //                          |                       |
    //                          |    +-----+  NNs   +-------+    
    //                          +--* | SCM | -----* | SPINV |
    //                               +-----+        +-------+
    //                                  *
    //                                  |
    //                             Ms (all 1's)

    //
    // Parameters
    //

    const unsigned int      num_channels      = 4;
    const unsigned int      num_shifts        = 128;
    const unsigned int      num_samples       = 512;
    const unsigned int      num_bins          = 257;
    const unsigned int      sample_rate       = 16000;
    const float             sound_speed       = 343.0f;
    const float             alpha_target      = 0.1f;    
    const float             alpha_noise       = 0.01f;
    const float             epsilon           = 0.00001f;
    const float             gamma             = 0.001f;
    const unsigned int      num_sources       = 1;
    const unsigned int      num_directions    = 1;
    const mics_hardware_t   micarray          = MICS_HARDWARE_RESPEAKER_USB_4;
    const points_geometry_t geometry          = POINTS_GEOMETRY_HALFSPHERE;
    const unsigned int      num_points        = 1000;

    //
    // Allocate memory
    //

    mics_t * mics = mics_construct(micarray); ODAS2_CHECK_PTR(mics);
    points_t * points = points_construct(geometry, num_points); ODAS2_CHECK_PTR(points);

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels, sample_rate); ODAS2_CHECK_PTR(wavin);

    hops_t * hops = hops_construct("xs", num_channels, num_shifts); ODAS2_CHECK_PTR(hops);
    freqs_t * freqs = freqs_construct("Xs", num_channels, num_bins); ODAS2_CHECK_PTR(freqs);
    masks_t * masks = masks_construct("Ms", num_channels, num_bins); ODAS2_CHECK_PTR(masks);
    covs_t * covs_target = covs_construct("XXs", num_channels, num_bins); ODAS2_CHECK_PTR(covs_target);
    covs_t * covs_noise = covs_construct("NNs", num_channels, num_bins); ODAS2_CHECK_PTR(covs_noise);
    covs_t * covs_noiseinv = covs_construct("NNinvs", num_channels, num_bins); ODAS2_CHECK_PTR(covs_noiseinv);
    covs_t * covs_transient = covs_construct("SSs", num_channels, num_bins); ODAS2_CHECK_PTR(covs_transient);
    covs_t * covs_phat = covs_construct("SSps", num_channels, num_bins); ODAS2_CHECK_PTR(covs_phat);
    tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources); ODAS2_CHECK_PTR(tdoas);
    doas_t * doas = doas_construct("doas", num_directions); ODAS2_CHECK_PTR(doas);

    stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, STFT_WINDOW_HANN); ODAS2_CHECK_PTR(stft);
    scm_t * scm_target = scm_construct(num_channels, num_bins, alpha_target); ODAS2_CHECK_PTR(scm_target);
    scm_t * scm_noise = scm_construct(num_channels, num_bins, alpha_noise); ODAS2_CHECK_PTR(scm_noise);
    spinv_t * spinv = spinv_construct(num_channels, num_bins, epsilon, gamma); ODAS2_CHECK_PTR(spinv);
    spw_t * spw = spw_construct(num_channels, num_bins); ODAS2_CHECK_PTR(spw);
    phat_t * phat = phat_construct(num_channels, num_bins); ODAS2_CHECK_PTR(phat);
    gcc_t * gcc = gcc_construct(num_sources, num_channels, num_bins); ODAS2_CHECK_PTR(gcc);
    ssl_t * ssl = ssl_construct(mics, points, (float)sample_rate, sound_speed, num_sources, num_directions); ODAS2_CHECK_PTR(ssl);

    msgout_t * msgout = msgout_construct("/dev/stdout"); ODAS2_CHECK_PTR(msgout);

    //
    // Process
    //

    masks_ones(masks);
    covs_eyes(covs_noiseinv);

    signed int frame_count = 0;
    const signed int noise_refresh = 100;

    while (wavin_read(wavin, hops) == 0) {

        ODAS2_CHECK_CODE(stft_process(stft, hops, freqs));
        ODAS2_CHECK_CODE(scm_process(scm_target, freqs, masks, covs_target));
        ODAS2_CHECK_CODE(scm_process(scm_noise, freqs, masks, covs_noise));
        
        if (frame_count == noise_refresh) {
            ODAS2_CHECK_CODE(spinv_process(spinv, covs_noise, covs_noiseinv));
        }
        
        ODAS2_CHECK_CODE(spw_process(spw, covs_target, covs_noiseinv, covs_transient));
        ODAS2_CHECK_CODE(phat_process(phat, covs_transient, covs_phat));
        ODAS2_CHECK_CODE(gcc_process(gcc, covs_phat, tdoas));

        ODAS2_CHECK_CODE(ssl_process(ssl, tdoas, doas, NULL));

        ODAS2_CHECK_CODE(msgout_write_doas(msgout, doas));

        frame_count++;

    }

    //
    // Free memory
    //

    mics_destroy(mics);
    points_destroy(points);

    wavin_destroy(wavin);

    hops_destroy(hops);
    freqs_destroy(freqs);
    masks_destroy(masks);
    covs_destroy(covs_target);
    covs_destroy(covs_noise);
    covs_destroy(covs_noiseinv);
    covs_destroy(covs_transient);
    covs_destroy(covs_phat);
    tdoas_destroy(tdoas);
    doas_destroy(doas);

    stft_destroy(stft);
    scm_destroy(scm_target);
    scm_destroy(scm_noise);
    phat_destroy(phat);
    gcc_destroy(gcc);
    ssl_destroy(ssl);

    msgout_destroy(msgout);

    return 0;

}
