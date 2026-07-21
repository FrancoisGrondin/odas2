#ifndef __BUFFERS_H
#define __BUFFERS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fifo_t {

    void ** buffer;
    unsigned int num_elements;
    unsigned int max_elements;

} fifo_t;

fifo_t * fifo_construct(const unsigned int max_elements);

void fifo_destroy(fifo_t * obj);

int fifo_push(fifo_t * obj, void * ptr);

void * fifo_pop(fifo_t * obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif