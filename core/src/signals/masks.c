#include <signals/masks.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

masks_t * masks_construct(const char * label, const unsigned int num_channels, const unsigned int num_bins) {

    masks_t * obj = (masks_t *) malloc(sizeof(masks_t));

    memset(obj->label, 0x00, 64);
    strcpy(obj->label, label);

    obj->num_channels = num_channels;
    obj->num_bins = num_bins;

    obj->gains = (float **) malloc(sizeof(float *) * num_channels);
    for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
        obj->gains[index_channel] = calloc(num_bins, sizeof(float));
    }

    return obj;

}

void masks_destroy(masks_t * obj) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        free(obj->gains[index_channel]);
    }
    free(obj->gains);

    free(obj);

}

void masks_ones(masks_t * obj) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
            obj->gains[index_channel][index_bin] = 1.0f;
        }
    }

}

void masks_zeros(masks_t * obj) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
            obj->gains[index_channel][index_bin] = 0.0f;
        }
    }

}

void masks_fprintf(const masks_t * obj, FILE * fp) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
            fprintf(fp, "[%u][%u]: %f\n", index_channel, index_bin, obj->gains[index_channel][index_bin]);
        }
    }    

}