#include "config.h"
#include <cassert>
#include <cstdint>
#include <cstring>

#include "babl-base.hpp"
#include "babl-internal.hpp"

#include <cmath>

static inline void convert_double_u8_scaled(BablConversion* c,
    double min_val,
    double max_val,
    uint8_t min,
    uint8_t max,
    int8_t* src,
    int8_t* dst,
    int32_t src_pitch,
    int32_t dst_pitch,
    int64_t n)
{
    while (n--) {
        double dval = *(double*)src;
        unsigned char u8val;

        if (dval < min_val)
            u8val = min;
        else if (dval > max_val)
            u8val = max;
        else
            u8val = ((dval - min_val) / (max_val - min_val) * (max - min) + min) + 0.5;

        *(unsigned char*)dst = u8val;
        src += src_pitch;
        dst += dst_pitch;
    }
}

static inline void convert_u8_double_scaled(BablConversion* c,
    double min_val,
    double max_val,
    unsigned char min,
    unsigned char max,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        int u8val = *(unsigned char*)src;
        double dval;

        if (u8val < min)
            dval = min_val;
        else if (u8val > max)
            dval = max_val;
        else
            dval = (u8val - min) / (double)(max - min) * (max_val - min_val) + min_val;

        (*(double*)dst) = dval;

        dst += dst_pitch;
        src += src_pitch;
    }
}

#define MAKE_CONVERSIONS(name, min_val, max_val, min, max)      \
    static void                                                 \
        convert_##name##_double(BablConversion* c, void* src,   \
            void* dst,                                          \
            int src_pitch,                                      \
            int dst_pitch,                                      \
            long n)                                             \
    {                                                           \
        convert_u8_double_scaled(c, min_val, max_val, min, max, \
            src, dst, src_pitch, dst_pitch, n);                 \
    }                                                           \
    static void                                                 \
        convert_double_##name(BablConversion* c, void* src,     \
            void* dst,                                          \
            int src_pitch,                                      \
            int dst_pitch,                                      \
            long n)                                             \
    {                                                           \
        convert_double_u8_scaled(c, min_val, max_val, min, max, \
            src, dst, src_pitch, dst_pitch, n);                 \
    }

MAKE_CONVERSIONS(u8, 0.0, 1.0, 0x00, UINT8_MAX)
MAKE_CONVERSIONS(u8_luma, 0.0, 1.0, 16, 235)
MAKE_CONVERSIONS(u8_chroma, -0.5, 0.5, 16, 240)

static inline void
convert_float_u8_scaled(BablConversion* c,
    double min_val,
    double max_val,
    unsigned char min,
    unsigned char max,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        float dval = *(float*)src;
        unsigned char u8val;

        if (dval < min_val)
            u8val = min;
        else if (dval > max_val)
            u8val = max;
        else
            u8val = (dval - min_val) / (max_val - min_val) * (max - min) + min + 0.5f;

        *(unsigned char*)dst = u8val;
        src += src_pitch;
        dst += dst_pitch;
    }
}

static inline void
convert_u8_float_scaled(BablConversion* c,
    double min_val,
    double max_val,
    unsigned char min,
    unsigned char max,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        int u8val = *(unsigned char*)src;
        float dval;

        if (u8val < min)
            dval = min_val;
        else if (u8val > max)
            dval = max_val;
        else
            dval = (u8val - min) / (float)(max - min) * (max_val - min_val) + min_val;

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
        convert_u8_float_scaled(c, min_val, max_val, min, max,   \
            src, dst, src_pitch, dst_pitch, n);                  \
    }                                                            \
    static void                                                  \
        convert_float_##name(BablConversion* c, void* src,       \
            void* dst,                                           \
            int src_pitch,                                       \
            int dst_pitch,                                       \
            long n)                                              \
    {                                                            \
        convert_float_u8_scaled(c, min_val, max_val, min, max,   \
            src, dst, src_pitch, dst_pitch, n);                  \
    }

MAKE_CONVERSIONS_float(u8, 0.0, 1.0, 0x00, UINT8_MAX)
MAKE_CONVERSIONS_float(u8_luma, 0.0, 1.0, 16, 235)
MAKE_CONVERSIONS_float(u8_chroma, -0.5, 0.5, 16, 240)

void BABL_SIMD_SUFFIX(babl_base_type_u8)(void)
{
    babl_type_new(
        "u8",
        "id", BABL_U8,
        "bits", 8,
        "doc", "uint8_t, 8 bit unsigned integer, values from 0-255",
        NULL);

    babl_type_new(
        "u8-luma",
        "id", BABL_U8_LUMA,
        "bits", 8,
        "doc", "8 bit unsigned integer, values from 16-235",
        NULL);

    babl_type_new(
        "u8-chroma",
        "id", BABL_U8_CHROMA,
        "integer",
        "unsigned",
        "bits", 8,
        "min", (long)16,
        "max", (long)240,
        "min_val", -0.5,
        "max_val", 0.5,
        "doc", "8 bit unsigned integer -0.5 to 0.5 maps to 16-240",
        NULL);
    babl_conversion_new(
        babl_type_from_id(BABL_U8),
        babl_type_from_id(BABL_DOUBLE),
        "plane", convert_u8_double,
        NULL);
    babl_conversion_new(
        babl_type_from_id(BABL_DOUBLE),
        babl_type_from_id(BABL_U8),
        "plane", convert_double_u8,
        NULL);
    babl_conversion_new(
        babl_type_from_id(BABL_U8_LUMA),
        babl_type_from_id(BABL_DOUBLE),
        "plane", convert_u8_luma_double,
        NULL);
    babl_conversion_new(
        babl_type_from_id(BABL_DOUBLE),
        babl_type_from_id(BABL_U8_LUMA),
        "plane", convert_double_u8_luma,
        NULL);
    babl_conversion_new(
        babl_type_from_id(BABL_U8_CHROMA),
        babl_type_from_id(BABL_DOUBLE),
        "plane", convert_u8_chroma_double,
        NULL);
    babl_conversion_new(
        babl_type_from_id(BABL_DOUBLE),
        babl_type_from_id(BABL_U8_CHROMA),
        "plane", convert_double_u8_chroma,
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_U8),
        babl_type_from_id(BABL_FLOAT),
        "plane", convert_u8_float,
        NULL);
    babl_conversion_new(
        babl_type_from_id(BABL_FLOAT),
        babl_type_from_id(BABL_U8),
        "plane", convert_float_u8,
        NULL);
    babl_conversion_new(
        babl_type_from_id(BABL_U8_LUMA),
        babl_type_from_id(BABL_FLOAT),
        "plane", convert_u8_luma_float,
        NULL);
    babl_conversion_new(
        babl_type_from_id(BABL_FLOAT),
        babl_type_from_id(BABL_U8_LUMA),
        "plane", convert_float_u8_luma,
        NULL);
    babl_conversion_new(
        babl_type_from_id(BABL_U8_CHROMA),
        babl_type_from_id(BABL_FLOAT),
        "plane", convert_u8_chroma_float,
        NULL);
    babl_conversion_new(
        babl_type_from_id(BABL_FLOAT),
        babl_type_from_id(BABL_U8_CHROMA),
        "plane", convert_float_u8_chroma,
        NULL);
}
