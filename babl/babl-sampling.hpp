#ifndef _BABL_SAMPLING_HPP
#define _BABL_SAMPLING_HPP

#include "babl-class.hpp"

BABL_CLASS_DECLARE(sampling);

typedef struct
{
    BablInstance instance;
    int32_t horizontal;
    int32_t vertical;
    char name[4];
} BablSampling;

void babl_sampling_class_init(void);

#endif
