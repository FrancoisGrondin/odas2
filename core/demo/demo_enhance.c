#include <odas2/ios/msg.h>
#include <odas2/ios/wav.h>
#include <odas2/signals/hops.h>
#include <odas2/signals/freqs.h>
#include <odas2/signals/masks.h>
#include <odas2/systems/enhancement.h>
#include <odas2/systems/postfilter.h>
#include <odas2/systems/stft.h>
#include <odas2/utils/error.h>

int main(int argc, char * argv[]) {

    //
    // +----+   xs   +------+   Xs   +-------------+   Ms   +------------+   Ys   +-------+   ys   +-----+
    // | In | -----* | STFT | -----* | Enhancement | -----* | Postfilter | -----* | iSTFT | -----* | Out |
    // +----+        +------+   |    +-------------+        +------------+        +-------+        +-----+
    //                          |                                  *
    //                          |                                  |
    //                          +----------------------------------+
    //

    //
    // Parameters
    //

    const unsigned int  num_channels        = 4;
    const unsigned int  num_shifts          = 128;
    const unsigned int  num_samples         = 512;
    const unsigned int  num_bins            = 257;
    const unsigned int  sample_rate         = 16000;

    //
    // Allocate memory
    //

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels, sample_rate);
    ODAS2_CHECK_PTR(wavin);
    wavout_t * wavout = wavout_construct("/dev/stdout", num_shifts, num_channels, sample_rate);
    ODAS2_CHECK_PTR(wavout);

    hops_t * hops_in = hops_construct("xs", num_channels, num_shifts);
    ODAS2_CHECK_PTR(hops_in);
    freqs_t * freqs_in = freqs_construct("Xs", num_channels, num_bins);
    ODAS2_CHECK_PTR(freqs_in);
    masks_t * masks_out = masks_construct("Ms", num_channels, num_bins);
    ODAS2_CHECK_PTR(masks_out);
    freqs_t * freqs_out = freqs_construct("Ys", num_channels, num_bins);
    ODAS2_CHECK_PTR(freqs_out);
    hops_t * hops_out = hops_construct("ys", num_channels, num_shifts);
    ODAS2_CHECK_PTR(hops_out);

    stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, STFT_WINDOW_HANN);
    ODAS2_CHECK_PTR(stft);
    enhancement_t * enhancement = enhancement_construct(num_channels, num_bins);
    ODAS2_CHECK_PTR(enhancement);
    postfilter_t * postfilter = postfilter_construct(num_channels, num_bins);
    ODAS2_CHECK_PTR(postfilter);
    istft_t * istft = istft_construct(num_channels, num_samples, num_shifts, STFT_WINDOW_HANN);
    ODAS2_CHECK_PTR(istft);

    //
    // Process
    //

    while (wavin_read(wavin, hops_in) == 0) {

        ODAS2_CHECK_CODE(stft_process(stft, hops_in, freqs_in));
        ODAS2_CHECK_CODE(enhancement_process(enhancement, freqs_in, masks_out));
        ODAS2_CHECK_CODE(postfilter_process(postfilter, freqs_in, masks_out, freqs_out));
        ODAS2_CHECK_CODE(istft_process(istft, freqs_out, hops_out));
        ODAS2_CHECK_CODE(wavout_write(wavout, hops_out));

    }

    //
    // Free memory
    //

    wavin_destroy(wavin);
    wavout_destroy(wavout);

    hops_destroy(hops_in);
    freqs_destroy(freqs_in);
    masks_destroy(masks_out);
    freqs_destroy(freqs_out);
    hops_destroy(hops_out);

    stft_destroy(stft);
    enhancement_destroy(enhancement);
    postfilter_destroy(postfilter);
    istft_destroy(istft);

    return 0;

}
