#ifndef _BABL_IMAGE_HPP
#define _BABL_IMAGE_HPP

#include "babl-class.hpp"

#include <cinttypes>

BABL_CLASS_DECLARE(image);

Babl* babl_image_new(const void* first_component, ...) BABL_ARG_NULL_TERMINATED;

typedef struct
{
    BablInstance instance;
    BablFormat* format;
    int32_t components;
    BablComponent** component;
    BablType** type;
    BablModel* model;
    BablSampling** sampling;
    char** data;
    int32_t* pitch;
    int32_t* stride;
} BablImage;

#endif
