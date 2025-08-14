#include <utils/openmp.h>

#ifdef OPENMP_ENABLED
#include <omp.h>

#define MAX_THREAD_COUNT 4

uint16_t get_thread_count(void)
{
    static int initialized = 0;
    static uint16_t thread_count = 0;

    if (!initialized) {
        thread_count = omp_get_num_procs();
        if (thread_count > MAX_THREAD_COUNT) {
            thread_count = MAX_THREAD_COUNT;
        }
        initialized = 1;
    }

    return thread_count;
}

#else

uint16_t get_thread_count(void)
{
    return 1;
}

int omp_get_thread_num(void)
{
    return 0;
}
#endif
