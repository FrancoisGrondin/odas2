#include <utils/mics.h>
#include <utils/error.h>

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

mic_t mic_cst(const xyz_t position, const xyz_t direction, const mic_pattern_t pattern) {

    mic_t mic;

    mic.position = position;
    mic.direction = direction;
    mic.pattern = pattern;

    return mic;

}

float mic_gain(const mic_t mic, const xyz_t direction) {

    float cos_theta = xyz_dot(mic.direction, direction) / (xyz_mag(mic.direction) * xyz_mag(direction));

    float gain = 0.0f;

    if (mic.pattern == MIC_PATTERN_OMNIDIRECTIONAL) {
        gain = 1.0f;
    }
    else if (mic.pattern == MIC_PATTERN_CARDIOID) {
        gain = 0.5f + 0.5f * cos_theta;
    }

    return gain;

}

void mic_printf(const mic_t mic) {

    printf("position: (%+1.3f, %+1.3f, %+1.3f)\n", mic.position.x, mic.position.y, mic.position.z);
    printf("direction: (%+1.2f, %+1.2f, %+1.2f)\n", mic.direction.x, mic.direction.y, mic.direction.z);

    if (mic.pattern == MIC_PATTERN_OMNIDIRECTIONAL) {
        printf("pattern: OMNIDIRECTIONAL\n");
    }
    else if (mic.pattern == MIC_PATTERN_CARDIOID) {
        printf("pattern: CARDIOID\n");
    }
    else {
        printf("pattern: UNKNOWN\n");
    }

}

mics_t * mics_construct(const mics_hardware_t hardware) {

    switch (hardware) {
        case MICS_HARDWARE_RESPEAKER_USB_4:
            return mics_respeaker_usb_4();
        case MICS_HARDWARE_RESPEAKER_USB_6:
            return mics_respeaker_usb_6();
        case MICS_HARDWARE_MINIDSP_UMA:
            return mics_minidsp_uma();
        case MICS_HARDWARE_SC16_DEMO_ARRAY:
            return mics_sc16_demo_array();
        case MICS_HARDWARE_SC16F:
            return mics_sc16f();
        case MICS_HARDWARE_VIBEUS_CIRCULAR:
            return mics_vibeus_circular();
        case MICS_HARDWARE_SOUNDSKRIT_MUG:
            return mics_soundskrit_mug();
    }

    odas2_set_error_number(ODAS2_ERROR_MICS_HARDWARE_UNKNOWN);
    return NULL;

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

    obj->mics[0] = mic_cst((xyz_t) { .x = -0.032f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[1] = mic_cst((xyz_t) { .x = +0.000f, .y = -0.032f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.032f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.000f, .y = +0.032f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);

    return obj;

}

static mics_t * mics_respeaker_usb_6(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 6;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = -0.0232f, .y = +0.0401f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[1] = mic_cst((xyz_t) { .x = -0.0463f, .y = -0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[2] = mic_cst((xyz_t) { .x = -0.0232f, .y = -0.0401f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.0232f, .y = -0.0401f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[4] = mic_cst((xyz_t) { .x = +0.0463f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[5] = mic_cst((xyz_t) { .x = +0.0232f, .y = +0.0401f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);

    return obj;

}

static mics_t * mics_minidsp_uma(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 7;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = +0.000f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[1] = mic_cst((xyz_t) { .x = +0.000f, .y = +0.043f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.037f, .y = +0.021f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.037f, .y = -0.021f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[4] = mic_cst((xyz_t) { .x = +0.000f, .y = -0.043f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[5] = mic_cst((xyz_t) { .x = -0.037f, .y = -0.021f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);
    obj->mics[6] = mic_cst((xyz_t) { .x = -0.037f, .y = +0.021f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_CARDIOID);

    return obj;

}

static mics_t * mics_sc16_demo_array(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 16;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = +0.088f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[1] = mic_cst((xyz_t) { .x = +0.076f, .y = +0.044f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.044f, .y = +0.076f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.000f, .y = +0.088f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);

    obj->mics[4] = mic_cst((xyz_t) { .x = +0.000f, .y = +0.048f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[5] = mic_cst((xyz_t) { .x = -0.044f, .y = +0.076f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[6] = mic_cst((xyz_t) { .x = -0.076f, .y = +0.044f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[7] = mic_cst((xyz_t) { .x = -0.088f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);

    obj->mics[8] = mic_cst((xyz_t) { .x = -0.048f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[9] = mic_cst((xyz_t) { .x = -0.076f, .y = -0.044f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[10] = mic_cst((xyz_t) { .x = -0.044f, .y = -0.076f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[11] = mic_cst((xyz_t) { .x = +0.000f, .y = -0.088f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);

    obj->mics[12] = mic_cst((xyz_t) { .x = +0.000f, .y = -0.048f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[13] = mic_cst((xyz_t) { .x = +0.044f, .y = -0.076f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[14] = mic_cst((xyz_t) { .x = +0.076f, .y = -0.044f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[15] = mic_cst((xyz_t) { .x = +0.048f, .y = +0.000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);

    return obj;

}

static mics_t * mics_sc16f(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 16;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = -0.0675f, .y = +0.0675f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[1] = mic_cst((xyz_t) { .x = -0.0247f, .y = +0.0922f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.0247f, .y = +0.0922f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[3] = mic_cst((xyz_t) { .x = +0.0675f, .y = +0.0675f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);

    obj->mics[4] = mic_cst((xyz_t) { .x = +0.0922f, .y = +0.0247f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[5] = mic_cst((xyz_t) { .x = +0.0922f, .y = -0.0247f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[6] = mic_cst((xyz_t) { .x = +0.0675f, .y = -0.0675f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[7] = mic_cst((xyz_t) { .x = +0.0247f, .y = -0.0922f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);

    obj->mics[8] = mic_cst((xyz_t) { .x = -0.0247f, .y = -0.0922f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[9] = mic_cst((xyz_t) { .x = -0.0675f, .y = -0.0675f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[10] = mic_cst((xyz_t) { .x = -0.0922f, .y = -0.0247f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[11] = mic_cst((xyz_t) { .x = -0.0922f, .y = +0.0247f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);

    obj->mics[12] = mic_cst((xyz_t) { .x = -0.0400f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[13] = mic_cst((xyz_t) { .x = +0.0000f, .y = +0.0400f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[14] = mic_cst((xyz_t) { .x = +0.0400f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[15] = mic_cst((xyz_t) { .x = +0.0000f, .y = -0.0400f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);

    return obj;

}

static mics_t * mics_vibeus_circular(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 6;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = -0.0450f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[1] = mic_cst((xyz_t) { .x = -0.0225f, .y = -0.0390f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[2] = mic_cst((xyz_t) { .x = +0.0225f, .y = +0.0390f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[3] = mic_cst((xyz_t) { .x = -0.0225f, .y = +0.0390f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[4] = mic_cst((xyz_t) { .x = +0.0225f, .y = -0.0390f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[5] = mic_cst((xyz_t) { .x = +0.0450f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);

    return obj;

}

static mics_t * mics_soundskrit_mug(void) {

    mics_t * obj = (mics_t *) malloc(sizeof(mics_t));

    obj->num_mics = 3;
    obj->mics = (mic_t *) malloc(sizeof(mic_t) * obj->num_mics);

    obj->mics[0] = mic_cst((xyz_t) { .x = +0.03750f, .y = +0.0000f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[1] = mic_cst((xyz_t) { .x = -0.01875f, .y = +0.0325f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);
    obj->mics[2] = mic_cst((xyz_t) { .x = -0.01875f, .y = -0.0325f, .z = +0.000f }, (xyz_t) { .x = +0.000f, .y = +0.000f, .z = +1.000f }, MIC_PATTERN_OMNIDIRECTIONAL);

    return obj;

}
