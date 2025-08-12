#include <systems/steering.h>
#include <utils/error.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

steering_t * steering_construct(const mics_t * mics, const float sample_rate, const float sound_speed, const unsigned int num_sources) {
    if (mics->num_mics < 2) {
        odas2_set_error_number(ODAS2_ERROR_STEERING_CONSTRUCT_NUM_MICS);
        return NULL;
    }
    if (sample_rate <= 0.0f) {
        odas2_set_error_number(ODAS2_ERROR_STEERING_CONSTRUCT_SAMPLE_RATE);
        return NULL;
    }
    if (sound_speed <= 0.0f) {
        odas2_set_error_number(ODAS2_ERROR_STEERING_CONSTRUCT_SOUND_SPEED);
        return NULL;
    }
    if (num_sources < 1) {
        odas2_set_error_number(ODAS2_ERROR_STEERING_CONSTRUCT_NUM_SOURCES);
        return NULL;
    }

    steering_t * obj = (steering_t *) malloc(sizeof(steering_t));

    obj->mics = mics;

    obj->num_channels = mics->num_mics;
    obj->num_pairs = (mics->num_mics - 1) * mics->num_mics / 2;
    obj->num_sources = num_sources;

    obj->sample_rate = sample_rate;
    obj->sound_speed = sound_speed;

    return obj;

}

void steering_destroy(steering_t * obj) {

    free(obj);

}

int steering_process(const steering_t * obj, const doas_t * doas, tdoas_t * tdoas)  {
    if (obj->num_sources != doas->num_directions) {
        odas2_set_error_number(ODAS2_ERROR_STEERING_PROCESS_DOAS_NUM_DIRECTIONS);
        return -1;
    }
    if (obj->num_sources != tdoas->num_sources) {
        odas2_set_error_number(ODAS2_ERROR_STEERING_PROCESS_TDOAS_NUM_SOURCES);
        return -1;
    }
    if (obj->num_channels != tdoas->num_channels) {
        odas2_set_error_number(ODAS2_ERROR_STEERING_PROCESS_TDOAS_NUM_CHANNELS);
        return -1;
    }

    unsigned int index_pair = 0;

    for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {

        mic_t mic1 = obj->mics->mics[index_channel1];

        for (unsigned int index_channel2 = (index_channel1 + 1); index_channel2 < obj->num_channels; index_channel2++) {

            mic_t mic2 = obj->mics->mics[index_channel2];

            for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {

                dir_t dir = doas->dirs[index_source];

                tdoas->taus[index_source][index_pair].delay = (obj->sample_rate / obj->sound_speed) * xyz_dot(xyz_sub(mic2.position, mic1.position), dir.coord);
                tdoas->taus[index_source][index_pair].amplitude = dir.energy;

            }

            index_pair++;

        }

    }

    return 0;

}
