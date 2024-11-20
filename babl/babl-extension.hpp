#ifndef _BABL_EXTENSION_HPP
#define _BABL_EXTENSION_HPP

#include "babl-class.hpp"

BABL_CLASS_DECLARE(extension);

const Babl* babl_extension(const char* name);
void babl_extension_load_dir_list(const char* dir_list, const char** exclusion_patterns);

typedef struct
{
    BablInstance instance;
    void* dl_handle;
    void (*destroy)(void);
} BablExtension;

#endif
