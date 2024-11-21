#ifndef _BABL_SPACE_HPP
#define _BABL_SPACE_HPP

#include "babl-matrix.hpp"
#include "babl-class.hpp"
#include "util.hpp"
#include "config.h"
#include <cmath>
#include <cstring>

#ifdef HAVE_LCMS
#include <lcms2.h>
#endif

BABL_CLASS_DECLARE(space);

typedef struct
{
#ifdef HAVE_LCMS
    cmsHPROFILE lcms_profile;
    cmsHTRANSFORM lcms_to_rgba;
    cmsHTRANSFORM lcms_from_rgba;
#endif
    int filler;
} BablCMYK;

#if 0 // draft datastructures for spectral spaces
typedef struct _BablSpectrumType BablSpectrumType;

struct _BablSpectrumType {
  double        nm_start;
  double        nm_gap;
  double        nm_end;
  int           bands;
};

typedef struct
{
  BablSpectrumType spectrum_type;
  int              is_spectral;
  float           *observer_x;
  float           *observer_y;
  float           *observer_z;
  float           *illuminant;
  float            rev_y_scale;
} BablSpectralSpace;

typedef struct
{
  BablSpectralSpace *spectral_space;
  int                inks;
  float             *on_white;
  float             *on_black;
  float             *opaqueness;
  float              scale;
  float              trc_gamma;
  float             *illuminant;
} BablCoat;

#define BABL_MAX_COATS 16

typedef struct
{
  BablSpectralSpace *spectral_space;
  BablCoat           coat_def[BABL_MAX_COATS];
  int                coats;
  float             *substrate;

  int    stochastic_iterations;
  float  stochastic_diffusion0;
  float  stochastic_diffusion1;
} BablProcessSpace;
#endif

typedef enum {
    BablICCTypeRGB = 0,
    BablICCTypeGray = 2,
    BablICCTypeCMYK = 3,
} BablICCType;

typedef struct
{
    BablInstance instance;
    double xw;
    double yw;

    double xr;
    double yr;

    double xg;
    double yg;

    double xb;
    double yb;

    BablICCType icc_type;
    double whitepoint[3];
    const Babl* trc[3];

    char name[512];

    double RGBtoXYZ[9];
    double XYZtoRGB[9];
    float RGBtoXYZf[9];
    float XYZtoRGBf[9];

    char* icc_profile;
    int icc_length;
    BablCMYK cmyk;
} BablSpace;

static inline void babl_space_to_xyzf(const Babl* space, const float* rgb, float* xyz)
{
    BablSpace* space_ = (void*)space;
    babl_matrix_mul_vectorff(space_->RGBtoXYZf, rgb, xyz);
}

static inline void babl_space_from_xyzf(const Babl* space, const float* xyz, float* rgb)
{
    BablSpace* space_ = (void*)space;
    babl_matrix_mul_vectorff(space_->XYZtoRGBf, xyz, rgb);
}

static inline void _babl_space_to_xyz(const Babl* space, const double* rgb, double* xyz)
{
    BablSpace* space_ = (void*)space;
    babl_matrix_mul_vector(space_->RGBtoXYZ, rgb, xyz);
}

static inline void _babl_space_from_xyz(const Babl* space, const double* xyz, double* rgb)
{
    BablSpace* space_ = (void*)space;
    babl_matrix_mul_vector(space_->XYZtoRGB, xyz, rgb);
}

void babl_space_class_init(void);

const Babl* babl_space_from_gray_trc(const char* name, const Babl* trc_gray, BablSpaceFlags flags);

void babl_chromatic_adaptation_matrix(const double* whitepoint, const double* target_whitepoint, double* chad_matrix);

#endif
