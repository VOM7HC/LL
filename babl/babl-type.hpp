#ifndef _BABL_TYPE_HPP
#define _BABL_TYPE_HPP

#include "babl-class.hpp"

BABL_CLASS_DECLARE(type);

typedef struct
{
    BablInstance instance;
    BablList* from_list;
    int bits;
    double min_val;
    double max_val;
} BablType;

typedef struct
{
    BablType type;
    int is_signed;
    long max;
    long min;
} BablTypeInteger;

typedef struct
{
    BablType type;
} BablTypeFloat;

#endif
