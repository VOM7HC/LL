#ifndef _BABL_VERSION_HPP
#define _BABL_VERSION_HPP

#if !defined(BABL_INSIDE_BABL_H) && !defined(BABL_IS_BEING_COMPILED)
#error "babl-version.h must not be included directly, include babl.h instead."
#endif

#define BABL_MAJOR_VERSION @BABL_MAJOR_VERSION @
#define BABL_MINOR_VERSION @BABL_MINOR_VERSION @
#define BABL_MICRO_VERSION @BABL_MICRO_VERSION @

void babl_get_version(int* major, int* minor, int* micro);

#endif
