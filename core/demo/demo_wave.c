#include <odas2/ios/msg.h>
#include <odas2/ios/wav.h>
#include <odas2/signals/hops.h>
#include <odas2/utils/error.h>

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
    ODAS2_CHECK_PTR(wavin);

    hops_t * hops = hops_construct("xs", num_channels, num_shifts);
    ODAS2_CHECK_PTR(hops);

    msgout_t * msgout = msgout_construct("/dev/stdout");
    ODAS2_CHECK_PTR(msgout);

    //
    // Process
    //

    while (wavin_read(wavin, hops) == 0) {
        ODAS2_CHECK_CODE(msgout_write_hops(msgout, hops));
    }

    //
    // Free memory
    //

    wavin_destroy(wavin);

    hops_destroy(hops);

    msgout_destroy(msgout);

    return 0;

}
