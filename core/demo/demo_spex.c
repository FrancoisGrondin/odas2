#include <odas2/ios/msg.h>
#include <odas2/ios/wav.h>
#include <odas2/signals/freqs.h>
#include <odas2/signals/hops.h>
#include <odas2/systems/stft.h>

int main(int argc, char * argv[]) {

    //
    // Parameters
    //

    const unsigned int  num_channels        = 4;
    const unsigned int  num_shifts          = 128;
    const unsigned int  num_samples         = 512;
    const unsigned int  num_bins            = 257;
    const unsigned int  sample_rate         = 16000;

    //
    // Processing
    //
    // +----+   xs   +------+   Xs   +-----+
    // | In | -----* | STFT | -----* | Out |
    // +----+        +------+        +-----+
    //

    //
    // Allocate memory
    //

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels, sample_rate);

    hops_t * hops = hops_construct("xs", num_channels, num_shifts);
    freqs_t * freqs = freqs_construct("Xs", num_channels, num_bins);    

    stft_t * stft = stft_construct(num_channels, num_samples, num_shifts, num_bins, "hann");

    msgout_t * msgout = msgout_construct("/dev/stdout");

    //
    // Process
    //

    while (wavin_read(wavin, hops) == 0) {

        stft_process(stft, hops, freqs);
        msgout_write_hops(msgout, hops);
        msgout_write_freqs(msgout, freqs);
        
    }

    // 
    // Free memory
    //

    wavin_destroy(wavin);

    freqs_destroy(freqs);
    hops_destroy(hops);

    stft_destroy(stft);

    msgout_destroy(msgout);

    return 0;

}