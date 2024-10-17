#ifndef __RING_H
#define __RING_H

#include <pthread.h>

typedef struct ring_t {

	unsigned int size;
	unsigned int count;
	unsigned int head;
	unsigned int tail;
	void ** array;

	pthread_mutex_t mutex;
	pthread_cond_t not_full;
	pthread_cond_t not_empty;

} ring_t;

ring_t * ring_construct(const unsigned int size);

void ring_destroy(ring_t * obj);

void ring_push(ring_t * obj, void * element);

void * ring_pop(ring_t * obj);

#endif // __RING_H