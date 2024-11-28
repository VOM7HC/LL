#include "config.h"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "babl-base.hpp"
#include "babl-internal.hpp"

static inline void convert_double_u32_scaled(BablConversion* c,
    double min_val,
    double max_val,
    uint32_t min,
    uint32_t max,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        double dval = *(double*)src;
        uint32_t u32val;

        if (dval < min_val)
            u32val = min;
        else if (dval > max_val)
            u32val = max;
        else
            u32val = (dval - min_val) / (max_val - min_val) * (max - min) + min + 0.5;

        *(uint32_t*)dst = u32val;
        dst += dst_pitch;
        src += src_pitch;
    }
}

static inline void
convert_u32_double_scaled(BablConversion* c,
    double min_val,
    double max_val,
    uint32_t min,
    uint32_t max,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        uint32_t u32val = *(uint32_t*)src;
        double dval;

        if (u32val < min)
            dval = min_val;
        else if (u32val > max)
            dval = max_val;
        else
            dval = (u32val - min) / (double)(max - min) * (max_val - min_val) + min_val;

        (*(double*)dst) = dval;
        dst += dst_pitch;
        src += src_pitch;
    }
}

#define MAKE_CONVERSIONS(name, min_val, max_val, min, max)       \
    static void                                                  \
        convert_##name##_double(BablConversion* c, void* src,    \
            void* dst,                                           \
            int src_pitch,                                       \
            int dst_pitch,                                       \
            long n)                                              \
    {                                                            \
        convert_u32_double_scaled(c, min_val, max_val, min, max, \
            src, dst, src_pitch, dst_pitch, n);                  \
    }                                                            \
    static void                                                  \
        convert_double_##name(BablConversion* c, void* src,      \
            void* dst,                                           \
            int src_pitch,                                       \
            int dst_pitch,                                       \
            long n)                                              \
    {                                                            \
        convert_double_u32_scaled(c, min_val, max_val, min, max, \
            src, dst, src_pitch, dst_pitch, n);                  \
    }

MAKE_CONVERSIONS(u32, 0.0, 1.0, 0, UINT32_MAX)

static inline void
convert_float_u32_scaled(BablConversion* c,
    float min_val,
    float max_val,
    uint32_t min,
    uint32_t max,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        float dval = *(float*)src;
        uint32_t u32val;

        if (dval < min_val)
            u32val = min;
        else if (dval > max_val)
            u32val = max;
        else
            u32val = (dval - min_val) / (max_val - min_val) * (max - min) + min + 0.5f;

        *(uint32_t*)dst = u32val;
        dst += dst_pitch;
        src += src_pitch;
    }
}

static inline void
convert_u32_float_scaled(BablConversion* c,
    float min_val,
    float max_val,
    uint32_t min,
    uint32_t max,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        uint32_t u32val = *(uint32_t*)src;
        float dval;

        if (u32val < min)
            dval = min_val;
        else if (u32val > max)
            dval = max_val;
        else
            dval = (u32val - min) / (float)(max - min) * (max_val - min_val) + min_val;

        (*(float*)dst) = dval;
        dst += dst_pitch;
        src += src_pitch;
    }
}

#define MAKE_CONVERSIONS_float(name, min_val, max_val, min, max) \
    static void                                                  \
        convert_##name##_float(BablConversion* c, void* src,     \
            void* dst,                                           \
            int src_pitch,                                       \
            int dst_pitch,                                       \
            long n)                                              \
    {                                                            \
        convert_u32_float_scaled(c, min_val, max_val, min, max,  \
            src, dst, src_pitch, dst_pitch, n);                  \
    }                                                            \
    static void                                                  \
        convert_float_##name(BablConversion* c, void* src,       \
            void* dst,                                           \
            int src_pitch,                                       \
            int dst_pitch,                                       \
            long n)                                              \
    {                                                            \
        convert_float_u32_scaled(c, min_val, max_val, min, max,  \
            src, dst, src_pitch, dst_pitch, n);                  \
    }

MAKE_CONVERSIONS_float(u32, 0.0, 1.0, 0, UINT32_MAX)

    void BABL_SIMD_SUFFIX(babl_base_type_u32)(void)
{
    babl_type_new(
        "u32",
        "id", BABL_U32,
        "bits", 32,
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_U32),
        babl_type_from_id(BABL_DOUBLE),
        "plane", convert_u32_double,
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_DOUBLE),
        babl_type_from_id(BABL_U32),
        "plane", convert_double_u32,
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_U32),
        babl_type_from_id(BABL_FLOAT),
        "plane", convert_u32_float,
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_FLOAT),
        babl_type_from_id(BABL_U32),
        "plane", convert_float_u32,
        NULL);
}
