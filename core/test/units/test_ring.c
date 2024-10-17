#include "test_ring.h"

static void * producer(void * arg);
static void * consumer(void * arg);

typedef struct container {

    ring_t * ring;
    void ** array_in;
    void ** array_out;
    unsigned int count;

} container;

int test_ring(void) {

    {

        container ctn;
        unsigned int size = 5;
        void * array_in[100] = { 0 };
        void * array_out[100] = { 0 };

        ctn.ring = ring_construct(size);
        ctn.array_in = array_in;
        ctn.array_out = array_out;
        ctn.count = 100;

        for (unsigned long index = 0; index < ctn.count; index++) {
            array_in[index] = (void *) index;
        }

        pthread_t prod_thread, cons_thread;
        pthread_create(&prod_thread, NULL, producer, &ctn);
        pthread_create(&cons_thread, NULL, consumer, &ctn);

        pthread_join(prod_thread, NULL);
        pthread_join(cons_thread, NULL);

        for (unsigned int index = 0; index < ctn.count; index++) {
            if (!(ctn.array_out[index] == ctn.array_in[index])) {
                return -1;
            }
        }

        ring_destroy(ctn.ring);

    }

    return 0;

}

static void * producer(void * arg) {

    container * ctn = (container *) arg;

    for (unsigned int index = 0; index < ctn->count; index++) {
        ring_push(ctn->ring, ctn->array_in[index]);
    }

    return NULL;

}

static void * consumer(void * arg) {

    container * ctn = (container *) arg;

    for (unsigned int index = 0; index < ctn->count; index++) {
        ctn->array_out[index] = ring_pop(ctn->ring);
    }

    return NULL;

}