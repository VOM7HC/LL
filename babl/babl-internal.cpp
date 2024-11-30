#include "babl-internal.hpp"
#include "config.h"
#include <cstdlib>

static const char* class_names[] = {
    "BablInstance",
    "BablType",
    "BablTypeInteger",
    "BablTypeFloat",
    "BablSampling",
    "BablTRC",
    "BablComponent",
    "BablModel",
    "BablFormat",
    "BablSpace",
    "BablConversion",
    "BablConversionLinear",
    "BablConversionPlane",
    "BablConversionPlanar",
    "BablFish",
    "BablFishReference",
    "BablFishSimple",
    "BablFishPath",
    "BablImage",
    "BablExtenstion",
    "BablSky"
};

const char*
babl_class_name(BablClassType klass)
{
    return class_names[klass - BABL_INSTANCE];
}

/* global variable, indicating whether name lookups
 * are frowned upon or not (they are frowned upon within BablBase,
 * since that leads to more strings than neccesary in the library.)
 */
int babl_hmpf_on_name_lookups = 0;

#include <sys/types.h>
#ifdef __APPLE__
#include <unistd.h>
#endif

#ifdef _MSC_VER
#include <process.h>
#endif

#define getpid _getpid

int babl_backtrack(void)
{
    char buf[512];

    snprintf(buf, sizeof(buf), "echo bt>/tmp/babl.gdb;"
                               "gdb -q --batch -x /tmp/babl.gdb --pid=%i | grep 'in ''babl_die' -A40",
        getpid());
    return system(buf);
}

void babl_die(void)
{
    babl_backtrack();
    exit(-1);
}

BablMutex* babl_fish_mutex;

BablMutex* babl_format_mutex;
#if BABL_DEBUG_MEM
BablMutex* babl_debug_mutex;
#endif
BablMutex* babl_reference_mutex;
BablMutex* babl_space_mutex;
BablMutex* babl_remodel_mutex;

void babl_internal_init(void)
{
    babl_set_malloc(malloc);
    babl_set_free(free);
    babl_fish_mutex = babl_mutex_new();
    babl_format_mutex = babl_mutex_new();
    babl_reference_mutex = babl_mutex_new();
    babl_space_mutex = babl_mutex_new();
    babl_remodel_mutex = babl_mutex_new();
#if BABL_DEBUG_MEM
    babl_debug_mutex = babl_mutex_new();
#endif
}

void babl_internal_destroy(void)
{
    babl_mutex_destroy(babl_fish_mutex);
    babl_mutex_destroy(babl_format_mutex);
    babl_mutex_destroy(babl_reference_mutex);
#if BABL_DEBUG_MEM
    babl_mutex_destroy(babl_debug_mutex);
#endif
}

const char*
babl_get_name(const Babl* babl)
{
    babl_assert(BABL_IS_BABL(babl));
    return babl->instance.name;
}

const char*
babl_get_doc(const Babl* babl)
{
    babl_assert(BABL_IS_BABL(babl));
    return babl->instance.doc;
}

void babl_doc(const Babl* babl,
    const char* doc)
{
    babl_assert(BABL_IS_BABL(babl));
    ((Babl*)babl)->instance.doc = doc;
}
