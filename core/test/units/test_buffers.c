#include "test_buffers.h"

int test_buffers(void) {

	{

		const unsigned int max_elements = 5;

		fifo_t * obj = fifo_construct(max_elements);

		void * ptr = fifo_pop(obj);

		if (!(ptr == NULL)) {
			return -1;
		}

		fifo_destroy(obj);

	}

	{

		const unsigned int max_elements = 5;
		const unsigned int num_elements = 5;

		void * ptrs[5] = { (void *) 0x01, (void *) 0x02, (void *) 0x03, (void *) 0x04, (void *) 0x05 };

		fifo_t * obj = fifo_construct(max_elements);

		for (unsigned int index_ptr = 0; index_ptr < num_elements; index_ptr++) {
			fifo_push(obj, ptrs[index_ptr]);	
		}

		for (unsigned int index_ptr = 0; index_ptr < num_elements; index_ptr++) {
			
			if (!(fifo_pop(obj) == ptrs[index_ptr])) {
				return -2;
			}

		}

		fifo_destroy(obj);

	}

	{

		const unsigned int max_elements = 5;

		fifo_t * obj = fifo_construct(max_elements);

		for (unsigned int index_ptr = 0; index_ptr < max_elements; index_ptr++) {
			fifo_push(obj, (void *) NULL);
		}

		if (!(fifo_push(obj, (void *) NULL) == -1)) {
			return -3;
		}

		fifo_destroy(obj);

	}

	return 0;

}