// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the BABL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// BABL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef BABL_EXPORTS
#define BABL_API __declspec(dllexport)
#else
#define BABL_API __declspec(dllimport)
#endif

// This class is exported from the dll
class BABL_API Cbabl {
public:
	Cbabl(void);
	// TODO: add your methods here.
};

extern BABL_API int nbabl;

BABL_API int fnbabl(void);


///////////////////////////////////

#ifndef _BABL_HPP
#define _BABL_HPP

#ifdef __cplusplus
extern "C" {
#endif

#define BABL_INSIDE_BABL_H
#include "babl-macros.hpp"
#include "babl-types.hpp"
#include "babl-version.hpp"
#undef BABL_INSIDE_BABL_H

void babl_init(void);

void babl_exit(void);

const Babl* babl_type(const char* name);

const Babl* babl_sampling(int horizontal, int vertical);

const Babl* babl_component(const char* name);

const Babl* babl_model(const char* name);

const Babl* babl_model_with_space(const char* name, const Babl* space);

const Babl* babl_space(const char* name);

typedef enum {
    BABL_ICC_INTENT_PERCEPTUAL = 0,
    BABL_ICC_INTENT_RELATIVE_COLORIMETRIC = 1,
    BABL_ICC_INTENT_SATURATION = 2,
    BABL_ICC_INTENT_ABSOLUTE_COLORIMETRIC = 3,
    BABL_ICC_INTENT_PERFORMANCE = 32
} BablIccIntent;

const Babl* babl_space_from_icc(const char* icc_data, int icc_length, BablIccIntent intent, const char** error);

double babl_space_get_gamma(const Babl* space);

[[deprecated("deprecated API")]]
const Babl* babl_icc_make_space(const char* icc_data, int icc_length, BablIccIntent intent, const char** error);

char* babl_icc_get_key(const char* icc_data, int icc_length, const char* key, const char* language, const char* country);

const Babl* babl_format(const char* encoding);

const Babl* babl_format_with_space(const char* encoding, const Babl* space);

int babl_format_exists(const char* name);

const Babl* babl_format_get_space(const Babl* format);

const Babl* babl_fish(const void* source_format, const void* destination_format);

const Babl* babl_fast_fish(const void* source_format, const void* destination_format, const char* performance);

long babl_process(const Babl* babl_fish, const void* source, void* destination, long n);

long babl_process_rows(const Babl* babl_fish, const void* source, int source_stride, void* dest, int dest_stride, long n, int rows);

const char* babl_get_name(const Babl* babl);

int babl_format_has_alpha(const Babl* format);

int babl_format_get_bytes_per_pixel(const Babl* format);

const Babl* babl_format_get_model(const Babl* format);

typedef enum {
    BABL_MODEL_FLAG_ALPHA = 1 << 1,
    BABL_MODEL_FLAG_ASSOCIATED = 1 << 2,
    BABL_MODEL_FLAG_INVERTED = 1 << 3,

    BABL_MODEL_FLAG_LINEAR = 1 << 10,
    BABL_MODEL_FLAG_NONLINEAR = 1 << 11,
    BABL_MODEL_FLAG_PERCEPTUAL = 1 << 12,

    BABL_MODEL_FLAG_GRAY = 1 << 20,
    BABL_MODEL_FLAG_RGB = 1 << 21,

    BABL_MODEL_FLAG_CIE = 1 << 23,
    BABL_MODEL_FLAG_CMYK = 1 << 24,

} BablModelFlag;

#define BABL_MODEL_FLAG_PREMULTIPLIED BABL_MODEL_FLAG_ASSOCIATED

BablModelFlag babl_get_model_flags(const Babl* model);

int babl_format_get_n_components(const Babl* format);

const Babl* babl_format_get_type(const Babl* format, int component_index);

const Babl* babl_type_new(void* first_arg, ...) BABL_ARG_NULL_TERMINATED;

const Babl* babl_component_new(void* first_arg, ...) BABL_ARG_NULL_TERMINATED;

const Babl* babl_model_new(void* first_arg, ...) BABL_ARG_NULL_TERMINATED;

const Babl* babl_format_new(const void* first_arg, ...) BABL_ARG_NULL_TERMINATED;

const Babl* babl_format_n(const Babl* type, int components);

int babl_format_is_format_n(const Babl* format);

const Babl* babl_conversion_new(const void* first_arg, ...) BABL_ARG_NULL_TERMINATED;

const Babl* babl_conversion_get_source_space(const Babl* conversion);

const Babl* babl_conversion_get_destination_space(const Babl* conversion);

const Babl* babl_new_palette(const char* name, const Babl** format_u8, const Babl** format_u8_with_alpha);

const Babl* babl_new_palette_with_space(const char* name, const Babl* space, const Babl** format_u8, const Babl** format_u8_with_alpha);

int babl_format_is_palette(const Babl* format);

void babl_palette_set_palette(const Babl* babl,
    const Babl* format,
    void* data,
    int count);

void babl_palette_reset(const Babl* babl);

void babl_set_user_data(const Babl* babl, void* data);

void* babl_get_user_data(const Babl* babl);

typedef enum {
    BABL_SPACE_FLAG_NONE = 0,
    BABL_SPACE_FLAG_EQUALIZE = 1
} BablSpaceFlags;

const Babl* babl_space_from_chromaticities(const char* name,
    double wx, double wy,
    double rx, double ry,
    double gx, double gy,
    double bx, double by,
    const Babl* trc_red,
    const Babl* trc_green,
    const Babl* trc_blue,
    BablSpaceFlags flags);

const Babl* babl_trc_gamma(double gamma);

const Babl* babl_trc(const char* name);

const Babl* babl_space_with_trc(const Babl* space, const Babl* trc);

void babl_space_get(const Babl* space, double* xw, double* yw, double* xr, double* yr, double* xg, double* yg, double* xb, double* yb,
    const Babl** red_trc,
    const Babl** green_trc,
    const Babl** blue_trc);

void babl_space_get_rgb_luminance(const Babl* space, double* red_luminance, double* green_luminance, double* blue_luminance);

int babl_model_is(const Babl* babl, const char* model_name);

#define babl_model_is(babl, model) (babl && (babl) == babl_model_with_space(model, babl))

const char* babl_space_get_icc(const Babl* babl, int* length);

const Babl* babl_space_from_rgbxyz_matrix(const char* name,
    double wx, double wy, double wz,
    double rx, double gx, double bx,
    double ry, double gy, double by,
    double rz, double gz, double bz,
    const Babl* trc_red,
    const Babl* trc_green,
    const Babl* trc_blue);

const char* babl_format_get_encoding(const Babl* babl);

int babl_space_is_rgb(const Babl* space);

int babl_space_is_cmyk(const Babl* space);

int babl_space_is_gray(const Babl* space);

typedef void (*BablFishProcess)(const Babl* babl, const char* src, char* dst, long n, void* data);

BablFishProcess babl_fish_get_process(const Babl* babl);

void babl_gc(void);

#define BABL_ALPHA_FLOOR (1 / 65536.0)
#define BABL_ALPHA_FLOOR_F (1 / 65536.0f)

#ifdef __cplusplus
}
#endif

#endif
