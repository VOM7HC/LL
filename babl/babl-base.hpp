#ifndef _BABL_BASE_HPP
#define _BABL_BASE_HPP

#ifdef _MSC_VER
#ifdef _M_ARM64
#define BABL_SIMD_SUFFIX(symbol) symbol##_arm64
#elif defined(_M_X64)
#define BABL_SIMD_SUFFIX(symbol) symbol##_x64
#else
#define BABL_SIMD_SUFFIX(symbol) symbol##_generic
#endif
#endif

extern void (*babl_base_init)(void);

void BABL_SIMD_SUFFIX(babl_base_init)(void);
void BABL_SIMD_SUFFIX(babl_base_destroy)(void);
void BABL_SIMD_SUFFIX(babl_formats_init)(void);

void BABL_SIMD_SUFFIX(babl_base_type_half)(void);
void BABL_SIMD_SUFFIX(babl_base_type_float)(void);
void BABL_SIMD_SUFFIX(babl_base_type_u8)(void);
void BABL_SIMD_SUFFIX(babl_base_type_u16)(void);
void BABL_SIMD_SUFFIX(babl_base_type_u15)(void);
void BABL_SIMD_SUFFIX(babl_base_type_u32)(void);

void BABL_SIMD_SUFFIX(babl_base_model_rgb)(void);
void BABL_SIMD_SUFFIX(babl_base_model_cmyk)(void);
void BABL_SIMD_SUFFIX(babl_base_model_gray)(void);
void BABL_SIMD_SUFFIX(babl_base_model_ycbcr)(void);

#endif
