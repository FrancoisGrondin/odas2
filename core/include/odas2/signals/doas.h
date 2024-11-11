#ifndef __DOAS_H
#define __DOAS_H

#include "../types/xyz.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {

    UNDEFINED,
    POTENTIAL,
    TRACKED,
    TARGET

} src_t;

typedef struct dir_t {

    src_t type;
    xyz_t coord;
    float energy;

} dir_t;

typedef struct doas_t {

    char label[64];

    unsigned int num_directions;
    dir_t * dirs;

} doas_t;

doas_t * doas_construct(const char * label, const unsigned int num_directions);

void doas_destroy(doas_t * obj);

void doas_target(doas_t * obj, const xyz_t * directions);

void doas_fprintf(const doas_t * obj, FILE * fp);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __DOAS_H
