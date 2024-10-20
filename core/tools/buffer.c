#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef struct frame_t {

    size_t num_bytes;
    char bytes[128];

} frame_t;

typedef struct buffer_t {

    FILE * in;
    FILE * out;

    frame_t ** frames;

    frame_t read;
    frame_t write;

    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;

    unsigned int num_frames;
    unsigned int num_filled;

} buffer_t;

buffer_t * buffer_construct(const size_t num_frames, FILE * in, FILE * out);
void buffer_destroy(buffer_t * obj);
void * in2buffer(void * arg);
void * buffer2out(void * arg);

int main(int argc, char * argv[]) {

    const size_t num_frames = 256;

    pthread_t reader;
    pthread_t writer;

    FILE * in = fopen("/dev/stdin", "rb");
    FILE * out = fopen("/dev/stdout", "wb");

    buffer_t * buf = buffer_construct(num_frames, in, out);

    pthread_create(&reader, NULL, in2buffer, buf);
    pthread_create(&writer, NULL, buffer2out, buf);

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    buffer_destroy(buf);

    fclose(in);
    fclose(out);

}

buffer_t * buffer_construct(const size_t num_frames, FILE * in, FILE * out) {

    buffer_t * obj = (buffer_t *) malloc(sizeof(buffer_t));

    obj->in = in;
    obj->out = out;

    obj->frames = (frame_t **) malloc(sizeof(frame_t *) * num_frames);
    for (unsigned int index_frame = 0; index_frame < num_frames; index_frame++) {
        obj->frames[index_frame] = (frame_t *) malloc(sizeof(frame_t));
    }

    obj->num_frames = num_frames;
    obj->num_filled = 0;

    pthread_mutex_init(&obj->mutex, NULL);
    pthread_cond_init(&obj->not_empty, NULL);
    pthread_cond_init(&obj->not_full, NULL);

    return obj;

}

void buffer_destroy(buffer_t * obj) {

    pthread_mutex_destroy(&obj->mutex);
    pthread_cond_destroy(&obj->not_empty);
    pthread_cond_destroy(&obj->not_full);

    for (unsigned int index_frame = 0; index_frame < obj->num_frames; index_frame++) {
        free(obj->frames[index_frame]);
    }
    free(obj->frames);

    free(obj);

}

void * in2buffer(void * arg) {

    buffer_t * buf = (buffer_t *) arg;

    do {

        buf->read.num_bytes = fread(buf->read.bytes, sizeof(char), sizeof(buf->read.bytes), buf->in);

        pthread_mutex_lock(&buf->mutex);

        while (buf->num_filled == buf->num_frames) {
            pthread_cond_wait(&buf->not_full, &buf->mutex);
        }

        memcpy(buf->frames[buf->num_filled], &buf->read, sizeof(frame_t));
        buf->num_filled++;

        pthread_cond_signal(&buf->not_empty);
        pthread_mutex_unlock(&buf->mutex);

    } while(buf->read.num_bytes == sizeof(buf->read.bytes));

    return NULL;

}

void * buffer2out(void * arg) {

    buffer_t * buf = (buffer_t *) arg;

    do {

        pthread_mutex_lock(&buf->mutex);

        while (buf->num_filled == 0) {
            pthread_cond_wait(&buf->not_empty, &buf->mutex);
        }

        memcpy(&buf->write, buf->frames[0], sizeof(frame_t));
        frame_t * tmp = buf->frames[0];
        memmove(&(buf->frames[0]), &(buf->frames[1]), (buf->num_frames-1) * sizeof(frame_t *));
        buf->frames[buf->num_frames-1] = tmp;
        buf->num_filled--;

        pthread_cond_signal(&buf->not_full);
        pthread_mutex_unlock(&buf->mutex);

        fwrite(buf->write.bytes, sizeof(char), buf->write.num_bytes, buf->out);

    } while(buf->write.num_bytes == sizeof(buf->write.bytes));

    return NULL;

}