#ifndef _BABL_TRC_HPP
#define _BABL_TRC_HPP

#include "babl-polynomial.hpp"
#include "util.hpp" // base/util.hpp
#include <cinttypes>
#include <cmath>
#include <cstring>

BABL_CLASS_DECLARE(trc);

typedef enum {
    BABL_TRC_LINEAR,
    BABL_TRC_FORMULA_GAMMA,
    BABL_TRC_SRGB,
    BABL_TRC_FORMULA_SRGB,
    BABL_TRC_LUT,
    BABL_TRC_FORMULA_CIE
} BablTRCType;

struct BablTRC {
    BablInstance instance;
    BablTRCType type;
    int32_t lut_size;
    double gamma;
    float rgamma;

    float (*fun_to_linear)(const Babl* trc, float val);
    float (*fun_from_linear)(const Babl* trc, float val);

    void (*fun_to_linear_buf)(const Babl* trc, const float* in, float* out, int32_t in_gap, int32_t out_gap, int32_t components, int32_t count);
    void (*fun_from_linear_buf)(const Babl* trc, const float* in, float* out, int32_t in_gap, int32_t out_gap, int32_t components, int32_t count);

    BablPolynomial poly_gamma_to_linear;
    float poly_gamma_to_linear_x0;
    float poly_gamma_to_linear_x1;
    BablPolynomial poly_gamma_from_linear;
    float poly_gamma_from_linear_x0;
    float poly_gamma_from_linear_x1;
    float* lut;
    float* inv_lut;
    char name[128];
    int valid_u8_lut;
    float u8_lut[256];
};

static inline void babl_trc_from_linear_buf(const Babl* trc_, const float* in, float* out, int32_t in_gap, int32_t out_gap, int32_t components, int32_t count)
{
    BablTRC* trc = (void*)trc_;
    trc->fun_from_linear_buf(trc_, in, out, in_gap, out_gap, components, count);
}

static inline void babl_trc_to_linear_buf(const Babl* trc_, const float* in, float* out, int32_t in_gap, int32_t out_gap, int32_t components, int32_t count)
{
    BablTRC* trc = (void*)trc_;
    trc->fun_to_linear_buf(trc_, in, out, in_gap, out_gap, components, count);
}

static inline float babl_trc_from_linear(const Babl* trc_, float value)
{
    BablTRC* trc = (void*)trc_;
    return trc->fun_from_linear(trc_, value);
}

static inline float babl_trc_to_linear(const Babl* trc_, float value)
{
    BablTRC* trc = (void*)trc_;
    return trc->fun_to_linear(trc_, value);
}

void babl_trc_class_init_generic(void);

#endif
