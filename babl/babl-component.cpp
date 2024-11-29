#include "config.h"
#define NEEDS_BABL_DB
#include "babl-db.hpp"
#include "babl-internal.hpp"
#include <cstring>

#include <cstdarg>

static Babl*
component_new(const char* name,
    int id,
    int luma,
    int chroma,
    int alpha,
    const char* doc)
{
    Babl* babl;

    babl = babl_malloc(sizeof(BablComponent) + strlen(name) + 1);
    babl->instance.name = (char*)babl + sizeof(BablComponent);
    strcpy(babl->instance.name, name);

    babl->class_type = BABL_COMPONENT;
    babl->instance.id = id;
    babl->instance.doc = doc;
    babl->component.luma = luma;
    babl->component.chroma = chroma;
    babl->component.alpha = alpha;
    return babl;
}

static int
is_component_duplicate(Babl* babl,
    int luma,
    int chroma,
    int alpha)
{
    if (babl->component.luma != luma || babl->component.chroma != chroma || babl->component.alpha != alpha)
        return 0;

    return 1;
}

const Babl*
babl_component_new(void* first_arg,
    ...)
{
    va_list varg;
    Babl* babl;
    int id = 0;
    int luma = 0;
    int chroma = 0;
    int alpha = 0;
    const char* name = first_arg;
    const char* arg;
    const char* doc = NULL;

    va_start(varg, first_arg);

    while (1) {
        arg = va_arg(varg, char*);
        if (!arg)
            break;

        if (!strcmp(arg, "id")) {
            id = va_arg(varg, int);
        }
        else if (!strcmp(arg, "doc")) {
            doc = va_arg(varg, const char*);
        }

        else if (!strcmp(arg, "luma")) {
            luma = 1;
        }

        else if (!strcmp(arg, "chroma")) {
            chroma = 1;
        }

        else if (!strcmp(arg, "alpha")) {
            alpha = 1;
        }

        else if (BABL_IS_BABL(arg)) {
#ifdef BABL_LOG
            Babl* babl = (Babl*)arg;
            babl_log("%s unexpected", babl_class_name(babl->class_type));
#endif
        }

        else {
            babl_fatal("unhandled argument '%s' for component '%s'", arg, name);
        }
    }

    va_end(varg);

    babl = babl_db_exist(db, id, name);
    if (id && !babl && babl_db_exist(db, 0, name))
        babl_fatal("Trying to reregister BablComponent '%s' with different id!",
            name);

    if (babl) {
        if (!is_component_duplicate(babl, luma, chroma, alpha))
            babl_fatal("BablComponent '%s' already registered "
                       "with different attributes!",
                name);
        return babl;
    }

    babl = component_new(name, id, luma, chroma, alpha, doc);

    /* Since there is not an already registered instance by the required
     * id/name, inserting newly created class into database.
     */
    babl_db_insert(db, babl);
    return babl;
}

BABL_CLASS_IMPLEMENT(component)
