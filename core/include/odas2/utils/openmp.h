#ifndef __OPENMP_H
#define __OPENMP_H

#include <stdint.h>

#ifdef OPENMP_ENABLED
#include <omp.h>

inline uint16_t get_thread_count(void)
{
    uint16_t thread_count = 0;

#pragma omp parallel reduction(+ : thread_count)
    {
        thread_count += 1;
    }

    return thread_count;
}

#else

inline uint16_t get_thread_count(void)
{
    return 1;
}

inline int omp_get_thread_num(void)
{
    return 0;
}
#endif

#endif // __OPENMP_H
