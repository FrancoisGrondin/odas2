#ifndef __MICS_H
#define __MICS_H

#include "../types/xyz.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum mic_pattern_t {
    MIC_PATTERN_OMNIDIRECTIONAL,
    MIC_PATTERN_CARDIOID
} mic_pattern_t;

typedef struct mic_t {

    xyz_t position;
    xyz_t direction;
    mic_pattern_t pattern;

} mic_t;

typedef enum mics_hardware_t {
    MICS_HARDWARE_RESPEAKER_USB_4,
    MICS_HARDWARE_RESPEAKER_USB_6,
    MICS_HARDWARE_MINIDSP_UMA,
    MICS_HARDWARE_SC16_DEMO_ARRAY,
    MICS_HARDWARE_SC16F,
    MICS_HARDWARE_VIBEUS_CIRCULAR,
    MICS_HARDWARE_SOUNDSKRIT_MUG
} mics_hardware_t;


typedef struct mics_t {

    unsigned int num_mics;
    mic_t * mics;

} mics_t;

mic_t mic_cst(const xyz_t position, const xyz_t direction, const mic_pattern_t pattern);

float mic_gain(const mic_t mic, const xyz_t direction);

void mic_printf(const mic_t mic);

mics_t * mics_construct(const mics_hardware_t hardware);

mics_t * mics_construct_uninitialized(const unsigned int num_mics);

void mics_destroy(mics_t * obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __MICS_H
