#include <odas2/ios/wav.h>
#include <odas2/signals/hops.h>
#include <odas2/systems/mixer.h>

int main(int argc, char * argv[]) {

    //
    // Parameters
    //

    const unsigned int  num_channels_in     = 4;
    const unsigned int  num_channels_out    = 2;
    const unsigned int  num_shifts          = 128;
    const unsigned int  sample_rate         = 16000;
    const char          channels[]          = "0,1";

    //
    // Pipeline
    //
    // +----+   xs   +-------+   ys   +-----+
    // | In | -----* | Mixer | -----* | Out |
    // +----+        +-------+        +-----+
    //

    //
    // Allocate memory
    //

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels_in, sample_rate);

    hops_t * hops_in = hops_construct("xs", num_channels_in, num_shifts);
    mixer_t * mixer = mixer_construct(channels);
    hops_t * hops_out = hops_construct("ys", num_channels_out, num_shifts);

    wavout_t * wavout = wavout_construct("/dev/stdout", num_shifts, num_channels_out, sample_rate);

    //
    // Process
    //

    while (wavin_read(wavin, hops_in) == 0) {
        mixer_process(mixer, hops_in, hops_out);
        wavout_write(wavout, hops_out);
    }
    
    //
    // Free memory
    //

    wavin_destroy(wavin);

    hops_destroy(hops_in);
    mixer_destroy(mixer);
    hops_destroy(hops_out);

    wavout_destroy(wavout);

    return 0;

}