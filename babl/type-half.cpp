#include "config.h"
#include <cassert>
#include <cstdint>
#include <cstring>

#include "babl-internal.hpp"

#include "babl-base.hpp"
#include "babl-classes.hpp"
#include "babl-ids.hpp"
#include "babl.hpp"

static int next = 1;

static void doubles2halfp(void* target,
    void* source,
    long numel)
{
    uint16_t* hp = (uint16_t*)target; // Type pun output as an unsigned 16-bit int
    uint32_t* xp = (uint32_t*)source; // Type pun input as an unsigned 32-bit int
    uint16_t hs, he, hm;
    uint32_t x, xs, xe, xm;
    int hes;

    xp += next; // Little Endian adjustment if necessary

    if (source == NULL || target == NULL) { // Nothing to convert (e.g., imag part of pure real)
        return;
    }
    while (numel--) {
        x = *xp++;
        xp++;                            // The extra xp++ is to skip over the remaining 32 bits of the mantissa
        if ((x & 0x7FFFFFFFu) == 0) {    // Signed zero
            *hp++ = (uint16_t)(x >> 16); // Return the signed zero
        }
        else {                    // Not zero
            xs = x & 0x80000000u; // Pick off sign bit
            xe = x & 0x7FF00000u; // Pick off exponent bits
            xm = x & 0x000FFFFFu; // Pick off mantissa bits
            if (xe == 0) {        // Denormal will underflow, return a signed zero
                *hp++ = (uint16_t)(xs >> 16);
            }
            else if (xe == 0x7FF00000u) {                     // Inf or NaN (all the exponent bits are set)
                if (xm == 0) {                                // If mantissa is zero ...
                    *hp++ = (uint16_t)((xs >> 16) | 0x7C00u); // Signed Inf
                }
                else {
                    *hp++ = (uint16_t)0xFE00u; // NaN, only 1st mantissa bit set
                }
            }
            else {                                            // Normalized number
                hs = (uint16_t)(xs >> 16);                    // Sign bit
                hes = ((int)(xe >> 20)) - 1023 + 15;          // Exponent unbias the double, then bias the halfp
                if (hes >= 0x1F) {                            // Overflow
                    *hp++ = (uint16_t)((xs >> 16) | 0x7C00u); // Signed Inf
                }
                else if (hes <= 0) {       // Underflow
                    if ((10 - hes) > 21) { // Mantissa shifted all the way off & no rounding possibility
                        hm = (uint16_t)0u; // Set mantissa to zero
                    }
                    else {
                        xm |= 0x00100000u;                    // Add the hidden leading bit
                        hm = (uint16_t)(xm >> (11 - hes));    // Mantissa
                        if ((xm >> (10 - hes)) & 0x00000001u) // Check for rounding
                            hm += (uint16_t)1u;               // Round, might overflow into exp bit, but this is OK
                    }
                    *hp++ = (hs | hm); // Combine sign bit and mantissa bits, biased exponent is zero
                }
                else {
                    he = (uint16_t)(hes << 10);                // Exponent
                    hm = (uint16_t)(xm >> 10);                 // Mantissa
                    if (xm & 0x00000200u)                      // Check for rounding
                        *hp++ = (hs | he | hm) + (uint16_t)1u; // Round, might overflow to inf, this is OK
                    else
                        *hp++ = (hs | he | hm); // No rounding
                }
            }
        }
    }
}

static void
halfp2doubles(void* target,
    void* source,
    long numel)
{
    uint16_t* hp = (uint16_t*)source; // Type pun input as an unsigned 16-bit int
    uint32_t* xp = (uint32_t*)target; // Type pun output as an unsigned 32-bit int
    uint16_t h, hs, he, hm;
    uint32_t xs, xe, xm;
    int32_t xes;
    int e;

    if (source == NULL || target == NULL) // Nothing to convert (e.g., imag part of pure real)
        return;
    while (numel--) {
        uint32_t x;

        h = *hp++;
        if ((h & 0x7FFFu) == 0) {    // Signed zero
            x = ((uint32_t)h) << 16; // Return the signed zero
        }
        else {                // Not zero
            hs = h & 0x8000u; // Pick off sign bit
            he = h & 0x7C00u; // Pick off exponent bits
            hm = h & 0x03FFu; // Pick off mantissa bits
            if (he == 0) {    // Denormal will convert to normalized
                e = -1;       // The following loop figures out how much extra to adjust the exponent
                do {
                    e++;
                    hm <<= 1;
                } while ((hm & 0x0400u) == 0); // Shift until leading bit overflows into exponent bit
                xs = ((uint32_t)hs) << 16;                   // Sign bit
                xes = ((int32_t)(he >> 10)) - 15 + 1023 - e; // Exponent unbias the halfp, then bias the double
                xe = (uint32_t)(xes << 20);                  // Exponent
                xm = ((uint32_t)(hm & 0x03FFu)) << 10;       // Mantissa
                x = (xs | xe | xm);                          // Combine sign bit, exponent bits, and mantissa bits
            }
            else if (he == 0x7C00u) {                                     // Inf or NaN (all the exponent bits are set)
                if (hm == 0) {                                            // If mantissa is zero ...
                    x = (((uint32_t)hs) << 16) | ((uint32_t)0x7FF00000u); // Signed Inf
                }
                else {
                    x = (uint32_t)0xFFF80000u; // NaN, only the 1st mantissa bit set
                }
            }
            else {
                xs = ((uint32_t)hs) << 16;               // Sign bit
                xes = ((int32_t)(he >> 10)) - 15 + 1023; // Exponent unbias the halfp, then bias the double
                xe = (uint32_t)(xes << 20);              // Exponent
                xm = ((uint32_t)hm) << 10;               // Mantissa
                x = (xs | xe | xm);                      // Combine sign bit, exponent bits, and mantissa bits
            }
        }

        xp[1 - next] = 0;
        xp[next] = x;

        xp += 2;
    }
}

static void
convert_double_half(BablConversion* conversion,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        doubles2halfp(dst, src, 1);
        dst += dst_pitch;
        src += src_pitch;
    }
}

static void
convert_half_double(BablConversion* conversion,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        halfp2doubles(dst, src, 1);
        dst += dst_pitch;
        src += src_pitch;
    }
}

static void
convert_float_half(BablConversion* conversion,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        _babl_float_to_half((void*)dst, (float*)src, 1);
        dst += dst_pitch;
        src += src_pitch;
    }
}

static void
convert_half_float(BablConversion* conversion,
    char* src,
    char* dst,
    int src_pitch,
    int dst_pitch,
    long n)
{
    while (n--) {
        _babl_half_to_float((float*)dst, (void*)src, 1);
        dst += dst_pitch;
        src += src_pitch;
    }
}

void BABL_SIMD_SUFFIX(babl_base_type_half)(void)
{
    babl_type_new(
        "half",
        "id", BABL_HALF,
        "bits", 16,
        "doc", "IEEE 754 half precision.",
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_HALF),
        babl_type_from_id(BABL_DOUBLE),
        "plane", convert_half_double,
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_DOUBLE),
        babl_type_from_id(BABL_HALF),
        "plane", convert_double_half,
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_HALF),
        babl_type_from_id(BABL_FLOAT),
        "plane", convert_half_float,
        NULL);

    babl_conversion_new(
        babl_type_from_id(BABL_FLOAT),
        babl_type_from_id(BABL_HALF),
        "plane", convert_float_half,
        NULL);
}
