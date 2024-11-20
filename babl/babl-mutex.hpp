#ifndef _BABL_MUTEX_H
#define _BABL_MUTEX_H

#ifndef _WIN32
#define _GNU_SOURCE 1
#include <pthread.h>
#else
#include <windows.h>
#endif

#ifdef _WIN32
typedef CRITICAL_SECTION BablMutex;
#else
typedef pthread_mutex_t BablMutex;
#endif

BablMutex* babl_mutex_new(void);
void babl_mutex_destroy(BablMutex* mutex);
void babl_mutex_lock(BablMutex* mutex);
void babl_mutex_unlock(BablMutex* mutex);

#endif
