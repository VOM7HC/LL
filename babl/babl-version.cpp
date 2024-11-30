#include "babl-internal.hpp"
#include "config.h"

void babl_get_version(int* major,
    int* minor,
    int* micro)
{
    if (major != NULL)
        *major = BABL_MAJOR_VERSION;

    if (minor != NULL)
        *minor = BABL_MINOR_VERSION;

    if (micro != NULL)
        *micro = BABL_MICRO_VERSION;
}
