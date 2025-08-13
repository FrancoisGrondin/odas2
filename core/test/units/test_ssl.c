#include "test_ssl.h"

int test_ssl(void) {

    const float eps = 0.1f;

    {

        mics_t * mics = mics_construct(MICS_HARDWARE_RESPEAKER_USB_4);
        points_t * points = points_construct(POINTS_GEOMETRY_HALFSPHERE, 1000);

        const float sample_rate = 16000.0f;
        const float sound_speed = 343.0f;
        const unsigned int num_sources = 4;
        const unsigned int num_directions = 2;
        const unsigned int num_channels = 4;

        ssl_t * ssl = ssl_construct(mics, points, sample_rate, sound_speed, num_sources, num_directions);
        steering_t * steering = steering_construct(mics, sample_rate, sound_speed, num_sources);

        doas_t * doas_src = doas_construct("doas_src", num_sources);
        doas_t * doas_dst = doas_construct("doas_dst", num_directions);
        tdoas_t * tdoas = tdoas_construct("tdoas", num_channels, num_sources);

        doas_src->dirs[0] = (dir_t) { .type = TARGET, .coord = (xyz_t) { .x = +1.000f, .y = +0.000f, .z = +0.000f }, .energy = 0.50f, .tracking_id = 0 };
        doas_src->dirs[1] = (dir_t) { .type = TARGET, .coord = (xyz_t) { .x = +0.000f, .y = +1.000f, .z = +0.000f }, .energy = 0.25f, .tracking_id = 0 };
        doas_src->dirs[2] = (dir_t) { .type = TARGET, .coord = (xyz_t) { .x = -0.707f, .y = -0.707f, .z = +0.000f }, .energy = 0.10f, .tracking_id = 0 };
        doas_src->dirs[3] = (dir_t) { .type = TARGET, .coord = (xyz_t) { .x = +0.577f, .y = +0.577f, .z = +0.577f }, .energy = 0.05f, .tracking_id = 0 };

        steering_process(steering, doas_src, tdoas);
        ssl_process(ssl, tdoas, doas_dst, NULL);

        if (!((fabsf(doas_src->dirs[0].coord.x - doas_dst->dirs[0].coord.x) < eps) &&
              (fabsf(doas_src->dirs[0].coord.y - doas_dst->dirs[0].coord.y) < eps) &&
              (fabsf(doas_src->dirs[0].coord.z - doas_dst->dirs[0].coord.z) < eps))) {
            return -1;
        }

        if (!((fabsf(doas_src->dirs[1].coord.x - doas_dst->dirs[1].coord.x) < eps) &&
              (fabsf(doas_src->dirs[1].coord.y - doas_dst->dirs[1].coord.y) < eps) &&
              (fabsf(doas_src->dirs[1].coord.z - doas_dst->dirs[1].coord.z) < eps))) {
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
