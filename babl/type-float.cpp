#include "config.h"
#include <cassert>
#include <cstring>

#include "babl-base.hpp"
#include "babl-classes.hpp"
#include "babl-ids.hpp"
#include "babl.hpp"

static void convert_double_float(BablConversion* conversion,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        (*(float*)dst) = (*(double*)src);
        dst += dst_pitch;
        src += src_pitch;
    }
}

static void
convert_float_double(BablConversion* conversion,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        (*(double*)dst) = (*(float*)src);
        dst += dst_pitch;
        src += src_pitch;
    }
}

static long
convert_float_float(const Babl* babl,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    if (src_pitch == 32 && dst_pitch == 32) {
        memcpy(dst, src, n / 4);
        return n;
    }

    while (n--) {
        (*(float*)dst) = (*(float*)src);
        dst += dst_pitch;
        src += src_pitch;
    }
    return n;
}

void BABL_SIMD_SUFFIX(babl_base_type_float)(void)
{
    babl_type_new(
        "float",
        "id", BABL_FLOAT,
        "bits", 32,
        "doc", "IEEE 754 single precision",
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_FLOAT),
        babl_type_from_id(BABL_DOUBLE),
        "plane", convert_float_double,
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_DOUBLE),
        babl_type_from_id(BABL_FLOAT),
        "plane", convert_double_float,
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_FLOAT),
        babl_type_from_id(BABL_FLOAT),
        "plane", convert_float_float,
        NULL);
}
