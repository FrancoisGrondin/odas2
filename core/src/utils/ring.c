#include <utils/ring.h>

#include <stdlib.h>

ring_t * ring_construct(const unsigned int size) {

    ring_t * obj = (ring_t *) malloc(sizeof(ring_t));

    obj->size = size;
    obj->count = 0;
    obj->head = 0;
    obj->tail = 0;
    obj->array = (void **) calloc(size, sizeof(void *));

    pthread_mutex_init(&obj->mutex, NULL);
    pthread_cond_init(&obj->not_full, NULL);
    pthread_cond_init(&obj->not_empty, NULL);

    return obj;

}

void ring_destroy(ring_t * obj) {

    pthread_mutex_destroy(&obj->mutex);
    pthread_cond_destroy(&obj->not_full);
    pthread_cond_destroy(&obj->not_empty);

    free(obj->array);
    free(obj);

}

void ring_push(ring_t * obj, void * element) {

    pthread_mutex_lock(&obj->mutex);
    while (obj->count == obj->size) {
        pthread_cond_wait(&obj->not_full, &obj->mutex);
    }

    obj->array[obj->tail] = element;
    obj->tail = (obj->tail + 1) % obj->size;
    obj->count++;

    pthread_cond_signal(&obj->not_empty);
    pthread_mutex_unlock(&obj->mutex);

}

void * ring_pop(ring_t * obj) {

    pthread_mutex_lock(&obj->mutex);
    while (obj->count == 0) {
        pthread_cond_wait(&obj->not_empty, &obj->mutex);
    }

    void * element = obj->array[obj->head];
    obj->head = (obj->head + 1) % obj->size;
    obj->count--;

    pthread_cond_signal(&obj->not_full);
    pthread_mutex_unlock(&obj->mutex);

    return element;

}
