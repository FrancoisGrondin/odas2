#include "test_mixer.h"

int test_mixer(void) {

    const float eps = 0.001f;

    {

        const char channels[] = "1,5,8,2";
        const unsigned int map[4] = {1,5,8,2};
        
        mixer_t * mixer = mixer_construct(channels);
        
        for (unsigned int index = 0; index < 4; index++) {
            if (mixer->map[index] != map[index]) {
                return -1;
            }
        }

        mixer_destroy(mixer);

    }

    {

        const unsigned int num_channels_in = 4;
        const unsigned int num_channels_out = 2;
        const unsigned int num_shifts = 5;
        const char channels[] = "0,3";
        const float samples[4][5] = { {  +1.0f,  +2.0f,  +3.0f,  +4.0f,  +5.0f }, 
                                      {  +6.0f,  +7.0f,  +8.0f,  +9.0f, +10.0f }, 
                                      { +11.0f, +12.0f, +13.0f, +14.0f, +15.0f },
                                      { +16.0f, +17.0f, +18.0f, +19.0f, +20.0f } };

        hops_t * hops_in = hops_construct("in", num_channels_in, num_shifts);
        hops_t * hops_out = hops_construct("out", num_channels_out, num_shifts);
        mixer_t * mixer = mixer_construct(channels);

        memcpy(hops_in->samples[0], samples[0], sizeof(float) * num_shifts);
        memcpy(hops_in->samples[1], samples[1], sizeof(float) * num_shifts);
        memcpy(hops_in->samples[2], samples[2], sizeof(float) * num_shifts);
        memcpy(hops_in->samples[3], samples[3], sizeof(float) * num_shifts);

        mixer_process(mixer, hops_in, hops_out);

        for (unsigned int index_shift = 0; index_shift < num_shifts; index_shift++) {
            if (!(fabsf(hops_out->samples[0][index_shift] - samples[0][index_shift]) < eps)) {
                return -2;
            }
            if (!(fabsf(hops_out->samples[1][index_shift] - samples[3][index_shift]) < eps)) {
                return -2;
            }
        }

        mixer_destroy(mixer);
        hops_destroy(hops_in);
        hops_destroy(hops_out);

    }

    return 0;

}