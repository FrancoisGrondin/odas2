#include <odas2/ios/msg.h>
#include <odas2/ios/wav.h>
#include <odas2/signals/hops.h>
#include <odas2/signals/freqs.h>
#include <odas2/signals/masks.h>
#include <odas2/systems/enhancement.h>
#include <odas2/systems/postfilter.h>
#include <odas2/systems/stft.h>

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
    wavout_t * wavout = wavout_construct("/dev/stdout", num_shifts, num_channels, sample_rate);

    hops_t * hops_in = hops_construct("xs", num_channels, num_shifts);
    freqs_t * freqs_in = freqs_construct("Xs", num_channels, num_bins);
    masks_t * masks_out = masks_construct("Ms", num_channels, num_bins);
    freqs_t * freqs_out = freqs_construct("Ys", num_channels, num_bins);
    hops_t * hops_out = hops_construct("ys", num_channels, num_shifts);

    stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, num_bins, "hann");
    enhancement_t * enhancement = enhancement_construct(num_channels, num_bins);
    postfilter_t * postfilter = postfilter_construct(num_channels, num_bins);
    istft_t * istft = istft_construct(num_channels, num_samples, num_shifts, num_bins, "hann");
    
    //
    // Process
    //

    while (wavin_read(wavin, hops_in) == 0) {

        stft_process(stft, hops_in, freqs_in);
        enhancement_process(enhancement, freqs_in, masks_out);
        postfilter_process(postfilter, freqs_in, masks_out, freqs_out);
        istft_process(istft, freqs_out, hops_out);
        wavout_write(wavout, hops_out);
        
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