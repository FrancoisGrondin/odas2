#ifndef __DOAS_H
#define __DOAS_H

#include "../types/xyz.h"
#include <stdio.h>

typedef struct pot_t {

    xyz_t direction;
    float energy;

} pot_t;

typedef struct doas_t {

    char label[64];

    unsigned int num_directions;
    pot_t * pots;

} doas_t;

doas_t * doas_construct(const char * label, const unsigned int num_directions);

void doas_destroy(doas_t * obj);

void doas_target(doas_t * obj, const xyz_t * directions);

void doas_fprintf(const doas_t * obj, FILE * fp);

#endif // __DOAS_H