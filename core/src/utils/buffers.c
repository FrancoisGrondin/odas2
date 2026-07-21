#include <utils/buffers.h>

#include <stdlib.h>

fifo_t * fifo_construct(const unsigned int max_elements) {

	fifo_t * obj = (fifo_t *) malloc(sizeof(fifo_t));

	obj->buffer = (void **) calloc(max_elements, sizeof(void *));

	obj->max_elements = max_elements;
	obj->num_elements = 0;

	return obj;

}

void fifo_destroy(fifo_t * obj) {

	free((void *) obj->buffer);
	free((void *) obj);

}

int fifo_push(fifo_t * obj, void * ptr) {

	int rtn = -1;

	if (obj->num_elements != obj->max_elements) {
		obj->buffer[obj->num_elements] = ptr;
		obj->num_elements++;
		rtn = 0;
	}

	return rtn;

}

void * fifo_pop(fifo_t * obj) {

	void * rtn = NULL;

	if (obj->num_elements > 0) {

		rtn = obj->buffer[0];

		for (unsigned int index_element = 1; index_element < obj->num_elements; index_element++) {
			obj->buffer[index_element-1] = obj->buffer[index_element];
		}
		obj->buffer[obj->num_elements-1] = NULL;

		obj->num_elements--;

	}

	return rtn;

}