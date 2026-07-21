#include <signals/freqs.h>
#include <utils/error.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

freqs_t * freqs_construct(const char * label, const unsigned int num_channels, const unsigned int num_bins) {

	if (label == NULL || strlen(label) >= SIGNAL_LABEL_SIZE) {
		odas2_set_error_number(ODAS2_ERROR_FREQS_CONSTRUCT_LABEL);
		return NULL;
	}
	if (num_channels < 1) {
		odas2_set_error_number(ODAS2_ERROR_FREQS_CONSTRUCT_NUM_CHANNELS);
		return NULL;
	}
	if (num_bins < 1) {
		odas2_set_error_number(ODAS2_ERROR_FREQS_CONSTRUCT_NUM_BINS);
		return NULL;
	}

	freqs_t * obj = (freqs_t *) malloc(sizeof(freqs_t));

	memset(obj->label, 0x00, SIGNAL_LABEL_SIZE);
	strcpy(obj->label, label);

	obj->num_channels = num_channels;
	obj->num_bins = num_bins;

	obj->buffer = (cplx_t *) calloc(num_channels * num_bins, sizeof(cplx_t));
	obj->bins = (cplx_t **) malloc(sizeof(cplx_t *) * num_channels);
	for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
		obj->bins[index_channel] = obj->buffer + index_channel * num_bins;
	}

	return obj;

}

void freqs_destroy(freqs_t * obj) {

	free(obj->bins);
	free(obj->buffer);

	free(obj);

}

void freqs_fprintf(const freqs_t * obj, FILE * fp) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
            fprintf(fp, "[%u][%u]: %f + j %f\n", index_channel, index_bin, obj->bins[index_channel][index_bin].real, obj->bins[index_channel][index_bin].imag);
        }
    }

}

unsigned int freqs_getsize(const freqs_t * obj) {

	return obj->num_channels * obj->num_bins * sizeof(cplx_t);

}
