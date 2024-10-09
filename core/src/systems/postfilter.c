#include <systems/postfilter.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>

postfilter_t * postfilter_construct(const unsigned int num_channels, const unsigned int num_bins) {

	postfilter_t * obj = (postfilter_t *) malloc(sizeof(postfilter_t));

	obj->num_channels = num_channels;
	obj->num_bins = num_bins;

	return obj;

}

void postfilter_destroy(postfilter_t * obj) {

	free(obj);

}

int postfilter_process(postfilter_t * obj, const freqs_t * freqs_in, const masks_t * masks_in, freqs_t * freqs_out) {

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