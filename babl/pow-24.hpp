#ifndef _BASE_POW_24_HPP
#define _BASE_POW_24_HPP

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include <cinttypes>
#include <cmath>
#include <cstdint>

static double babl_pow_1_24(double x);
static double babl_pow_24(double x);
static float babl_pow_1_24f(float x);
static float babl_pow_24f(float x);

static inline double init_newton(double x, double exponent, double c0, double c1, double c2)
{
    int32_t iexp;
    double y = frexp(x, &iexp);
    y = 2 * y + (iexp - 2);
    c1 *= M_LN2 * exponent;
    c2 *= M_LN2 * M_LN2 * exponent * exponent;
    return y = c0 + c1 * y + c2 * y * y;
}

static inline double babl_pow_24(double x)
{
    double y;
    int32_t i;
    if (x > 16.0) {
        return exp(log(x) * 2.4);
    }
    y = init_newton(x, -1. / 5, 0.9953189663, 0.9594345146, 0.6742970332);
    for (i = 0; i < 3; i++)
        y = (1. + 1. / 5) * y - ((1. / 5) * x * (y * y)) * ((y * y) * (y * y));
    x *= y;
    return x * x * x;
}

static inline double babl_pow_1_24(double x)
{
    double y;
    int32_t i;
    double z;
    if (x > 1024.0) {
        return exp(log(x) * (1.0 / 2.4));
    }
    y = init_newton(x, -1. / 12, 0.9976800269, 0.9885126933, 0.5908575383);
    x = sqrt(x);
    z = (1. / 6.) * x;
    for (i = 0; i < 3; i++)
        y = (7. / 6.) * y - z * ((y * y) * (y * y) * (y * y * y));
    return x * y;
}

static inline float babl_frexpf(float x, int32_t* e)
{
    union {
        float f;
        uint32_t i;
    } y = { x };
    int32_t ee = y.i >> 23 & 0xff;

    if (!ee) {
        if (x) {
            x = babl_frexpf(x * 18446744073709551616.0f, e);
            *e -= 64;
        }
        else
            *e = 0;
        return x;
    }
    else if (ee == 0xff) {
        return x;
    }

    *e = ee - 0x7e;
    y.i &= 0x807ffffful;
    y.i |= 0x3f000000ul;
    return y.f;
}

static inline float init_newtonf(float x, float exponent, float c0, float c1, float c2)
{
#define fM_LN2 0.69314718055994530942f
    int32_t iexp = 0;
    float y = babl_frexpf(x, &iexp);
    y = 2 * y + (iexp - 2);
    c1 *= fM_LN2 * exponent;
    c2 *= fM_LN2 * fM_LN2 * exponent * exponent;
    return y = c0 + c1 * y + c2 * y * y;
}

static inline float babl_pow_24f(float x)
{
    float y;
    int32_t i;
    if (x > 16.0f) {
        return expf(logf(x) * 2.4f);
    }
    y = init_newtonf(x, -1.f / 5, 0.9953189663f, 0.9594345146f, 0.6742970332f);
    for (i = 0; i < 3; i++)
        y = (1.f + 1.f / 5) * y - ((1.f / 5) * x * (y * y)) * ((y * y) * (y * y));
    x *= y;
    return x * x * x;
}

static inline float babl_pow_1_24f(float x)
{
    float y;
    int32_t i;
    float z;
    if (x > 1024.0f) {
        return expf(logf(x) * (1.0f / 2.4f));
    }
    y = init_newtonf(x, -1.f / 12, 0.9976800269f, 0.9885126933f, 0.5908575383f);
    x = sqrtf(x);
    z = (1.f / 6.f) * x;
    for (i = 0; i < 3; i++)
        y = (7.f / 6.f) * y - z * ((y * y) * (y * y) * (y * y * y));
    return x * y;
}

#endif
