#include <signals/imgs.h>
#include <utils/error.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

imgs_t * imgs_construct(const char * label, const unsigned int num_points, const unsigned int num_directions) {

    if (label == NULL || strlen(label) >= SIGNAL_LABEL_SIZE) {
        odas2_set_error_number(ODAS2_ERROR_IMGS_CONSTRUCT_LABEL);
        return NULL;
    }
    if (num_points < 1) {
        odas2_set_error_number(ODAS2_ERROR_IMGS_CONSTRUCT_NUM_POINTS);
        return NULL;
    }
    if (num_directions < 1) {
        odas2_set_error_number(ODAS2_ERROR_IMGS_CONSTRUCT_NUM_DIRECTIONS);
        return NULL;
    }

    imgs_t * obj = (imgs_t *) malloc(sizeof(imgs_t));

    memset(obj->label, 0x00, SIGNAL_LABEL_SIZE);
    strcpy(obj->label, label);

    obj->num_points = num_points;
    obj->num_directions = num_directions;
    obj->energies_buffer  = (float *) calloc(num_directions * num_points, sizeof(float));
    obj->energies = (float **) malloc(sizeof(float*) * num_directions);
    for (unsigned int index_direction = 0; index_direction < num_directions; index_direction++) {
        obj->energies[index_direction] = obj->energies_buffer + index_direction * num_points;
    }

    return obj;

}

void imgs_destroy(imgs_t * obj) {

    free(obj->energies);
    free(obj->energies_buffer);

    free(obj);

}
