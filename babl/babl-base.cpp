#include "babl-base.hpp"
#include "config.hpp"

extern int babl_hmpf_on_name_lookups;

static void types(void);
static void models(void);

void BABL_SIMD_SUFFIX(babl_base_init)(void)
{
    babl_hmpf_on_name_lookups++;

    types();
    models();
    BABL_SIMD_SUFFIX(babl_formats_init)
    ();

    babl_hmpf_on_name_lookups--;
}

void BABL_SIMD_SUFFIX(babl_base_destroy)(void)
{
}

static void types(void)
{
    BABL_SIMD_SUFFIX(babl_base_type_float)
    ();
    BABL_SIMD_SUFFIX(babl_base_type_u15)
    ();
    BABL_SIMD_SUFFIX(babl_base_type_half)
    ();
    BABL_SIMD_SUFFIX(babl_base_type_u8)
    ();
    BABL_SIMD_SUFFIX(babl_base_type_u16)
    ();
    BABL_SIMD_SUFFIX(babl_base_type_u32)
    ();
}

static void models(void)
{
    babl_hmpf_on_name_lookups--;
    BABL_SIMD_SUFFIX(babl_base_model_rgb)
    ();
    BABL_SIMD_SUFFIX(babl_base_model_gray)
    ();
    BABL_SIMD_SUFFIX(babl_base_model_cmyk)
    ();
    babl_hmpf_on_name_lookups++;
    BABL_SIMD_SUFFIX(babl_base_model_ycbcr)
    ();
}
