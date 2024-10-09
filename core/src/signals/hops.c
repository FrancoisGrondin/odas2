#include <signals/hops.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

hops_t * hops_construct(const char * label, const unsigned int num_channels, const unsigned int num_shifts) {

    hops_t * obj = (hops_t *) malloc(sizeof(hops_t));

    memset(obj->label, 0x00, 64);
    strcpy(obj->label, label);

    obj->num_channels = num_channels;
    obj->num_shifts = num_shifts;

    obj->samples = (float **) malloc(sizeof(float *) * num_channels);
    for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
        obj->samples[index_channel] = (float *) calloc(sizeof(float), num_shifts);
    }

    return obj;

}

void hops_destroy(hops_t * obj) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        free(obj->samples[index_channel]);
    }
    free(obj->samples);

    free(obj);

}

void hops_fprintf(const hops_t * obj, FILE * fp) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        for (unsigned int index_sample = 0; index_sample < obj->num_shifts; index_sample++) {
            fprintf(fp, "[%u][%u]: %f\n", index_channel, index_sample, obj->samples[index_channel][index_sample]);
        }
    }    

}