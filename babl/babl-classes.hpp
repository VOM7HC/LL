#ifndef _BABL_CLASSES_HPP
#define _BABL_CLASSES_HPP

#include "babl-class.hpp"
#include "babl-db.hpp"

#define BABL_MAGIC 0xbab100

enum {
    BABL_INSTANCE = BABL_MAGIC,
    BABL_TYPE,
    BABL_TYPE_INTEGER,
    BABL_TYPE_FLOAT,
    BABL_SAMPLING,
    BABL_TRC,
    BABL_COMPONENT,
    BABL_MODEL,
    BABL_FORMAT,
    BABL_SPACE,

    BABL_CONVERSION,
    BABL_CONVERSION_LINEAR,
    BABL_CONVERSION_PLANE,
    BABL_CONVERSION_PLANAR,

    BABL_FISH,
    BABL_FISH_REFERENCE,
    BABL_FISH_SIMPLE,
    BABL_FISH_PATH,
    BABL_IMAGE,

    BABL_EXTENSION,

    BABL_SKY
};

#include "babl-component.hpp"
#include "babl-conversion.hpp"
#include "babl-extension.hpp"
#include "babl-fish.hpp"
#include "babl-format.hpp"
#include "babl-image.hpp"
#include "babl-model.hpp"
#include "babl-sampling.hpp"
#include "babl-space.hpp"
#include "babl-type.hpp"
#include "babl-trc.hpp" // base/babl-trc.h

typedef union _Babl {
    BablClassType class_type;
    BablInstance instance;
    BablType type;
    BablSampling sampling;
    BablSpace space;
    BablTRC trc;
    BablComponent component;
    BablModel model;
    BablFormat format;
    BablConversion conversion;
    BablImage image;
    BablFish fish;
    BablFishReference fish_reference;
    BablFishSimple fish_simple;
    BablFishPath fish_path;
    BablExtension extension;
} _Babl;

#endif
