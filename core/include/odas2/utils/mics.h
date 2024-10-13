#ifndef __MICS_H
#define __MICS_H

#include "../types/xyz.h"

typedef struct mic_t {

    xyz_t position;
    xyz_t direction;
    char pattern[32];

} mic_t;

typedef struct mics_t {

    unsigned int num_mics;
    mic_t * mics;

} mics_t;

mic_t mic_cst(const xyz_t position, const xyz_t direction, const char * pattern);

float mic_gain(const mic_t mic, const xyz_t direction);

void mic_printf(const mic_t mic);

mics_t * mics_construct(const char * hardware);

void mics_destroy(mics_t * obj);

static mics_t * mics_respeaker_usb(void);

static mics_t * mics_minidsp_uma(void);

static mics_t * mics_introlab_circular(void);

#endif // __MICS_H