#ifndef _BABL_UTIL_HPP
#define _BABL_UTIL_HPP

#include <cstddef>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef _WIN32
typedef struct stat BablStat;
#else
typedef struct _stat64 BablStat;
#endif

long babl_ticks(void);

double babl_rel_avg_error(const double* imgA, const double* imgB, long samples);

size_t add_check_overflow(size_t numbers_count, ...);

size_t mul_check_overflow(size_t numbers_count, ...);

FILE* _babl_fopen(const char* path, const char* mode);

int _babl_remove(const char* path);

int _babl_rename(const char* oldname, const char* newname);

int _babl_stat(const char* path, BablStat* buffer);

int _babl_mkdir(const char* path, int mode);

typedef void (*_babl_dir_foreach_cb_t)(const char* base_path, const char* entry, void* data);

void _babl_dir_foreach(const char* path, _babl_dir_foreach_cb_t callback, void* user_data);

#ifdef _WIN32

wchar_t* babl_convert_utf8_to_utf16(const char* str);

char* babl_convert_utf16_to_utf8(const wchar_t* wstr);

void* get_libbabl_module(void);

#endif /* _WIN32 */

#endif
