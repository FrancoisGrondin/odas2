#include "test_ssl.h"

int test_ssl(void) {

    const float eps = 0.1f;

    {

        mics_t * mics = mics_construct("respeaker_usb");
        points_t * points = points_construct("halfsphere");

        const float sample_rate = 16000.0f;
        const float sound_speed = 343.0f;
        const unsigned int num_sources = 4;
        const unsigned int num_directions = 2;
        const unsigned int num_channels = 4;
        const unsigned int num_pairs = 6;

        ssl_t * ssl = ssl_construct(mics, points, sample_rate, sound_speed, num_sources, num_directions);
        steering_t * steering = steering_construct(mics, sample_rate, sound_speed, num_sources);

        doas_t * doas_src = doas_construct("doas_src", num_sources);
        doas_t * doas_dst = doas_construct("doas_dst", num_directions);
        tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);

        doas_src->pots[0] = (pot_t) { .direction = (xyz_t) { .x = +1.000, .y = +0.000, .z = +0.000 }, .energy = 0.50 };
        doas_src->pots[1] = (pot_t) { .direction = (xyz_t) { .x = +0.000, .y = +1.000, .z = +0.000 }, .energy = 0.25 };
        doas_src->pots[2] = (pot_t) { .direction = (xyz_t) { .x = -0.707, .y = -0.707, .z = +0.000 }, .energy = 0.10 };
        doas_src->pots[3] = (pot_t) { .direction = (xyz_t) { .x = +0.577, .y = +0.577, .z = +0.577 }, .energy = 0.05 };

        steering_process(steering, doas_src, tdoas);
        ssl_process(ssl, tdoas, doas_dst);

        if (!((fabsf(doas_src->pots[0].direction.x - doas_dst->pots[0].direction.x) < eps) &&
              (fabsf(doas_src->pots[0].direction.y - doas_dst->pots[0].direction.y) < eps) &&
              (fabsf(doas_src->pots[0].direction.z - doas_dst->pots[0].direction.z) < eps))) {
            return -1;
        }

        if (!((fabsf(doas_src->pots[1].direction.x - doas_dst->pots[1].direction.x) < eps) &&
              (fabsf(doas_src->pots[1].direction.y - doas_dst->pots[1].direction.y) < eps) &&
              (fabsf(doas_src->pots[1].direction.z - doas_dst->pots[1].direction.z) < eps))) {
            return -1;
        }       

        doas_destroy(doas_src);
        doas_destroy(doas_dst);
        tdoas_destroy(tdoas);
        ssl_destroy(ssl);
        steering_destroy(steering);

    }

    return 0;

}