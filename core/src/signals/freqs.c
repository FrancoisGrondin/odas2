#include <signals/freqs.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

freqs_t * freqs_construct(const char * label, const unsigned int num_channels, const unsigned int num_bins) {

	freqs_t * obj = (freqs_t *) malloc(sizeof(freqs_t));

	memset(obj->label, 0x00, 64);
	strcpy(obj->label, label);

	obj->num_channels = num_channels;
	obj->num_bins = num_bins;

	obj->bins = (cplx_t **) malloc(sizeof(cplx_t *) * num_channels);
	for (unsigned int index_channel = 0; index_channel < num_channels; index_channel++) {
		obj->bins[index_channel] = (cplx_t *) calloc(sizeof(cplx_t), num_bins);
	}

	return obj;

}

void freqs_destroy(freqs_t * obj) {

	for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
		free(obj->bins[index_channel]);
	}
	free(obj->bins);

	free(obj);

}

void freqs_fprintf(const freqs_t * obj, FILE * fp) {

    for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {
        for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {
            fprintf(fp, "[%u][%u]: %f + j %f\n", index_channel, index_bin, obj->bins[index_channel][index_bin].real, obj->bins[index_channel][index_bin].imag);
        }
    }

}