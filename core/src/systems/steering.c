#include <systems/steering.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

steering_t * steering_construct(const mics_t * mics, const float sample_rate, const float sound_speed, const unsigned int num_sources) {

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

    unsigned int index_pair = 0;

    for (unsigned int index_channel1 = 0; index_channel1 < obj->num_channels; index_channel1++) {

        mic_t mic1 = obj->mics->mics[index_channel1];

        for (unsigned int index_channel2 = (index_channel1 + 1); index_channel2 < obj->num_channels; index_channel2++) {

            mic_t mic2 = obj->mics->mics[index_channel2];

            for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {

                pot_t pot = doas->pots[index_source];

                tdoas->taus[index_source][index_pair].delay = (obj->sample_rate / obj->sound_speed) * xyz_dot(xyz_sub(mic2.position, mic1.position), pot.direction);
                tdoas->taus[index_source][index_pair].amplitude = pot.energy;

            }

            index_pair++;

        }

    }

    return 0;

}

    

