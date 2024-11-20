#ifndef _BABL_CONVERSION_HPP
#define _BABL_CONVERSION_HPP

#include "babl-class.hpp"

BABL_CLASS_DECLARE(conversion);

const Babl* babl_conversion(const char* name);

typedef struct _BablConversion BablConversion;

typedef void (*BablFuncPlane)(BablConversion* conversion, const char* src, char* dst, int src_pitch, int dst_pitch, long n, void* user_data);

struct _BablConversion {
    BablInstance instance;
    const Babl* source;
    const Babl* destination;
    void (*dispatch)(const Babl* babl, const char* src, char* dst, long n, void* user_data);
    void* data;

    long cost;
    double error;
    union {
        BablFuncLinear linear;
        BablFuncPlane plane;
        BablFuncPlanar planar;
    } function;
    long pixels;
};

#endif
