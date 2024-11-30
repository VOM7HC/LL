#include "babl-internal.hpp"
#include "config.h"

static char*
create_name(BablConversion* conversion)
{
    return conversion->instance.name;
}

Babl* babl_fish_simple(BablConversion* conversion)
{
    Babl* babl = NULL;
    char* name;

    babl_assert(BABL_IS_BABL(conversion));
    name = create_name(conversion);
    babl = babl_db_exist_by_name(babl_fish_db(), name);
    if (babl) {
        /* There is an instance already registered by the required name,
         * returning the preexistent one instead.
         */
        return babl;
    }

    babl = babl_calloc(1, sizeof(BablFishSimple) + strlen(name) + 1);
    babl->class_type = BABL_FISH_SIMPLE;
    babl->instance.id = babl_fish_get_id(conversion->source, conversion->destination);
    babl->instance.name = ((char*)babl) + sizeof(BablFishSimple);
    strcpy(babl->instance.name, name);
    babl->fish.source = conversion->source;
    babl->fish.destination = conversion->destination;

    babl->fish.pixels = 0;
    babl->fish_simple.conversion = conversion;
    babl->fish.error = 0.0; /* babl fish simple should only be used by bablfish
                          reference, and babl fish reference only requests clean
                          conversions */

    _babl_fish_rig_dispatch(babl);

    babl_db_insert(babl_fish_db(), babl);
    return babl;
}
