#ifndef _BABL_FISH_HPP
#define _BABL_FISH_HPP

#include "babl-class.hpp"
#include "babl-conversion.hpp"

BABL_CLASS_DECLARE(fish);

typedef struct
{
    BablInstance instance;
    const Babl* source;
    const Babl* destination;
    void (*dispatch)(const Babl* babl, const char* src, char* dst, long n, void* data);
    void** data;
    long pixels;
    double error;
} BablFish;

typedef struct
{
    BablFish fish;
    BablConversion* conversion;
    double cost;
    int32_t source_bpp;
    int32_t dest_bpp;
    void* foo;
} BablFishSimple;

typedef struct
{
    BablFish fish;
    double cost;
    int32_t source_bpp;
    int32_t dest_bpp;
    uint32_t is_u8_color_conv : 1;
    uint32_t* u8_lut;
    long last_lut_use;
    BablList* conversion_list;
} BablFishPath;

typedef struct
{
    BablFish fish;
} BablFishReference;

#endif
