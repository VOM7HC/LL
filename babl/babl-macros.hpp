#ifndef _BABL_MACROS_HPP
#define _BABL_MACROS_HPP

#if !defined(BABL_INSIDE_BABL_H) && !defined(BABL_IS_BEING_COMPILED)
#error "babl-macros.h must not be included directly, include babl.h instead."
#endif

#if __GNUC__ >= 4
#define BABL_ARG_NULL_TERMINATED __attribute__((__sentinel__))
#else
#define BABL_ARG_NULL_TERMINATED
#endif

#endif
