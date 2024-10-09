#include <signals/tdoas.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tdoas_t * tdoas_construct(const char * label, const unsigned int num_channels, const unsigned int num_sources) {

    tdoas_t * obj = (tdoas_t *) malloc(sizeof(tdoas_t));

    memset(obj->label, 0x00, 64);
    strcpy(obj->label, label);

    obj->num_sources = num_sources;
    obj->num_channels = num_channels;
    obj->num_pairs = num_channels * (num_channels - 1) / 2;

    obj->taus = (tau_t **) malloc(sizeof(tau_t *) * num_sources);
    for (unsigned int index_source = 0; index_source < num_sources; index_source++) {
        obj->taus[index_source] = (tau_t *) calloc(sizeof(tau_t), obj->num_pairs);
    }

    return obj;

}

void tdoas_destroy(tdoas_t * obj) {

    for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {
        free(obj->taus[index_source]);
    }
    free(obj->taus);

    free(obj);

}

void tdoas_fprintf(const tdoas_t * obj, FILE * fp) {

    for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {

        printf("[%02u]: ", index_source);

        for (unsigned int index_pair = 0; index_pair < obj->num_pairs; index_pair++) {

            printf("(%+1.4f, %+1.4f) ", obj->taus[index_source][index_pair].delay, obj->taus[index_source][index_pair].amplitude);

        }

        printf("\n");

    }

}