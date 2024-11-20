#ifndef _BABL_INTERNAL_HPP
#define _BABL_INTERNAL_HPP

#ifndef BABL_LIBRARY
#error "config.h must be included prior to babl-internal.h"
#endif

#ifdef _BABL_H
#error babl-internal.h included after babl.h
#endif

#define BABL_MAX_COMPONENTS 32
#define BABL_CONVERSIONS 5

#include "assert.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#undef _BABL_INTERNAL_HPP
#include "babl.h"
#define _BABL_INTERNAL_HPP

#include "babl-class.hpp"
#include "babl-classes.hpp"
#include "babl-cpuaccel.hpp"
#include "babl-db.hpp"
#include "babl-hash-table.hpp"
#include "babl-ids.hpp"
#include "babl-introspect.hpp"
#include "babl-list.hpp"
#include "babl-memory.hpp"
#include "babl-mutex.hpp"
#include "babl-polynomial.hpp"
#include "babl-shared-util.hpp"
#include "babl-util.hpp"

Babl* babl_conversion_find(const void* source, const void* destination);
double babl_conversion_error(BablConversion* conversion);
long babl_conversion_cost(BablConversion* conversion);

Babl* babl_extension_base(void);

Babl* babl_extender(void);
void babl_set_extender(Babl* new_extender);

Babl* babl_extension_quiet_log(void);
void babl_extension_deinit(void);

void babl_fish_reference_process(const Babl* babl, const char* source, char* destination, long n, void* data);

Babl* babl_fish_reference(const Babl* source, const Babl* destination);
Babl* babl_fish_simple(BablConversion* conversion);
Babl* babl_fish_path(const Babl* source, const Babl* destination);

int32_t babl_fish_get_id(const Babl* source, const Babl* destination);

double babl_format_loss(const Babl* babl);
Babl* babl_image_from_linear(char* buffer, const Babl* format);
Babl* babl_image_double_from_image(const Babl* source);

double babl_model_is_symmetric(const Babl* babl);
void babl_die(void);
int32_t babl_sanity(void);

void babl_doc(const Babl* babl, const char* doc);

const char* babl_get_doc(const Babl* babl);

void babl_core_init(void);
const Babl* babl_format_with_model_as_type(const Babl* model, const Babl* type);
int babl_formats_count(void);
int babl_type_is_symmetric(const Babl* babl);

#include <cstdarg>

int babl_backtrack(void);

static inline void real_babl_log_va(const char* file, int32_t line, const char* function, const char* fmt, va_list varg)
{
    Babl* extender = babl_extender();

    if (extender != babl_extension_quiet_log()) {
        if (babl_extender()) {
            fprintf(stderr, "When loading %s:\n\t", babl_extender()->instance.name);
        }

        fprintf(stderr, "%s:%i %s()\n\t", file, line, function);
    }

    vfprintf(stderr, fmt, varg);
    fprintf(stderr, "\n");
    fflush(NULL);

    return;
}

static inline void real_babl_log(const char* file, int32_t line, const char* function, const char* fmt, ...)
{
    va_list varg;

    va_start(varg, fmt);
    real_babl_log_va(file, line, function, fmt, varg);
    va_end(varg);
}

#ifndef G_STRFUNC
#if defined(__GNUC__)
#define G_STRFUNC ((const char*)(__PRETTY_FUNCTION__))
#elif defined(G_HAVE_ISO_VARARGS)
#define G_STRFUNC ((const char*)(__func__))
#else
#define G_STRFUNC ((const char*)("???"))
#endif
#endif

#if defined(__cplusplus) && defined(BABL_ISO_CXX_VARIADIC_MACROS)
#define BABL_ISO_VARIADIC_MACROS 1
#endif

#if defined(BABL_ISO_VARIADIC_MACROS)

#define babl_log(...) \
    real_babl_log(__FILE__, __LINE__, G_STRFUNC, __VA_ARGS__)

#define babl_fatal(...)                                            \
    do {                                                           \
        real_babl_log(__FILE__, __LINE__, G_STRFUNC, __VA_ARGS__); \
        babl_die();                                                \
    } while (0)

#elif defined(BABL_GNUC_VARIADIC_MACROS)

#define babl_log(args...) \
    real_babl_log(__FILE__, __LINE__, G_STRFUNC, args)

#define babl_fatal(args...)                                 \
    do {                                                    \
        real_babl_log(__FILE__, __LINE__, G_STRFUNC, args); \
        babl_die();                                         \
    } while (0)

#else

static inline void babl_log(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    real_babl_log_va(__FILE__, __LINE__, G_STRFUNC, format, args);
    va_end(args);
}

static inline void babl_fatal(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    real_babl_log_va(__FILE__, __LINE__, G_STRFUNC, format, args);
    va_end(args);
    babl_die();
}

#endif

#define babl_assert(expr)                                                                          \
    do {                                                                                           \
        if (!(expr)) {                                                                             \
            real_babl_log(__FILE__, __LINE__, G_STRFUNC, "Eeeeek! Assertion failed: `" #expr "`"); \
            assert(expr);                                                                          \
        }                                                                                          \
    } while (0)

#define BABL_CLASS_TYPE_IS_VALID(klass_type) \
    (((klass_type) >= BABL_INSTANCE) && ((klass_type) <= BABL_SKY) ? 1 : 0)

#define BABL_IS_BABL(obj) \
    (NULL == (obj) ? 0    \
                   : BABL_CLASS_TYPE_IS_VALID(((Babl*)(obj))->class_type))

extern int babl_hmpf_on_name_lookups;
extern int babl_in_fish_path;
extern BablMutex* babl_format_mutex;
extern BablMutex* babl_fish_mutex;
extern BablMutex* babl_reference_mutex;
extern BablMutex* babl_space_mutex;
extern BablMutex* babl_remodel_mutex;

#define BABL_DEBUG_MEM 0
#if BABL_DEBUG_MEM
extern BablMutex* babl_debug_mutex;
#endif

const char* babl_class_name(BablClassType klass);
void babl_internal_init(void);
void babl_internal_destroy(void);

#define BABL_CLASS_MINIMAL_IMPLEMENT(klass)                                        \
                                                                                   \
    BablDb* babl_##klass##_db(void)                                                \
    {                                                                              \
        if (!db)                                                                   \
            db = babl_db_init();                                                   \
        return db;                                                                 \
    }                                                                              \
                                                                                   \
    void babl_##klass##_class_for_each(BablEachFunction each_fun, void* user_data) \
    {                                                                              \
        babl_db_each(db, each_fun, user_data);                                     \
    }

#define BABL_CLASS_IMPLEMENT(klass)                                                   \
    BABL_CLASS_MINIMAL_IMPLEMENT(klass)                                               \
                                                                                      \
    const Babl* babl_##klass(const char* name)                                        \
    {                                                                                 \
        Babl* babl;                                                                   \
                                                                                      \
        if (babl_hmpf_on_name_lookups) {                                              \
            babl_log("%s(\"%s\"): looking up", G_STRFUNC, name);                      \
        }                                                                             \
        if (!db) {                                                                    \
            babl_fatal("%s(\"%s\"): you must call babl_init first", G_STRFUNC, name); \
        }                                                                             \
        babl = babl_db_exist_by_name(db, name);                                       \
                                                                                      \
        if (!babl) {                                                                  \
            babl_fatal("%s(\"%s\"): not found", G_STRFUNC, name);                     \
        }                                                                             \
        return babl;                                                                  \
    }                                                                                 \
                                                                                      \
    const Babl* babl_##klass##_from_id(int id)                                        \
    {                                                                                 \
        Babl* babl;                                                                   \
        babl = babl_db_exist_by_id(db, id);                                           \
        if (!babl) {                                                                  \
            babl_fatal("%s(%i): not found", G_STRFUNC, id);                           \
        }                                                                             \
        return babl;                                                                  \
    }

#define BABL(obj) ((Babl*)(obj))

static inline double babl_parse_double(const char* str)
{
    double result = 0;
    if (!str)
        return 0.0;
    result = atoi(str);
    if (strchr(str, '.')) {
        char* p = strchr(str, '.') + 1;
        double d = 10;
        for (; *p && *p >= '0' && *p <= '9'; p++, d *= 10) {
            if (result >= 0)
                result += (*p - '0') / d;
            else
                result -= (*p - '0') / d;
        }
    }
    return result;
}

const Babl* babl_remodel_with_space(const Babl* model, const Babl* space);
Babl* _conversion_new(const char* name, int32_t id, const Babl* source, const Babl* destination, BablFuncLinear linear, BablFuncPlane plane, BablFuncPlanar planar, void* user_data, int32_t allow_collision);

double _babl_legal_error(void);
void babl_init_db(void);
void babl_store_db(void);
int _babl_max_path_len(void);

extern const Babl* (*babl_trc_new)(const char* name, BablTRCType type, double gamma, int32_t n_lut, float* lut);

extern const Babl* (*babl_trc_lookup_by_name)(const char* name);

void babl_space_to_xyz(const Babl* space, const double* rgb, double* xyz);
void babl_space_from_xyz(const Babl* space, const double* xyz, double* rgb);

const Babl* babl_trc_lut_find(float* lut, int32_t lut_size);
const Babl* babl_trc_lut(const char* name, int32_t n, float* entries);

Babl* format_new_from_format_with_space(const Babl* format, const Babl* space);

int32_t babl_list_destroy(void* data);

const char* babl_conversion_create_name(Babl* source, Babl* destination, int32_t type, int32_t allow_collision);

extern void (*_babl_space_add_universal_rgb)(const Babl* space);
const Babl* babl_trc_formula_srgb(double gamma, double a, double b, double c, double d, double e, double f);
const Babl* babl_trc_formula_cie(double gamma, double a, double b, double c);

const Babl* babl_space_match_trc_matrix(const Babl* trc_red, const Babl* trc_green, const Babl* trc_blue, float rx, float ry, float rz, float gx, float gy, float gz, float bx, float by, float bz);

int _babl_file_get_contents(const char* path, char** contents, long* length, void* error);

const double* babl_space_get_rgbtoxyz(const Babl* space);

void babl_space_to_xyz(const Babl* space, const double* rgb, double* xyz);

void babl_space_from_xyz(const Babl* space, const double* xyz, double* rgb);

static inline void babl_conversion_process(const Babl* babl, const char* source, char* destination, long n)
{
    BablConversion* conversion = (BablConversion*)babl;
    conversion->dispatch(babl, source, destination, n, conversion->data);
}

void _babl_fish_missing_fast_path_warning(const Babl* source, const Babl* destination);
void _babl_fish_rig_dispatch(Babl* babl);
void _babl_fish_prepare_bpp(Babl* babl);

typedef enum {
    BABL_ICC_DEFAULTS = 0,
    BABL_ICC_COMPACT_TRC_LUT = 1,
} BablICCFlags;

char* babl_space_to_icc(const Babl* space, const char* description, const char* copyright, BablICCFlags flags, int32_t* icc_length);
Babl* _babl_space_for_lcms(const char* icc_data, int32_t icc_length);

void babl_trc_class_init(void);

#endif
