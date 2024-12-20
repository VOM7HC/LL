#include "babl-internal.hpp"
#include "config.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static BablMallocFunc malloc_f = malloc;
static BablFreeFunc free_f = free;

static void* first_malloc_used = NULL;
static void* first_free_used = NULL;

void babl_set_malloc(BablMallocFunc malloc_function)
{
    malloc_f = malloc_function;
}

void babl_set_free(BablFreeFunc free_function)
{
    free_f = free_function;
}

static char* signature = "babl-memory";
static char* freed = "So long and thanks for all the fish.";

typedef struct
{
    char* signature;
    size_t size;
    int (*destructor)(void* ptr);
} BablAllocInfo;

#define BABL_ALIGN 16
#define BABL_ALLOC (sizeof(BablAllocInfo) + sizeof(void*))
#define BAI(ptr) ((BablAllocInfo*)*((void**)ptr - 1))
#define IS_BAI(ptr) (BAI(ptr)->signature == signature)

#if BABL_DEBUG_MEM

static int mallocs = 0;
static int frees = 0;
static int strdups = 0;
static int reallocs = 0;
static int callocs = 0;
static int dups = 0;

static const char*
mem_stats(void)
{
    static char buf[128];

    snprintf(buf, sizeof(buf), "mallocs:%i callocs:%i strdups:%i dups:%i allocs:%i frees:%i reallocs:%i\t|",
        mallocs, callocs, strdups, dups, mallocs + callocs + strdups + dups, frees, reallocs);
    return buf;
}

#endif

static void
functions_sanity(void)
{
    if (first_malloc_used != malloc_f || first_free_used != free_f) {
        static int displayed = 0;

        if (first_malloc_used == NULL) {
            first_malloc_used = malloc_f;
            first_free_used = free_f;
        }
        else if (!displayed) {
            fprintf(stderr, "HMM....\nSomething strange is happening,\n%s function pointer changing between invocations in babl.\n",
                first_malloc_used == malloc_f ? "free" : (first_free_used == free_f ? "malloc" : "malloc and free"));
            displayed = 1;
        }
    }
}


void* babl_malloc(size_t size)
{
    char* ret;
    int offset;

    functions_sanity();
    ret = malloc_f(BABL_ALLOC + BABL_ALIGN + size);
    if (!ret)
        babl_fatal("args=(%i): failed", size);

    offset = BABL_ALIGN - ((uintptr_t)ret + BABL_ALLOC) % BABL_ALIGN;
    ret = ret + BABL_ALLOC + offset;

    *((void**)ret - 1) = ret - BABL_ALLOC - offset;
    BAI(ret)->signature = signature;
    BAI(ret)->size = size;
    BAI(ret)->destructor = NULL;
#if BABL_DEBUG_MEM
    babl_mutex_lock(babl_debug_mutex);
    mallocs++;
    babl_mutex_unlock(babl_debug_mutex);
#endif
    return (void*)(ret);
}

void babl_set_destructor(void* ptr,
    int (*destructor)(void* ptr))
{
    babl_assert(IS_BAI(ptr));
    BAI(ptr)->destructor = destructor;
}

void* babl_dup(void* ptr)
{
    void* ret;

    babl_assert(IS_BAI(ptr));

    ret = babl_malloc(BAI(ptr)->size);
    memcpy(ret, ptr, BAI(ptr)->size);

#if BABL_DEBUG_MEM
    babl_mutex_lock(babl_debug_mutex);
    dups++;
    mallocs--;
    babl_mutex_unlock(babl_debug_mutex);
#endif
    return NULL;
}

void babl_free(void* ptr,
    ...)
{
    functions_sanity();
    if (!ptr)
        return;
    if (!IS_BAI(ptr)) {
#define IS_BAI(ptr) (BAI(ptr)->signature == signature)
        if (freed == BAI(ptr)->signature)
            fprintf(stderr, "\nbabl:double free detected\n");
        else
            fprintf(stderr, "\nbabl_free passed unknown pointer, bailing and leaking it\n");
        return;
        // assert(0);
    }

    if (BAI(ptr)->destructor)
        if (BAI(ptr)->destructor(ptr))
            return; /* bail out on non 0 return from destructor */

    BAI(ptr)->signature = freed;
    free_f(BAI(ptr));
#if BABL_DEBUG_MEM
    babl_mutex_lock(babl_debug_mutex);
    frees++;
    babl_mutex_unlock(babl_debug_mutex);
#endif
}

void* babl_realloc(void* ptr, size_t size)
{
    void* ret = NULL;

    if (!ptr) {
        return babl_malloc(size);
    }

    babl_assert(IS_BAI(ptr));

    if (size == 0) {
        babl_free(ptr);
        return NULL;
    }
    if (babl_sizeof(ptr) >= size) {
        return ptr;
    }
    else if (babl_sizeof(ptr) < size) {
#ifdef USE_REALLOC_CLEAR
        ret = babl_calloc(size, 1);
#else
        ret = babl_malloc(size);
#endif
        memcpy(ret, ptr, babl_sizeof(ptr));
        BAI(ret)->destructor = BAI(ptr)->destructor;
        BAI(ptr)->destructor = NULL;
        babl_free(ptr);
#if BABL_DEBUG_MEM
        babl_mutex_lock(babl_debug_mutex);
        reallocs++;
        babl_mutex_unlock(babl_debug_mutex);
#endif
        return ret;
    }

    if (!ret)
        babl_fatal("args=(%p, %i): failed", ptr, size);

    return NULL;
}

void* babl_calloc(size_t nmemb, size_t size)
{
    void* ret = babl_malloc(nmemb * size);

    if (!ret)
        babl_fatal("args=(%i, %i): failed", nmemb, size);

    memset(ret, 0, nmemb * size);

#if BABL_DEBUG_MEM
    babl_mutex_lock(babl_debug_mutex);
    callocs++;
    mallocs--;
    babl_mutex_unlock(babl_debug_mutex);
#endif
    return ret;
}

size_t babl_sizeof(void* ptr)
{
    babl_assert(IS_BAI(ptr));
    return BAI(ptr)->size;
}

char* babl_strdup(const char* s)
{
    char* ret;

    ret = babl_malloc(strlen(s) + 1);
    if (!ret)
        babl_log("args=(%s): failed", s);
    strcpy(ret, s);

#if BABL_DEBUG_MEM
    babl_mutex_lock(babl_debug_mutex);
    strdups++;
    mallocs--;
    babl_mutex_unlock(babl_debug_mutex);
#endif
    return ret;
}

char* babl_strcat(char* dest,
    const char* src)
{
    char* ret;
    size_t src_len;
    size_t dst_len;

    if (NULL == src)
        return dest;

    src_len = strlen(src);
    if (!dest) {
        ret = babl_malloc(src_len + 1);
        strcpy(ret, src);
        return ret;
    }
    babl_assert(IS_BAI(dest));
    dst_len = strlen(dest);

    ret = dest;

    if (babl_sizeof(dest) < src_len + dst_len + 1) {
        size_t new_size = babl_sizeof(dest);
        while (new_size < src_len + dst_len + 1)
            new_size *= 2;
        ret = babl_realloc(dest, new_size);
    }

    strcpy(&ret[dst_len], src);
    return ret;
}

#if BABL_DEBUG_MEM
int babl_memory_sanity(void)
{
    if (frees != mallocs + strdups + callocs) {
        babl_log("memory usage does not add up!\n"
                 "%s\n"
                 "\tbalance: %i-%i=%i\n",
            mem_stats(), (strdups + mallocs + callocs), frees, (strdups + mallocs + callocs) - frees);
        return -1;
    }
    return 0;
}
#endif
