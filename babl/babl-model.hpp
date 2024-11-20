#ifndef _BABL_MODEL_HPP
#define _BABL_MODEL_HPP

#include "babl-class.hpp"

BABL_CLASS_DECLARE(model);

typedef struct
{
    BablInstance instance;
    BablList* from_list;
    int components;
    BablComponent** component;
    BablType** type;

    void* data;
    const Babl* space;
    void* model;
    BablModelFlag flags;
} BablModel;

#endif
