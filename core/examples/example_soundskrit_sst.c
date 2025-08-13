#include <odas2/ios/msg.h>
#include <odas2/ios/wav.h>
#include <odas2/signals/covs.h>
#include <odas2/signals/doas.h>
#include <odas2/signals/freqs.h>
#include <odas2/signals/hops.h>
#include <odas2/signals/masks.h>
#include <odas2/signals/tdoas.h>
#include <odas2/systems/fcc.h>
#include <odas2/systems/gcc.h>
#include <odas2/systems/mixer.h>
#include <odas2/systems/phat.h>
#include <odas2/systems/scm.h>
#include <odas2/systems/ssl.h>
#include <odas2/systems/sst.h>
#include <odas2/systems/stft.h>
#include <odas2/utils/mics.h>
#include <odas2/utils/points.h>
#include <odas2/utils/error.h>

#include <string.h>

int main(int argc, char * argv[]) {

    //
    //                             Ms (all 1's)                                                                      dsf (default values)
    //                                  |                                                                                     |
    //                                  *                                                                                     *
    // +----+   rs   +-----+   xs   +------+   Xs   +-----+   XXs   +------+   XXps   +---------+  tdoas   +-----+  doas   +-----+  doas   +-----+
    // | In | -----* | Mix | -----* | STFT | -----* | SCM | ------* | PHAT | -------* | GCC/FCC | -------* | SSL | ------* | SST | ------* | Out |
    // +----+        +-----+         ------+        +-----+         +------+          +---------+          +-----+         +-----+         +-----+
    //

    //
    // Parameters
    //

    const unsigned int      num_channels_raw = 6;
    const unsigned int      num_channels     = 3;
    const char              channels[]       = "0,2,4";
    const unsigned int      num_shifts       = 128;
    const unsigned int      num_samples      = 512;
    const unsigned int      num_bins         = 257;
    const unsigned int      sample_rate      = 16000;
    const float             sound_speed      = 343.0f;
    const float             alpha            = 0.5f;
    const unsigned int      num_sources      = 2;
    const unsigned int      num_directions   = 2;
    const unsigned int      num_tracks       = 2;
    const unsigned int      num_pasts        = 40;
    const char              method[]         = "gcc";
    const mics_hardware_t   micarray         = MICS_HARDWARE_SOUNDSKRIT_MUG;
    const points_geometry_t geometry         = POINTS_GEOMETRY_CIRCLE;
    const unsigned int      num_points       = 360;


    //
    // Allocate memory
    //

    mics_t * mics = mics_construct(micarray);
    ODAS2_CHECK_PTR(mics);
    points_t * points = points_construct(geometry, num_points);
    ODAS2_CHECK_PTR(points);

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels_raw, sample_rate);
    ODAS2_CHECK_PTR(wavin);

    hops_t * hops_raw = hops_construct("rs", num_channels_raw, num_shifts);
    ODAS2_CHECK_PTR(hops_raw);
    hops_t * hops_chs = hops_construct("xs", num_channels, num_shifts);
    ODAS2_CHECK_PTR(hops_chs);
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
    doas_t * doas_potential = doas_construct("potential", num_directions);
    ODAS2_CHECK_PTR(doas_potential);
    dsf_t * dsf = dsf_construct("dsf");
    ODAS2_CHECK_PTR(dsf);
    doas_t * doas_tracked = doas_construct("tracked", num_tracks);
    ODAS2_CHECK_PTR(doas_tracked);

    mixer_t * mixer = mixer_construct(channels);
    ODAS2_CHECK_PTR(mixer);
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
    ssl_t * ssl = ssl_construct(mics, points, (float)sample_rate, sound_speed, num_sources, num_directions);
    ODAS2_CHECK_PTR(ssl);
    sst_t * sst = sst_construct(num_tracks, num_directions, num_pasts);
    ODAS2_CHECK_PTR(sst);

    msgout_t * msgout = msgout_construct("/dev/stdout");
    ODAS2_CHECK_PTR(msgout);

    //
    // Process
    //

    masks_ones(masks);

    while (wavin_read(wavin, hops_raw) == 0) {

        ODAS2_CHECK_CODE(mixer_process(mixer, hops_raw, hops_chs));
        ODAS2_CHECK_CODE(stft_process(stft, hops_chs, freqs));
        ODAS2_CHECK_CODE(scm_process(scm, freqs, masks, covs));
        ODAS2_CHECK_CODE(phat_process(phat, covs, covs_phat));

        if (strcmp(method, "gcc") == 0) {
            ODAS2_CHECK_CODE(gcc_process(gcc, covs_phat, tdoas));
        }
        if (strcmp(method, "fcc") == 0) {
            ODAS2_CHECK_CODE(fcc_process(fcc, covs_phat, tdoas));
        }

        ODAS2_CHECK_CODE(ssl_process(ssl, tdoas, doas_potential));
        ODAS2_CHECK_CODE(sst_process(sst, dsf, doas_potential, doas_tracked));

        //ODAS2_CHECK_CODE(msgout_write_freqs(msgout, freqs));
        //ODAS2_CHECK_CODE(msgout_write_tdoas(msgout, tdoas));
        ODAS2_CHECK_CODE(msgout_write_doas(msgout, doas_potential));
        ODAS2_CHECK_CODE(msgout_write_doas(msgout, doas_tracked));

    }

    //
    // Free memory
    //

    mics_destroy(mics);
    points_destroy(points);

    wavin_destroy(wavin);

    hops_destroy(hops_raw);
    hops_destroy(hops_chs);
    freqs_destroy(freqs);
    masks_destroy(masks);
    covs_destroy(covs);
    covs_destroy(covs_phat);
    tdoas_destroy(tdoas);
    doas_destroy(doas_potential);
    dsf_destroy(dsf);
    doas_destroy(doas_tracked);

    mixer_destroy(mixer);
    stft_destroy(stft);
    scm_destroy(scm);
    phat_destroy(phat);
    fcc_destroy(fcc);
    gcc_destroy(gcc);
    ssl_destroy(ssl);
    sst_destroy(sst);

    msgout_destroy(msgout);

    return 0;

}
