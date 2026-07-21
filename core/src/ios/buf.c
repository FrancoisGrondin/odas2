#include <ios/buf.h>

#include <stdlib.h>

buf_t * buf_construct(const unsigned int num_bytes) {

	buf_t * obj = (buf_t *) malloc(sizeof(buf_t));

	obj->buffer = fifo_construct(num_bytes);

	return obj;

}

void buf_destroy(buf_t * obj) {

	fifo_destroy(obj->buffer);

	free((void *) obj);

}

int buf_write_freqs(buf_t * obj, const freqs_t * freqs) {

	return fifo_push(obj->buffer, (const char *) freqs->buffer, freqs->num_channels * freqs->num_bins * sizeof(cplx_t));

}

int buf_read_freqs(buf_t * obj, freqs_t * freqs) {

	return fifo_pop(obj->buffer, (char *) freqs->buffer, freqs->num_channels * freqs->num_bins * sizeof(cplx_t));

}
