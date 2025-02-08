#include <utils/mics.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static mics_t * mics_respeaker_usb_4(void);
static mics_t * mics_respeaker_usb_6(void);
static mics_t * mics_minidsp_uma(void);
static mics_t * mics_sc16_demo_array(void);
static mics_t * mics_sc16f(void);
static mics_t * mics_vibeus_circular(void);
static mics_t * mics_soundskrit_mug(void);

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

    if (strcmp(hardware, "respeaker_usb_4") == 0) {
        obj = mics_respeaker_usb_4();
    }
    if (strcmp(hardware, "respeaker_usb_6") == 0) {
        obj = mics_respeaker_usb_6();
    }
    if (strcmp(hardware, "minidsp_uma") == 0) {
        obj = mics_minidsp_uma();
    }
    if (strcmp(hardware, "sc16_demo_array") == 0) {
        obj = mics_sc16_demo_array();
    }
    if (strcmp(hardware, "sc16f") == 0) {
        obj = mics_sc16f();
    }
    if (strcmp(hardware, "vibeus_circular") == 0) {
        obj = mics_vibeus_circular();
    }
    if (strcmp(hardware, "soundskrit_mug") == 0) {
        obj = mics_soundskrit_mug();
    }

    return obj;

}

mics_t * mics_construct_uninitialized(const unsigned int num_mics) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = num_mics;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    return obj;
}

void mics_destroy(mics_t * obj) {

    free(obj->mics);
    free(obj);

}

static mics_t * mics_respeaker_usb_4(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 4;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = -0.032f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[1] = mic_cst((xyz_t) { .x = +0.000f, .y = -0.032f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.032f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.000f, .y = +0.032f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");

    return obj;

}

static mics_t * mics_respeaker_usb_6(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 6;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = -0.0232f, .y = +0.0401f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[1] = mic_cst((xyz_t) { .x = -0.0463f, .y = -0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[2] = mic_cst((xyz_t) { .x = -0.0232f, .y = -0.0401f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.0232f, .y = -0.0401f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[4] = mic_cst((xyz_t) { .x = +0.0463f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[5] = mic_cst((xyz_t) { .x = +0.0232f, .y = +0.0401f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");

    return obj;

}

static mics_t * mics_minidsp_uma(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 7;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = +0.000f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[1] = mic_cst((xyz_t) { .x = +0.000f, .y = +0.043f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.037f, .y = +0.021f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.037f, .y = -0.021f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[4] = mic_cst((xyz_t) { .x = +0.000f, .y = -0.043f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[5] = mic_cst((xyz_t) { .x = -0.037f, .y = -0.021f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");
    obj->mics[6] = mic_cst((xyz_t) { .x = -0.037f, .y = +0.021f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "cardioid");

    return obj;

}

static mics_t * mics_sc16_demo_array(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 16;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = +0.088f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[1] = mic_cst((xyz_t) { .x = +0.076f, .y = +0.044f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.044f, .y = +0.076f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.000f, .y = +0.088f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");

    obj->mics[4] = mic_cst((xyz_t) { .x = +0.000f, .y = +0.048f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[5] = mic_cst((xyz_t) { .x = -0.044f, .y = +0.076f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[6] = mic_cst((xyz_t) { .x = -0.076f, .y = +0.044f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[7] = mic_cst((xyz_t) { .x = -0.088f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");

    obj->mics[8] = mic_cst((xyz_t) { .x = -0.048f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[9] = mic_cst((xyz_t) { .x = -0.076f, .y = -0.044f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[10] = mic_cst((xyz_t) { .x = -0.044f, .y = -0.076f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[11] = mic_cst((xyz_t) { .x = +0.000f, .y = -0.088f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");

    obj->mics[12] = mic_cst((xyz_t) { .x = +0.000f, .y = -0.048f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[13] = mic_cst((xyz_t) { .x = +0.044f, .y = -0.076f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[14] = mic_cst((xyz_t) { .x = +0.076f, .y = -0.044f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[15] = mic_cst((xyz_t) { .x = +0.048f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");

    return obj;

}

static mics_t * mics_sc16f(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 16;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = -0.0675f, .y = +0.0675f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[1] = mic_cst((xyz_t) { .x = -0.0247f, .y = +0.0922f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.0247f, .y = +0.0922f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.0675f, .y = +0.0675f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");

    obj->mics[4] = mic_cst((xyz_t) { .x = +0.0922f, .y = +0.0247f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[5] = mic_cst((xyz_t) { .x = +0.0922f, .y = -0.0247f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[6] = mic_cst((xyz_t) { .x = +0.0675f, .y = -0.0675f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[7] = mic_cst((xyz_t) { .x = +0.0247f, .y = -0.0922f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");

    obj->mics[8] = mic_cst((xyz_t) { .x = -0.0247f, .y = -0.0922f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[9] = mic_cst((xyz_t) { .x = -0.0675f, .y = -0.0675f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[10] = mic_cst((xyz_t) { .x = -0.0922f, .y = -0.0247f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[11] = mic_cst((xyz_t) { .x = -0.0922f, .y = +0.0247f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");

    obj->mics[12] = mic_cst((xyz_t) { .x = -0.0400f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[13] = mic_cst((xyz_t) { .x = +0.0000f, .y = +0.0400f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[14] = mic_cst((xyz_t) { .x = +0.0400f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[15] = mic_cst((xyz_t) { .x = +0.0000f, .y = -0.0400f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");

    return obj;

}

static mics_t * mics_vibeus_circular(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 6;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = -0.0450f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[1] = mic_cst((xyz_t) { .x = -0.0225f, .y = -0.0390f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.0225f, .y = +0.0390f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[3] = mic_cst((xyz_t) { .x = -0.0225f, .y = +0.0390f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[4] = mic_cst((xyz_t) { .x = +0.0225f, .y = -0.0390f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[5] = mic_cst((xyz_t) { .x = +0.0450f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");

    return obj;

}

static mics_t * mics_soundskrit_mug(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 3;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = +0.03750f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[1] = mic_cst((xyz_t) { .x = -0.01875f, .y = +0.0325f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");
    obj->mics[2] = mic_cst((xyz_t) { .x = -0.01875f, .y = -0.0325f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, "omnidirectional");

    return obj;

}
