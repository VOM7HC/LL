#ifndef _BABL_FORMAT_HPP
#define _BABL_FORMAT_HPP

#include "babl-class.hpp"
#include "babl-component.hpp"

BABL_CLASS_DECLARE(format);

typedef struct
{
    BablInstance instance;
    BablList* from_list;
    int components;
    BablComponent** component;
    BablType** type;
    BablModel* model;
    const Babl* space;
    void* model_data;
    void* image_template; /* image template for use with
                             linear (non-planer) images */

    BablSampling** sampling;
    int bytes_per_pixel;
    int planar;
    double loss;  /*< average relative error when converting
                      from and to RGBA double */
    int visited;  /* for convenience in code while searching
                     for conversion paths */
    int format_n; /* whether the format is a format_n type or not */
    int palette;
    const char* encoding;
} BablFormat;

#endif
