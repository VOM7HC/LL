#include "babl-mutex.hpp"
#include "config.h"

#include <cstdlib>

#ifndef _WIN32

static const pthread_mutexattr_t*
get_mutex_attr(void)
{
    static pthread_mutexattr_t mutexattr;
    static int initialized = 0;

    if (!initialized) {
        /* On some platforms, this will keep an allocation till process
           termination, but it isn't a growing leak. */
        pthread_mutexattr_init(&mutexattr);
        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
        initialized = 1;
    }

    return &mutexattr;
}

#endif

BablMutex*
babl_mutex_new(void)
{
    BablMutex* mutex = malloc(sizeof(BablMutex));
#ifdef _WIN32
    InitializeCriticalSection(mutex);
#else
    pthread_mutex_init(mutex, get_mutex_attr());
#endif
    return mutex;
}

void babl_mutex_destroy(BablMutex* mutex)
{
#ifdef _WIN32
    DeleteCriticalSection(mutex);
#else
    pthread_mutex_destroy(mutex);
#endif
    free(mutex);
}

void babl_mutex_lock(BablMutex* mutex)
{
#ifdef _WIN32
    EnterCriticalSection(mutex);
#else
    pthread_mutex_lock(mutex);
#endif
}

void babl_mutex_unlock(BablMutex* mutex)
{
#ifdef _WIN32
    LeaveCriticalSection(mutex);
#else
    pthread_mutex_unlock(mutex);
#endif
}
