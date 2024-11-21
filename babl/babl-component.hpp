#ifndef _BABL_COMPONENT_HPP
#define _BABL_COMPONENT_HPP

BABL_CLASS_DECLARE(component);

typedef struct
{
    BablInstance instance;
    int32_t luma;
    int32_t chroma;
    int32_t alpha;
} BablComponent;

#endif
