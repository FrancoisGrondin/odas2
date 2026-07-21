#include <utils/buffers.h>

#include <stdlib.h>
#include <string.h>

fifo_t * fifo_construct(const unsigned int max_bytes) {

	fifo_t * obj = (fifo_t *) malloc(sizeof(fifo_t));

	obj->bytes = (char *) calloc(max_bytes, sizeof(char));

	obj->max_bytes = max_bytes;
	obj->num_bytes = 0;

	return obj;

}

void fifo_destroy(fifo_t * obj) {

	free((void *) obj->bytes);
	free((void *) obj);

}

int fifo_push(fifo_t * obj, const char * bytes, const unsigned int num_bytes) {

	int rtn = -1;

	if (obj->num_bytes + num_bytes <= obj->max_bytes) {

		memcpy(&(obj->bytes[obj->num_bytes]), bytes, num_bytes * sizeof(char));
		obj->num_bytes += num_bytes;
		rtn = 0;

	}

	return rtn;

}

int fifo_pop(fifo_t * obj, char * bytes, const unsigned int num_bytes) {

	int rtn = -1;

	if (obj->num_bytes >= num_bytes) {

		memcpy(bytes, &(obj->bytes[0]), num_bytes * sizeof(char));
		memmove(&(obj->bytes[0]), &(obj->bytes[num_bytes]), (obj->num_bytes-num_bytes) * sizeof(char));
		memset(&(obj->bytes[obj->num_bytes-num_bytes]), 0x00, num_bytes * sizeof(char));
		obj->num_bytes -= num_bytes;

	}

	return rtn;

}