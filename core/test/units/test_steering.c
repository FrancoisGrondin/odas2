#include "test_ssl.h"

int test_steering(void) {

    const float eps = 0.001f;

    {

        const float sample_rate = 16000.0f;
        const float sound_speed = 343.0f;
        const unsigned int num_channels = 4;
        const unsigned int num_pairs = 6;
        const unsigned int num_sources = 3;
        const unsigned int num_directions = 3;

        const float tdoas_0[6] = { -1.4927f, -2.9854f, -1.4927f, -1.4927f, +0.0000f, +1.4927f };
        const float tdoas_1[6] = { +1.4927f, +0.0000f, -1.4927f, -1.4927f, -2.9854f, -1.4927f };
        const float tdoas_2[6] = { +0.0000f, +2.1107f, +2.1107f, +2.1107f, +2.1107f, +0.0000f };

        mics_t * mics = mics_construct("respeaker_usb");
        doas_t * doas = doas_construct("doas", num_directions);
        tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);
        steering_t * steering = steering_construct(mics, sample_rate, sound_speed, num_sources);

        doas->pots[0] = (pot_t) { .direction = (xyz_t) { .x = -1.000, .y = +0.000, .z = +0.000 }, .energy = 0.10 };
        doas->pots[1] = (pot_t) { .direction = (xyz_t) { .x = +0.000, .y = -1.000, .z = +0.000 }, .energy = 0.10 };
        doas->pots[2] = (pot_t) { .direction = (xyz_t) { .x = +0.707, .y = +0.707, .z = +0.000 }, .energy = 0.10 };

        steering_process(steering, doas, tdoas);

        for (unsigned int index_pair = 0; index_pair < num_pairs; index_pair++) {
            
            if (!(fabsf(tdoas->taus[0][index_pair].delay - tdoas_0[index_pair]) < eps)) {
                return -1;
            }
            if (!(fabsf(tdoas->taus[1][index_pair].delay - tdoas_1[index_pair]) < eps)) {
                return -1;
            }
            if (!(fabsf(tdoas->taus[2][index_pair].delay - tdoas_2[index_pair]) < eps)) {
                return -1;
            }

        }

        mics_destroy(mics);
        doas_destroy(doas);
        tdoas_destroy(tdoas);
        steering_destroy(steering);

    }

    return 0;

}