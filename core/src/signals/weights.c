#include <signals/weights.h>
#include <utils/error.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

weights_t * weights_construct(const char * label, const unsigned int num_sources, const unsigned int num_channels, const unsigned int num_bins) {

	if (label == NULL || strlen(label) >= SIGNAL_LABEL_SIZE) {
		odas2_set_error_number(ODAS2_ERROR_WEIGHTS_CONSTRUCT_LABEL);
		return NULL;
	}
	if (num_sources < 1) {
		odas2_set_error_number(ODAS2_ERROR_WEIGHTS_CONSTRUCT_NUM_SOURCES);
		return NULL;
	}
	if (num_channels < 1) {
		odas2_set_error_number(ODAS2_ERROR_WEIGHTS_CONSTRUCT_NUM_CHANNELS);
		return NULL;
	}
	if (num_bins < 1) {
		odas2_set_error_number(ODAS2_ERROR_WEIGHTS_CONSTRUCT_NUM_BINS);
		return NULL;
	}

	weights_t * obj = (weights_t *) malloc(sizeof(weights_t));

	memset(obj->label, 0x00, SIGNAL_LABEL_SIZE);
	strcpy(obj->label, label);

	obj->num_sources = num_sources;
	obj->num_channels = num_channels;
	obj->num_bins = num_bins;

	obj->buffer = (cplx_t *) calloc(obj->num_sources * obj->num_channels * obj->num_bins, sizeof(cplx_t));
	obj->bins = (cplx_t ***) malloc(sizeof(cplx_t **) * obj->num_sources);
	for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {
		obj->bins[index_source] = (cplx_t **) malloc(sizeof(cplx_t *) * obj->num_channels);
		for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
			obj->bins[index_source][index_channel] = obj->buffer + index_source * obj->num_channels * obj->num_bins + index_channel * obj->num_bins;
		}
	}

	return obj;

}

void weights_destroy(weights_t * obj) {

	for (unsigned int index_source = 0; index_source < obj->num_sources; index_source++) {
		free(obj->bins[index_source]);
	}
	free(obj->bins);
	free(obj->buffer);

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
