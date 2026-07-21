#include <odas2/ios/buf.h>
#include <odas2/ios/msg.h>
#include <odas2/ios/wav.h>
#include <odas2/signals/covs.h>
#include <odas2/signals/doas.h>
#include <odas2/signals/dsf.h>
#include <odas2/signals/freqs.h>
#include <odas2/signals/hops.h>
#include <odas2/signals/masks.h>
#include <odas2/signals/tdoas.h>
#include <odas2/signals/weights.h>
#include <odas2/systems/beamformer.h>
#include <odas2/systems/delaysum.h>
#include <odas2/systems/gcc.h>
#include <odas2/systems/phat.h>
#include <odas2/systems/scm.h>
#include <odas2/systems/ssl.h>
#include <odas2/systems/sst.h>
#include <odas2/systems/steering.h>
#include <odas2/systems/stft.h>
#include <odas2/utils/mics.h>
#include <odas2/utils/points.h>
#include <odas2/utils/error.h>

#include <string.h>

int main(int argc, char * argv[]) {

    //
    //                             Ms (all 1's)                                                       dsf (default values)
    //                                  |                                                                      |
    //                                  *                                                                      *
    // +----+   xs   +------+   Xs   +-----+   XXs   +------+   XXps   +-----+  tdoas   +-----+  doas   +-----+  doas   +-----+
    // | In | -----* | STFT | --+--* | SCM | ------* | PHAT | -------* | GCC | -------* | SSL | ------* | SST | ---+--* | Out |
    // +----+        +------+   |    +-----+         +------+          +-----+          +-----+         +-----+    |    +-----+
    //                          |                                                                                  |
    //                          |         +------------------------------------------------------------------------+
    //                          |         |                  
    //                          |         |    +----------+  tdoas  +----------+    Ws
    //                          |         +--* | Steering | ------* | DelaySum | -------+
    //                          |              +----------+         +----------+        |
    //                          |                                                       *
    //                          |    +-----+                  Zs                  +------------+   Ys   +-------+  ys   +-----+
    //                          +--* | Buf | -----------------------------------* | Beamformer | -----* | iSTFT | ----* | Out |
    //                               +-----+                                      +-------------        +-------+       +-----+

    //
    // Parameters
    //

    const unsigned int      num_channels      = 4;
    const unsigned int      num_shifts        = 128;
    const unsigned int      num_samples       = 512;
    const unsigned int      num_bins          = 257;
    const unsigned int      sample_rate       = 16000;
    const float             sound_speed       = 343.0f;
    const float             alpha             = 0.5f;
    const unsigned int      num_sources       = 1;
    const unsigned int      num_directions    = 1;
    const unsigned int      num_tracks        = 1;
    const unsigned int      num_pasts         = 40;
    const mics_hardware_t   micarray          = MICS_HARDWARE_RESPEAKER_USB_4;
    const points_geometry_t geometry          = POINTS_GEOMETRY_HALFSPHERE;
    const unsigned int      num_points        = 1000;
    const unsigned int      num_lags          = 50;

    //
    // Allocate memory
    //

    mics_t * mics = mics_construct(micarray); ODAS2_CHECK_PTR(mics);
    points_t * points = points_construct(geometry, num_points); ODAS2_CHECK_PTR(points);

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels, sample_rate); ODAS2_CHECK_PTR(wavin);

    hops_t * hops = hops_construct("xs", num_channels, num_shifts); ODAS2_CHECK_PTR(hops);
    freqs_t * freqs = freqs_construct("Xs", num_channels, num_bins); ODAS2_CHECK_PTR(freqs);
    masks_t * masks = masks_construct("Ms", num_channels, num_bins); ODAS2_CHECK_PTR(masks);
    covs_t * covs = covs_construct("XXs", num_channels, num_bins); ODAS2_CHECK_PTR(covs);
    covs_t * covs_phat = covs_construct("XXps", num_channels, num_bins); ODAS2_CHECK_PTR(covs_phat);
    tdoas_t * tdoas_pot = tdoas_construct("tdoas_pot", num_channels, num_sources); ODAS2_CHECK_PTR(tdoas_pot);
    doas_t * doas_pot = doas_construct("doas_pot", num_directions); ODAS2_CHECK_PTR(doas_pot);
    dsf_t * dsf = dsf_construct("dsf"); ODAS2_CHECK_PTR(dsf);
    doas_t * doas_trk = doas_construct("doas_trk", num_tracks); ODAS2_CHECK_PTR(doas_trk);
    tdoas_t * tdoas_trk = tdoas_construct("tdoas_trk", num_channels, num_tracks); ODAS2_CHECK_PTR(tdoas_trk);
    weights_t * weights = weights_construct("Ws", num_tracks, num_channels, num_bins); ODAS2_CHECK_PTR(weights);
    freqs_t * freqs_lag = freqs_construct("Zs", num_channels, num_bins); ODAS2_CHECK_PTR(freqs);
    freqs_t * freqs_bf = freqs_construct("Ys", num_tracks, num_bins); ODAS2_CHECK_PTR(freqs);
    hops_t * hops_bf = hops_construct("ys", num_tracks, num_shifts); ODAS2_CHECK_PTR(hops);

    stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, STFT_WINDOW_HANN); ODAS2_CHECK_PTR(stft);
    scm_t * scm = scm_construct(num_channels, num_bins, alpha); ODAS2_CHECK_PTR(scm);
    phat_t * phat = phat_construct(num_channels, num_bins); ODAS2_CHECK_PTR(phat);
    gcc_t * gcc = gcc_construct(num_sources, num_channels, num_bins); ODAS2_CHECK_PTR(gcc);
    ssl_t * ssl = ssl_construct(mics, points, (float)sample_rate, sound_speed, num_sources, num_directions); ODAS2_CHECK_PTR(ssl);
    sst_t * sst = sst_construct(num_tracks, num_directions, num_pasts); ODAS2_CHECK_PTR(sst);
    steering_t * steering = steering_construct(mics, (float)sample_rate, sound_speed, num_tracks); ODAS2_CHECK_PTR(steering);
    delaysum_t * delaysum = delaysum_construct(num_tracks, num_channels, num_bins); ODAS2_CHECK_PTR(delaysum);
    beamformer_t * beamformer = beamformer_construct(num_tracks, num_channels, num_bins); ODAS2_CHECK_PTR(beamformer);
    istft_t * istft = istft_construct(num_tracks, num_samples, num_shifts, STFT_WINDOW_HANN); ODAS2_CHECK_PTR(istft);

    buf_t * buf = buf_construct(freqs_getsize(freqs) * num_lags); ODAS2_CHECK_PTR(buf);
    msgout_t * msgout = msgout_construct("/dev/stdout"); ODAS2_CHECK_PTR(msgout);

    //
    // Process
    //

    masks_ones(masks);

    for (unsigned int index_lag = 0; index_lag < num_lags; index_lag++) {
        ODAS2_CHECK_CODE(buf_write_freqs(buf, freqs));
    }

    while (wavin_read(wavin, hops) == 0) {

        ODAS2_CHECK_CODE(stft_process(stft, hops, freqs));
        ODAS2_CHECK_CODE(scm_process(scm, freqs, masks, covs));
        ODAS2_CHECK_CODE(phat_process(phat, covs, covs_phat));
        ODAS2_CHECK_CODE(gcc_process(gcc, covs_phat, tdoas_pot));

        ODAS2_CHECK_CODE(ssl_process(ssl, tdoas_pot, doas_pot, NULL));
        ODAS2_CHECK_CODE(sst_process(sst, dsf, doas_pot, doas_trk));

        ODAS2_CHECK_CODE(buf_read_freqs(buf, freqs_lag));
        ODAS2_CHECK_CODE(buf_write_freqs(buf, freqs));

        ODAS2_CHECK_CODE(steering_process(steering, doas_trk, tdoas_trk));
        ODAS2_CHECK_CODE(delaysum_process(delaysum, tdoas_trk, weights));
        ODAS2_CHECK_CODE(beamformer_process(beamformer, freqs_lag, weights, freqs_bf));
        ODAS2_CHECK_CODE(istft_process(istft, freqs_bf, hops_bf));

        ODAS2_CHECK_CODE(msgout_write_doas(msgout, doas_trk));
        ODAS2_CHECK_CODE(msgout_write_hops(msgout, hops_bf));

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
    covs_destroy(covs);
    covs_destroy(covs_phat);
    tdoas_destroy(tdoas_pot);
    doas_destroy(doas_pot);
    doas_destroy(doas_trk);
    tdoas_destroy(tdoas_trk);
    weights_destroy(weights);
    freqs_destroy(freqs_lag);
    freqs_destroy(freqs_bf);
    hops_destroy(hops_bf);

    stft_destroy(stft);
    scm_destroy(scm);
    phat_destroy(phat);
    gcc_destroy(gcc);
    ssl_destroy(ssl);
    sst_destroy(sst);
    steering_destroy(steering);
    delaysum_destroy(delaysum);
    beamformer_destroy(beamformer);
    istft_destroy(istft);

    buf_destroy(buf);
    msgout_destroy(msgout);

    return 0;

}
