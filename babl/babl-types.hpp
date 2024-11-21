#ifndef _BABL_TYPES_HPP
#define _BABL_TYPES_HPP

#if !defined(BABL_INSIDE_BABL_H) && !defined(BABL_IS_BEING_COMPILED)
#error "babl-types.h must not be included directly, include babl.h instead."
#endif

typedef union _Babl Babl;

typedef void (*BablFuncLinear)(const Babl* conversion, const char* src, char* dst, long n, void* user_data);

typedef void (*BablFuncPlanar)(const Babl* conversion, int src_bands, const char* src[], int src_pitch[], int dst_bands, char* dst[], int dst_pitch[], long n, void* user_data);

#endif
