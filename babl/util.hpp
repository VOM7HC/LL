#ifndef _BASE_UTIL_HPP
#define _BASE_UTIL_HPP

#include "pow-24.hpp"
#include "babl.hpp"
#include <cassert>
#include <cinttypes>
#include <cmath>

#define BABL_PLANAR_SANITY     \
    {                          \
        assert(src_bands > 0); \
        assert(dst_bands > 0); \
        assert(src);           \
        assert(*src);          \
        assert(dst);           \
        assert(*dst);          \
        assert(n > 0);         \
        assert(*src_pitch);    \
    }

#define BABL_PLANAR_STEP                \
    {                                   \
        int32_t i;                      \
        for (i = 0; i < src_bands; i++) \
            src[i] += src_pitch[i];     \
        for (i = 0; i < dst_bands; i++) \
            dst[i] += dst_pitch[i];     \
    }

static inline double babl_epsilon_for_zero(double value)
{
    return value * (value > BABL_ALPHA_FLOOR || value < -BABL_ALPHA_FLOOR) + BABL_ALPHA_FLOOR * (value <= BABL_ALPHA_FLOOR && value >= -BABL_ALPHA_FLOOR);
}

static inline float babl_epsilon_for_zero_float(float value)
{
    return value * (value > BABL_ALPHA_FLOOR_F || value < -BABL_ALPHA_FLOOR_F) + BABL_ALPHA_FLOOR_F * (value <= BABL_ALPHA_FLOOR_F && value >= -BABL_ALPHA_FLOOR_F);
}

#define BABL_USE_SRGB_GAMMA

#ifdef BABL_USE_SRGB_GAMMA
static inline double linear_to_gamma_2_2(double value)
{
    if (value > 0.003130804954)
        return 1.055 * pow(value, (1.0 / 2.4)) - 0.055;
    return 12.92 * value;
}

static inline double gamma_2_2_to_linear(double value)
{
    if (value > 0.04045)
        return pow((value + 0.055) / 1.055, 2.4);
    return value / 12.92;
}
static inline double babl_linear_to_gamma_2_2(double value)
{
    if (value > 0.003130804954)
        return 1.055 * babl_pow_1_24(value) - 0.055;
    return 12.92 * value;
}
static inline float babl_linear_to_gamma_2_2f(float value)
{
    if (value > 0.003130804954f) {
        return 1.055f * babl_pow_1_24f(value) - (0.055f - 3.0f / (float)(1 << 24));
    }
    return 12.92f * value;
}

static inline double babl_gamma_2_2_to_linear(double value)
{
    if (value > 0.04045)
        return babl_pow_24((value + 0.055) / 1.055);
    return value / 12.92;
}
static inline float babl_gamma_2_2_to_linearf(float value)
{
    if (value > 0.04045f)
        return babl_pow_24f((value + 0.055f) / 1.055f);
    return value / 12.92f;
}

#else
#define linear_to_gamma_2_2(value) (pow((value), (1.0F / 2.2F)))
#define gamma_2_2_to_linear(value) (pow((value), 2.2F))

#define babl_linear_to_gamma_2_2f(value) (powf((value), (1.0f / 2.2f)))
#define babl_gamma_2_2_to_linearf(value) (powf((value), 2.2f))
#endif

#endif
