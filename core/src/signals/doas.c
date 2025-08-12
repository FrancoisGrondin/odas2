#include <signals/doas.h>
#include <utils/error.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

doas_t * doas_construct(const char * label, const unsigned int num_directions) {

    if (label == NULL || strlen(label) >= SIGNAL_LABEL_SIZE) {
        odas2_set_error_number(ODAS2_ERROR_DOAS_CONSTRUCT_LABEL);
        return NULL;
    }
    if (num_directions < 1) {
        odas2_set_error_number(ODAS2_ERROR_DOAS_CONSTRUCT_NUM_DIRECTIONS);
        return NULL;
    }

    doas_t * obj = (doas_t *) malloc(sizeof(doas_t));

    memset(obj->label, 0x00, SIGNAL_LABEL_SIZE);
    strcpy(obj->label, label);

    obj->num_directions = num_directions;
    obj->dirs = (dir_t *) calloc(sizeof(dir_t), num_directions);

    return obj;

}

void doas_destroy(doas_t * obj) {

    free(obj->dirs);
    free(obj);

}

void doas_target(doas_t * obj, const xyz_t * directions) {

    for (unsigned int index_direction = 0; index_direction < obj->num_directions; index_direction++) {
        obj->dirs[index_direction].type = TARGET;
        obj->dirs[index_direction].coord = xyz_unit(directions[index_direction]);
        obj->dirs[index_direction].energy = 1.0f;
    }

}

void doas_fprintf(const doas_t * obj, FILE * fp) {

    for (unsigned int index_direction = 0; index_direction < obj->num_directions; index_direction++) {
        fprintf(fp, "(%+1.3f, %+1.3f, %+1.3f) > %+1.3f\n",
            obj->dirs[index_direction].coord.x,
            obj->dirs[index_direction].coord.y,
            obj->dirs[index_direction].coord.z,
            obj->dirs[index_direction].energy);
    }

}
