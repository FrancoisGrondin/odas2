#include <systems/postfilter.h>
#include <utils/error.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

postfilter_t * postfilter_construct(const unsigned int num_channels, const unsigned int num_bins) {
	if (num_channels < 1) {
		odas2_set_error_number(ODAS2_ERROR_POSTFILTER_CONSTRUCT_NUM_CHANNELS);
		return NULL;
	}
	if (num_bins < 1) {
		odas2_set_error_number(ODAS2_ERROR_POSTFILTER_CONSTRUCT_NUM_BINS);
		return NULL;
	}

	postfilter_t * obj = (postfilter_t *) malloc(sizeof(postfilter_t));

	obj->num_channels = num_channels;
	obj->num_bins = num_bins;

	return obj;

}

void postfilter_destroy(postfilter_t * obj) {

	free(obj);

}

int postfilter_process(postfilter_t * obj, const freqs_t * freqs_in, const masks_t * masks_in, freqs_t * freqs_out) {
	if (obj->num_channels != freqs_in->num_channels) {
		odas2_set_error_number(ODAS2_ERROR_POSTFILTER_PROCESS_FREQS_IN_NUM_CHANNELS);
		return -1;
	}
	if (obj->num_channels != masks_in->num_channels) {
		odas2_set_error_number(ODAS2_ERROR_POSTFILTER_PROCESS_MASKS_NUM_CHANNELS);
		return -1;
	}
	if (obj->num_channels != freqs_out->num_channels) {
		odas2_set_error_number(ODAS2_ERROR_POSTFILTER_PROCESS_FREQS_OUT_NUM_CHANNELS);
		return -1;
	}
	if (obj->num_bins != freqs_in->num_bins) {
		odas2_set_error_number(ODAS2_ERROR_POSTFILTER_PROCESS_FREQS_IN_NUM_BINS);
		return -1;
	}
	if (obj->num_bins != masks_in->num_bins) {
		odas2_set_error_number(ODAS2_ERROR_POSTFILTER_PROCESS_MASKS_NUM_BINS);
		return -1;
	}
	if (obj->num_bins != freqs_out->num_bins) {
		odas2_set_error_number(ODAS2_ERROR_POSTFILTER_PROCESS_FREQS_OUT_NUM_BINS);
		return -1;
	}

	#pragma omp parallel for collapse(2)
	for (unsigned int index_channel = 0; index_channel < obj->num_channels; index_channel++) {

		for (unsigned int index_bin = 0; index_bin < obj->num_bins; index_bin++) {

			cplx_t x = freqs_in->bins[index_channel][index_bin];
			float m = masks_in->gains[index_channel][index_bin];

			cplx_t y = cplx_mul(cplx_cst(m, 0.0f), x);

			freqs_out->bins[index_channel][index_bin] = y;

		}

	}

	return 0;

}
