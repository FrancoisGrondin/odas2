#ifndef __BUFFERS_H
#define __BUFFERS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct fifo_t {

    char * bytes;
    unsigned int num_bytes;
    unsigned int max_bytes;

} fifo_t;

fifo_t * fifo_construct(const unsigned int max_bytes);

void fifo_destroy(fifo_t * obj);

int fifo_push(fifo_t * obj, const char * bytes, const unsigned int num_bytes);

int fifo_pop(fifo_t * obj, char * bytes, const unsigned int num_bytes);

#ifdef __cplusplus
} //extern "C"
#endif

#endif