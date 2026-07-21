#include "test_buffers.h"

int test_buffers(void) {

	{

		const unsigned int max_bytes = 10;
		const unsigned int num_bytes = 15;
		const char bytes[15] = {0};

		fifo_t * obj = fifo_construct(max_bytes);

		if (!(fifo_push(obj, bytes, num_bytes) == -1)) {
			return -1;
		}

		fifo_destroy(obj);

	}

	{

		const unsigned int max_bytes = 10;
		const unsigned int num_bytes = 5;
		char bytes[5] = {0};

		fifo_t * obj = fifo_construct(max_bytes);

		if (!(fifo_pop(obj, bytes, num_bytes) == -1)) {
			return -2;
		}

		fifo_destroy(obj);		

	}

	{

		const unsigned int max_bytes = 10;
		const unsigned int num_bytes = 4;
		char bytes1[4] = {1,2,3,4};
		char bytes2[4] = {5,6,7,8};
		char bytes3[4] = {9,10,11,12};
		char bytes_dest[4];

		fifo_t * obj = fifo_construct(max_bytes);

		fifo_push(obj, bytes1, num_bytes);
		fifo_push(obj, bytes2, num_bytes);
		fifo_pop(obj, bytes_dest, num_bytes);

		for (unsigned int index_byte = 0; index_byte < num_bytes; index_byte++) {
			if (!(bytes_dest[index_byte] == bytes1[index_byte])) {
				return -3;
			}
		}

		fifo_push(obj, bytes3, num_bytes);
		fifo_pop(obj, bytes_dest, num_bytes);

		for (unsigned int index_byte = 0; index_byte < num_bytes; index_byte++) {
			if (!(bytes_dest[index_byte] == bytes2[index_byte])) {
				return -3;
			}
		}		

		fifo_pop(obj, bytes_dest, num_bytes);

		for (unsigned int index_byte = 0; index_byte < num_bytes; index_byte++) {
			if (!(bytes_dest[index_byte] == bytes3[index_byte])) {
				return -3;
			}
		}	

	}

	return 0;

}