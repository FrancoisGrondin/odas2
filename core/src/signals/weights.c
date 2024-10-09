#include <signals/weights.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

weights_t * weights_construct(const char * label, const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins) {

	weights_t * obj = (weights_t *) malloc(sizeof(weights_t));

	memset(obj->label, 0x00, 64);
	strcpy(obj->label, label);

	obj->num_sources = num_sources;
	obj->num_channels = num_channels;
	obj->num_bins = num_bins;

	obj->bins = (cplx_t ***) malloc(sizeof(cplx_t **) * num_sources);
	for (unsigned int index_source = 0; index_source < num_sources; index_source++) {
		obj->bins[index_source] = (cplx_t **) malloc(sizeof(cplx_t *) * num_channels);
		for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
			obj->bins[index_source][index_channel] = (cplx_t *) calloc(sizeof(cplx_t), num_bins);
		}
	}

	return obj;

}

void weights_destroy(weights_t * obj) {

	for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {
		for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
			free(obj->bins[index_source][index_channel]);
		}
		free(obj->bins[index_source]);
	}
	free(obj->bins);

	free(obj);

}

void weights_fprintf(const weights_t * obj, FILE * fp) {

	for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {
		for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
			for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
				cplx_t value = obj->bins[index_source][index_channel][index_bin];
				fprintf(fp, "(%u, %u)[%03u]: %+1.3f + j %+1.3f\n", index_source, index_channel, index_bin, value.real, value.imag);
			}
		}
	}

}