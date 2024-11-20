#ifndef _BABL_CLASS_HPP
#define _BABL_CLASS_HPP

#include "babl.hpp"
#include <cinttypes>

typedef struct _BablList BablList;

typedef int BablClassType;

typedef int (*BablEachFunction)(Babl* entry, void* data);

#define BABL_CLASS_DECLARE(klass)                                 \
                                                                  \
    BablDb* babl_##klass##_db(void);                              \
    const Babl* babl_##klass##_from_id(int id);                   \
    void babl_##klass##_class_for_each(BablEachFunction each_fun, \
        void* user_data)

typedef struct
{
    BablClassType class_type;
    int32_t id;
    void* creator;
    char* name;
    const char* doc;
} BablInstance;

#endif
