#include <ios/raw.h>
#include <utils/error.h>

#include <stdlib.h>
#include <string.h>

rawout_t * rawout_construct(const char * file_name) {

	rawout_t * obj = (rawout_t *) malloc(sizeof(rawout_t));

    obj->file_pointer = fopen(file_name, "wb");
	if (obj->file_pointer == NULL) {
		odas2_set_error_number(ODAS2_ERROR_RAWOUT_CONSTRUCT_FILE_CREATION);
		free((void*)obj);
		return NULL;
	}

	return obj;

}

void rawout_destroy(rawout_t * obj) {

	fclose(obj->file_pointer);

	free(obj);

}

int rawout_write_hops(rawout_t * obj, const hops_t * hops) {

	for (unsigned int index_channel = 0; index_channel < hops->num_channels; index_channel++) {
		if (fwrite(hops->samples[index_channel], sizeof(float), hops->num_shifts, obj->file_pointer) != hops->num_shifts) {
			odas2_set_error_number(ODAS2_ERROR_RAWOUT_WRITE);
			return -1;
		}
	}

	return 0;

}

int rawout_write_freqs(rawout_t * obj, const freqs_t * freqs) {

	for (unsigned int index_channel = 0; index_channel < freqs->num_channels; index_channel++) {
		if (fwrite(freqs->bins[index_channel], sizeof(cplx_t), freqs->num_bins, obj->file_pointer) != freqs->num_bins) {
			odas2_set_error_number(ODAS2_ERROR_RAWOUT_WRITE);
			return -1;
		}
	}

	return 0;

}

int rawout_write_covs(rawout_t * obj, const covs_t * covs) {

	for (unsigned int index_pair = 0; index_pair < covs->num_pairs; index_pair++) {
		if (fwrite(covs->xcorrs[index_pair], sizeof(cplx_t), covs->num_bins, obj->file_pointer) != covs->num_bins) {
			odas2_set_error_number(ODAS2_ERROR_RAWOUT_WRITE);
			return -1;
		}
	}
	for (unsigned int index_channel = 0; index_channel < covs->num_channels; index_channel++) {
		if (fwrite(covs->acorrs[index_channel], sizeof(float), covs->num_bins, obj->file_pointer) != covs->num_bins) {
			odas2_set_error_number(ODAS2_ERROR_RAWOUT_WRITE);
			return -1;
		}
	}

	return 0;

}

int rawout_write_masks(rawout_t * obj, const masks_t * masks) {

	for (unsigned int index_channel = 0; index_channel < masks->num_channels; index_channel++) {
		if (fwrite(masks->gains[index_channel], sizeof(float), masks->num_bins, obj->file_pointer) != masks->num_bins) {
			odas2_set_error_number(ODAS2_ERROR_RAWOUT_WRITE);
			return -1;
		}
	}

	return 0;

}

int rawout_write_doas(rawout_t * obj, const doas_t * doas) {

	for (unsigned int index_direction = 0; index_direction < doas->num_directions; index_direction++) {
		if (fwrite(&(doas->dirs[index_direction]), sizeof(dir_t), 1, obj->file_pointer) != 1) {
			odas2_set_error_number(ODAS2_ERROR_RAWOUT_WRITE);
			return -1;
		}
	}

	return 0;

}

int rawout_write_tdoas(rawout_t * obj, const tdoas_t * tdoas) {

	for (unsigned int index_source = 0; index_source < tdoas->num_sources; index_source++) {
		if (fwrite(tdoas->taus[index_source], sizeof(tau_t), tdoas->num_pairs, obj->file_pointer) != tdoas->num_pairs) {
			odas2_set_error_number(ODAS2_ERROR_RAWOUT_WRITE);
			return -1;
		}
	}

	return 0;

}
