#include <odas2/ios/msg.h>
#include <odas2/ios/wav.h>
#include <odas2/signals/hops.h>

int main(int argc, char * argv[]) {

    //
    // +----+   xs   +-----+
    // | In | -----* | Out |
    // +----+        +-----+
    //

    //
    // Parameters
    //

    const unsigned int  num_channels        = 4;
    const unsigned int  num_shifts          = 128;
    const unsigned int  sample_rate         = 16000;

    //
    // Allocate memory
    //

    wavin_t * wavin = wavin_construct("/dev/stdin", num_shifts, num_channels, sample_rate);

    hops_t * hops = hops_construct("xs", num_channels, num_shifts);

    msgout_t * msgout = msgout_construct("/dev/stdout");

    //
    // Process
    //

    while (wavin_read(wavin, hops) == 0) {
        msgout_write_hops(msgout, hops);
    }
    
    //
    // Free memory
    //

    wavin_destroy(wavin);

    hops_destroy(hops);

    msgout_destroy(msgout);

    return 0;

}