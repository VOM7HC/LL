#ifndef _BABL_MEMORY_HPP
#define _BABL_MEMORY_HPP

typedef void* (*BablMallocFunc)(size_t size);
typedef void (*BablFreeFunc)(void* ptr);

void babl_set_malloc(BablMallocFunc malloc_function);
void babl_set_free(BablFreeFunc free_function);
int babl_memory_sanity(void);

void* babl_malloc(size_t size);
void babl_set_destructor(void* ptr, int (*destructor)(void* ptr));

void babl_free(void* ptr, ...);
void* babl_calloc(size_t nmemb, size_t size);
void* babl_realloc(void* ptr, size_t size);

size_t babl_sizeof(void* ptr);
void* babl_dup(void* ptr);

char* babl_strdup(const char* s);
char* babl_strcat(char* dest, const char* src);

#endif
