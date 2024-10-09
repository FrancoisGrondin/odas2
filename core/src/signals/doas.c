#include <signals/doas.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

doas_t * doas_construct(const char * label, const unsigned int num_directions) {

    doas_t * obj = (doas_t *) malloc(sizeof(doas_t));

    memset(obj->label, 0x00, 64);
    strcpy(obj->label, label);

    obj->num_directions = num_directions;
    obj->pots = (pot_t *) calloc(sizeof(pot_t), num_directions);

    return obj;

}

void doas_destroy(doas_t * obj) {

    free(obj->pots);
    free(obj);

}

void doas_target(doas_t * obj, const xyz_t * directions) {

    for (unsigned int index_direction = 0; index_direction < obj->num_directions; index_direction++) {
        obj->pots[index_direction].direction = xyz_unit(directions[index_direction]);
        obj->pots[index_direction].energy = 1.0f;
    }
    
}

void doas_fprintf(const doas_t * obj, FILE * fp) {

    for (unsigned int index_direction = 0; index_direction < obj->num_directions; index_direction++) {
        fprintf(fp, "[%02u]: (%+1.3f, %+1.3f, %+1.3f) > %+1.3f\n", index_direction, 
                                                                   obj->pots[index_direction].direction.x,
                                                                   obj->pots[index_direction].direction.y,
                                                                   obj->pots[index_direction].direction.z,
                                                                   obj->pots[index_direction].energy);
    }

}