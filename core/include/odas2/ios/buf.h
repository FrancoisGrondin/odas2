#ifndef __BUF_H
#define __BUF_H

#include "../utils/buffers.h"
#include "../signals/freqs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct buf_t {

    fifo_t * buffer;

} buf_t;

buf_t * buf_construct(const unsigned int num_bytes);

void buf_destroy(buf_t * obj);

int buf_write_freqs(buf_t * obj, const freqs_t * freqs);

int buf_read_freqs(buf_t * obj, freqs_t * freqs);

#ifdef __cplusplus
} //extern "C"
#endif

#endif // __LAG_H    

