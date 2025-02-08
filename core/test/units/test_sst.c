#include "test_sst.h"

int test_sst(void) {

    const float eps = 0.01f;

    {

        const unsigned int num_directions = 4;
        const unsigned int num_tracks = 3;
        const unsigned int num_pasts = 40;

        doas_t * doas_src = doas_construct("doas_src", num_directions);
        doas_t * doas_dst = doas_construct("doas_dst", num_tracks);
        dsf_t * dsf = dsf_construct("dsf");

        sst_t * sst = sst_construct(num_tracks, num_directions, num_pasts);

        dir_t target[4];

        target[0] = (dir_t) { .type = POTENTIAL, .coord = (xyz_t) { .x = +0.707f, .y = +0.707f, .z = +0.000f }, .energy = 0.50f, .tracking_id = 0 };
        target[1] = (dir_t) { .type = POTENTIAL, .coord = (xyz_t) { .x = +0.000f, .y = +1.000f, .z = +0.000f }, .energy = 0.10f, .tracking_id = 0 };
        target[2] = (dir_t) { .type = POTENTIAL, .coord = (xyz_t) { .x = -0.707f, .y = -0.707f, .z = +0.000f }, .energy = 0.10f, .tracking_id = 0 };
        target[3] = (dir_t) { .type = POTENTIAL, .coord = (xyz_t) { .x = -1.000f, .y = +0.000f, .z = +0.000f }, .energy = 0.05f, .tracking_id = 0 };

        dir_t noise[7];

        noise[0] = (dir_t) { .type = POTENTIAL, .coord = (xyz_t) { .x = +0.001f, .y = -0.002f, .z = +0.001f }, .energy = +0.01f, .tracking_id = 0 };
        noise[1] = (dir_t) { .type = POTENTIAL, .coord = (xyz_t) { .x = -0.002f, .y = +0.001f, .z = -0.003f }, .energy = -0.01f, .tracking_id = 0 };
        noise[2] = (dir_t) { .type = POTENTIAL, .coord = (xyz_t) { .x = +0.015f, .y = -0.012f, .z = +0.004f }, .energy = +0.02f, .tracking_id = 0 };
        noise[3] = (dir_t) { .type = POTENTIAL, .coord = (xyz_t) { .x = -0.012f, .y = +0.007f, .z = +0.013f }, .energy = +0.01f, .tracking_id = 0 };
        noise[4] = (dir_t) { .type = POTENTIAL, .coord = (xyz_t) { .x = +0.001f, .y = -0.012f, .z = +0.011f }, .energy = +0.03f, .tracking_id = 0 };
        noise[5] = (dir_t) { .type = POTENTIAL, .coord = (xyz_t) { .x = +0.020f, .y = +0.021f, .z = +0.008f }, .energy = -0.03f, .tracking_id = 0 };
        noise[6] = (dir_t) { .type = POTENTIAL, .coord = (xyz_t) { .x = +0.005f, .y = -0.005f, .z = +0.001f }, .energy = -0.02f, .tracking_id = 0 };

        unsigned int index_noise = 0;

        for (unsigned int index_frame = 0; index_frame < 20; index_frame++) {

            for (unsigned int index_pot = 0; index_pot < num_directions; index_pot++) {

                doas_src->dirs[index_pot] = target[index_pot];
                doas_src->dirs[index_pot].coord = xyz_unit(xyz_add(doas_src->dirs[index_pot].coord, noise[index_noise].coord));
                doas_src->dirs[index_pot].energy += noise[index_noise].energy;

                index_noise++;
                index_noise %= 7;

            }

            sst_process(sst, dsf, doas_src, doas_dst);

        }


        if (!((doas_dst->dirs[0].type == TRACKED) && (xyz_mag(xyz_sub(doas_dst->dirs[0].coord, target[0].coord)) < eps) && doas_dst->dirs[0].tracking_id == 1)) {
            return -1;
        }
        if (!(doas_dst->dirs[1].type == UNDEFINED)) {
            return -1;
        }
        if (!(doas_dst->dirs[2].type == UNDEFINED)) {
            return -1;
        }

        doas_destroy(doas_src);
        doas_destroy(doas_dst);
        dsf_destroy(dsf);

        sst_destroy(sst);

    }

    return 0;

}
