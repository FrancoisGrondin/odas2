#include <utils/mics.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

mic_t mic_cst(const xyz_t position, const xyz_t direction, const char * pattern) {

    mic_t mic;

    mic.position = position;
    mic.direction = direction;
    strcpy(mic.pattern, pattern);

    return mic;

}

float mic_gain(const mic_t mic, const xyz_t direction) {

    float cos_theta = xyz_dot(mic.direction, direction) / (xyz_mag(mic.direction) * xyz_mag(direction));

    float gain = 0.0f;

    if (strcmp(mic.pattern, "omnidirectional") == 0) {
        gain = 1.0f;
    }
    if (strcmp(mic.pattern, "cardioid") == 0) {
        gain = 0.5f + 0.5f * cos_theta;
    }

    return gain;

}

void mic_printf(const mic_t mic) {

    printf("position: (%+1.3f, %+1.3f, %+1.3f)\n", mic.position.x, mic.position.y, mic.position.z);
    printf("direction: (%+1.2f, %+1.2f, %+1.2f)\n", mic.direction.x, mic.direction.y, mic.direction.z);
    printf("pattern: %s\n", mic.pattern);

}

mics_t * mics_construct(const char * hardware) {

    mics_t * obj = (mics_t *) NULL;

    if (strcmp(hardware, "respeaker_usb") == 0) {
        obj = mics_respeaker_usb();
    }
    if (strcmp(hardware, "minidsp_uma") == 0) {
        obj = mics_minidsp_uma();
    }

    return obj;

}

void mics_destroy(mics_t * obj) {

    free(obj->mics);
    free(obj);

}

static mics_t * mics_respeaker_usb(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 4;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = -0.032, .y = +0.000, .z = +0.000 }, (xyz_t) { .x = +0.000, .y = +0.000, .z = +1.000 }, "cardioid");
    obj->mics[1] = mic_cst((xyz_t) { .x = +0.000, .y = -0.032, .z = +0.000 }, (xyz_t) { .x = +0.000, .y = +0.000, .z = +1.000 }, "cardioid");
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.032, .y = +0.000, .z = +0.000 }, (xyz_t) { .x = +0.000, .y = +0.000, .z = +1.000 }, "cardioid");
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.000, .y = +0.032, .z = +0.000 }, (xyz_t) { .x = +0.000, .y = +0.000, .z = +1.000 }, "cardioid");

    return obj;

}

static mics_t * mics_minidsp_uma(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 7;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = +0.000, .y = +0.000, .z = +0.000 }, (xyz_t) { .x = +0.000, .y = +0.000, .z = +1.000 }, "cardioid");
    obj->mics[1] = mic_cst((xyz_t) { .x = +0.000, .y = +0.043, .z = +0.000 }, (xyz_t) { .x = +0.000, .y = +0.000, .z = +1.000 }, "cardioid");
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.037, .y = +0.021, .z = +0.000 }, (xyz_t) { .x = +0.000, .y = +0.000, .z = +1.000 }, "cardioid");
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.037, .y = -0.021, .z = +0.000 }, (xyz_t) { .x = +0.000, .y = +0.000, .z = +1.000 }, "cardioid");
    obj->mics[4] = mic_cst((xyz_t) { .x = +0.000, .y = -0.043, .z = +0.000 }, (xyz_t) { .x = +0.000, .y = +0.000, .z = +1.000 }, "cardioid");
    obj->mics[5] = mic_cst((xyz_t) { .x = -0.037, .y = -0.021, .z = +0.000 }, (xyz_t) { .x = +0.000, .y = +0.000, .z = +1.000 }, "cardioid");
    obj->mics[6] = mic_cst((xyz_t) { .x = -0.037, .y = +0.021, .z = +0.000 }, (xyz_t) { .x = +0.000, .y = +0.000, .z = +1.000 }, "cardioid");

    return obj;

}