// babl.cpp : Defines the exported functions for the DLL.
//

#include "babl.hpp"
#include "framework.h"
#include "pch.h"

// This is an example of an exported variable
BABL_API int nbabl = 0;

// This is an example of an exported function.
BABL_API int fnbabl(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Cbabl::Cbabl()
{
    return;
}

////////////////////////////////////////////////////

#include "babl-base.hpp"
#include "babl-internal.hpp"
#include "config.hpp"

#include <cstring>

static int ref_count = 0;

#ifndef _WIN32
#define BABL_PATH LIBDIR BABL_DIR_SEPARATOR BABL_LIBRARY
#endif

#if defined(_MSC_VER)
#define strcasecmp _stricmp
#endif

static char*
babl_dir_list(void)
{
    char* ret;

    ret = getenv("BABL_PATH");
    if (!ret) {
#ifdef _WIN32
        /* Figure it out from the location of this DLL */
        wchar_t w_filename[MAX_PATH];
        char* filename = NULL;
        char *sep1, *sep2;
        DWORD nSize = sizeof(w_filename) / sizeof((w_filename)[0]);

        if (GetModuleFileNameW(get_libbabl_module(), w_filename, nSize) == 0)
            babl_fatal("GetModuleFilenameW failed");

        filename = babl_convert_utf16_to_utf8(w_filename);
        if (!filename)
            babl_fatal("Converting module filename to UTF-8 failed");

        /* If the DLL file name is of the format
         * <foobar>\bin\*.dll, use <foobar>\lib\{BABL_LIBRARY}.
         * Otherwise, use the directory where the DLL is.
         */

        sep1 = strrchr(filename, BABL_DIR_SEPARATOR[0]);
        *sep1 = '\0';

        sep2 = strrchr(filename, BABL_DIR_SEPARATOR[0]);
        if (sep2 != NULL) {
            if (strcasecmp(sep2 + 1, "bin") == 0) {
                char* filename_tmp;
                *(++sep2) = '\0';
                filename_tmp = babl_malloc(sizeof(char) * (strlen(filename) + strlen(BABL_DIR_SEPARATOR BABL_LIBRARY) + 4));
                strcpy(filename_tmp, filename);
                babl_free(filename);
                strcat(filename_tmp, "lib" BABL_DIR_SEPARATOR BABL_LIBRARY);
                filename = filename_tmp;
            }
        }

        ret = filename;
#else
        ret = babl_malloc(sizeof(char) * (strlen(BABL_PATH) + 1));
        strcpy(ret, BABL_PATH);
#endif
    }
    else {
        char* ret_tmp = babl_malloc(sizeof(char) * (strlen(ret) + 1));
        strcpy(ret_tmp, ret);
        ret = ret_tmp;
    }
    return ret;
}

static const char** simd_init(void);
void babl_init(void)
{
    const char** exclusion_pattern;
    babl_cpu_accel_set_use(1);
    exclusion_pattern = simd_init();

    if (ref_count++ == 0) {
        char* dir_list;

        babl_internal_init();
        babl_sampling_class_init();
        babl_type_db();
        babl_trc_class_init();
        babl_space_class_init();
        _babl_legal_error();
        babl_component_db();
        babl_model_db();
        babl_format_db();
        babl_conversion_db();
        babl_extension_db();
        babl_fish_db();
        babl_core_init();
        babl_sanity();
        babl_extension_base();
        babl_sanity();

        dir_list = babl_dir_list();
        babl_extension_load_dir_list(dir_list, exclusion_pattern);
        babl_free(dir_list);

        if (!getenv("BABL_INHIBIT_CACHE"))
            babl_init_db();
    }
}

void babl_exit(void)
{
    if (!--ref_count) {
        babl_store_db();

        babl_extension_deinit();
        babl_free(babl_extension_db());
        ;
        babl_free(babl_fish_db());
        ;
        babl_free(babl_conversion_db());
        ;
        babl_free(babl_format_db());
        ;
        babl_free(babl_model_db());
        ;
        babl_free(babl_component_db());
        ;
        babl_free(babl_type_db());
        ;

        babl_internal_destroy();
#if BABL_DEBUG_MEM
        babl_memory_sanity();
#endif
    }
}

#undef babl_model_is

int babl_model_is(const Babl* babl,
    const char* model)
{
    return babl && ((babl) == babl_model_with_space(model, babl));
}

#include "babl-cpuaccel.hpp"
void (*babl_base_init)(void) = babl_base_init_generic;

void babl_base_init_generic(void)
{
}

const Babl* babl_trc_lookup_by_name_generic(const char* name);

const Babl*
babl_trc_new_generic(const char* name,
    BablTRCType type,
    double gamma,
    int n_lut,
    float* lut);

void _babl_space_add_universal_rgb_generic(const Babl* space);
void (*_babl_space_add_universal_rgb)(const Babl* space) = _babl_space_add_universal_rgb_generic;

const Babl* (*babl_trc_lookup_by_name)(const char* name) = babl_trc_lookup_by_name_generic;
const Babl* (*babl_trc_new)(const char* name,
    BablTRCType type,
    double gamma,
    int n_lut,
    float* lut)
    = babl_trc_new_generic;

#ifdef _M_X64
void babl_base_init_x86_64_v2(void);
void babl_base_init_x86_64_v3(void);
void _babl_space_add_universal_rgb_x86_64_v2(const Babl* space);
void _babl_space_add_universal_rgb_x86_64_v3(const Babl* space);

const Babl*
babl_trc_lookup_by_name_x86_64_v2(const char* name);
const Babl*
babl_trc_lookup_by_name_x86_64_v3(const char* name);

const Babl*
babl_trc_new_x86_64_v2(const char* name,
    BablTRCType type,
    double gamma,
    int n_lut,
    float* lut);
const Babl*
babl_trc_new_x86_64_v3(const char* name,
    BablTRCType type,
    double gamma,
    int n_lut,
    float* lut);

#endif
#ifdef _M_ARM64
void babl_base_init_arm_neon(void);
void _babl_space_add_universal_rgb_arm_neon(const Babl* space);

const Babl*
babl_trc_lookup_by_name_arm_neon(const char* name);

const Babl*
babl_trc_new_arm_neon(const char* name,
    BablTRCType type,
    double gamma,
    int n_lut,
    float* lut);

#endif

static const char** simd_init(void)
{
    static const char* exclude[] = { "neon-", "x86-64-v3", "x86-64-v2", NULL };
#ifdef _M_X64
    BablCpuAccelFlags accel = babl_cpu_accel_get_support();
    if ((accel & BABL_CPU_ACCEL_X86_64_V3) == BABL_CPU_ACCEL_X86_64_V3) {
        static const char* exclude[] = { NULL };
        babl_base_init = babl_base_init_x86_64_v2; /// !!
                                                   // this is correct,
                                                   // it performs better
                                                   // as observed in benchmarking
        babl_trc_new = babl_trc_new_x86_64_v2;
        babl_trc_lookup_by_name = babl_trc_lookup_by_name_x86_64_v2;
        _babl_space_add_universal_rgb = _babl_space_add_universal_rgb_x86_64_v3;
        return exclude;
    }
    else if ((accel & BABL_CPU_ACCEL_X86_64_V2) == BABL_CPU_ACCEL_X86_64_V2) {
        static const char* exclude[] = { "x86-64-v3-", NULL };
        babl_base_init = babl_base_init_x86_64_v2;
        babl_trc_new = babl_trc_new_x86_64_v2;
        babl_trc_lookup_by_name = babl_trc_lookup_by_name_x86_64_v2;
        _babl_space_add_universal_rgb = _babl_space_add_universal_rgb_x86_64_v2;
        return exclude;
    }
    else {
        static const char* exclude[] = { "x86-64-v3-", "x86-64-v2-", NULL };
        return exclude;
    }
#endif
#ifdef _M_ARM64
    BablCpuAccelFlags accel = babl_cpu_accel_get_support();
    if ((accel & BABL_CPU_ACCEL_ARM_NEON) == BABL_CPU_ACCEL_ARM_NEON) {
        static const char* exclude[] = { NULL };
        babl_base_init = babl_base_init_arm_neon;
        babl_trc_new = babl_trc_new_arm_neon;
        babl_trc_lookup_by_name = babl_trc_lookup_by_name_arm_neon;
        _babl_space_add_universal_rgb = _babl_space_add_universal_rgb_arm_neon;
        return exclude;
    }
    else {
        static const char* exclude[] = { "arm-neon-", NULL };
        return exclude;
    }
#endif
    return exclude;
}

/////////////////////////////////////////////////////////////

#include "config.hpp"

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "babl-shared-util.hpp"

#include "babl.hpp"

static const Babl* babl_cli_get_space(const char* path,
    BablIccIntent intent);
static void babl_cli_print_usage(FILE* stream);

int main(int argc,
    char** argv)
{
    const Babl* from_format;
    const Babl* from_space = NULL;
    const Babl* to_format;
    const Babl* to_space = NULL;
    const Babl* fish;
    const char* from = NULL;
    const char* to = NULL;
    const char* from_profile = NULL;
    const char* to_profile = NULL;
    BablIccIntent intent = BABL_ICC_INTENT_RELATIVE_COLORIMETRIC;
    char* source;
    char* dest;
    int set_from = 0;
    int set_to = 0;
    int set_from_profile = 0;
    int set_to_profile = 0;
    int set_intent = 0;
    int brief_output = 0;
    int options_ended = 0;
    int n_components;
    int data_index;
    int c;
    int i;

    babl_init();

    if (argc == 1) {
        babl_cli_print_usage(stderr);
        return 2;
    }

    /* Looping through arguments to get source and destination formats. */
    for (i = 1; i < argc; i++) {
        if (set_from) {
            from = argv[i];
            set_from = 0;
            if (!babl_format_exists(from)) {
                fprintf(stderr, "babl: unknown format: %s\n", from);
                return 1;
            }
        }
        else if (set_to) {
            to = argv[i];
            set_to = 0;
            if (!babl_format_exists(to)) {
                fprintf(stderr, "babl: unknown format: %s\n", to);
                return 1;
            }
        }
        else if (set_from_profile) {
            set_from_profile = 0;
            from_profile = argv[i];
        }
        else if (set_to_profile) {
            set_to_profile = 0;
            to_profile = argv[i];
        }
        else if (set_intent) {
            set_intent = 0;

            if (strcmp(argv[i], "perceptual") == 0) {
                intent = BABL_ICC_INTENT_PERCEPTUAL;
            }
            else if (strcmp(argv[i], "relative") == 0) {
                intent = BABL_ICC_INTENT_RELATIVE_COLORIMETRIC;
            }
            else if (strcmp(argv[i], "saturation") == 0) {
                intent = BABL_ICC_INTENT_SATURATION;
            }
            else if (strcmp(argv[i], "absolute") == 0) {
                intent = BABL_ICC_INTENT_ABSOLUTE_COLORIMETRIC;
            }
            else {
                fprintf(stderr, "babl: unknown intent: %s\n", argv[i]);
                fprintf(stderr, "valid intents: perceptual, relative, saturation, absolute.\n");
                return 2;
            }
        }
        else if (strcmp(argv[i], "--") == 0) {
            break;
        }
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            babl_cli_print_usage(stdout);

            return 0;
        }
        else if (strcmp(argv[i], "--from") == 0 || strcmp(argv[i], "-f") == 0) {
            set_from = 1;
        }
        else if (strcmp(argv[i], "--to") == 0 || strcmp(argv[i], "-t") == 0) {
            set_to = 1;
        }
        else if (strcmp(argv[i], "--input-space") == 0 ||
            /* Deprecated option name, but keep compatibility. */
            strcmp(argv[i], "--input-profile") == 0 || strcmp(argv[i], "-i") == 0) {
            if (strcmp(argv[i], "--input-profile") == 0)
                fprintf(stderr, "babl: warning: --input-profile option renamed --input-space\n");
            set_from_profile = 1;
        }
        else if (strcmp(argv[i], "--output-space") == 0 ||
            /* Deprecated option name, but keep compatibility. */
            strcmp(argv[i], "--output-profile") == 0 || strcmp(argv[i], "-o") == 0) {
            if (strcmp(argv[i], "--output-profile") == 0)
                fprintf(stderr, "babl: warning: --output-profile option renamed --output-space\n");
            set_to_profile = 1;
        }
        else if (strcmp(argv[i], "--intent") == 0 || strcmp(argv[i], "-r") == 0) {
            set_intent = 1;
        }
        else if (strcmp(argv[i], "--brief") == 0 || strcmp(argv[i], "-b") == 0) {
            brief_output = 1;
        }
    }

    if (from_profile != NULL) {
        if (strlen(from_profile) > 2 && *from_profile == '*') {
            if (from_profile[1] != '*')
                from_space = babl_space(from_profile + 1);
            else
                from_space = babl_cli_get_space(from_profile + 1, intent);
        }
        else {
            from_space = babl_cli_get_space(from_profile, intent);
        }

        if (!from_space) {
            if (strlen(from_profile) > 2 && *from_profile == '*' && from_profile[1] != '*') {
                fprintf(stderr, "babl: unknown named space '%s'\n",
                    from_profile + 1);
                fprintf(stderr, "      Known spaces: sRGB, scRGB (sRGB with linear TRCs), Rec2020, "
                                "Adobish, Apple, ProPhoto, ACEScg and ACES2065-1.\n");
            }
            return 6;
        }
    }

    if (to_profile != NULL) {
        if (strlen(to_profile) > 2 && *to_profile == '*') {
            if (to_profile[1] != '*')
                to_space = babl_space(to_profile + 1);
            else
                to_space = babl_cli_get_space(to_profile + 1, intent);
        }
        else {
            to_space = babl_cli_get_space(to_profile, intent);
        }

        if (!to_space) {
            if (strlen(to_profile) > 2 && *to_profile == '*' && to_profile[1] != '*') {
                fprintf(stderr, "babl: unknown named space '%s'\n",
                    to_profile + 1);
                fprintf(stderr, "      Known spaces: sRGB, scRGB (sRGB with linear TRCs), Rec2020, "
                                "Adobish, Apple, ProPhoto, ACEScg and ACES2065-1.\n");
            }
            return 6;
        }
    }

    if (from == NULL) {
        if (babl_space_is_cmyk(from_space))
            from = "CMYK float";
        else if (babl_space_is_gray(from_space))
            from = "Y' float";
        else
            from = "R'G'B' float";
    }
    if (to == NULL) {
        if (babl_space_is_cmyk(to_space))
            to = "CMYK float";
        else if (babl_space_is_gray(to_space))
            to = "Y' float";
        else
            to = "R'G'B' float";
    }

    from_format = babl_format_with_space(from, from_space);
    n_components = babl_format_get_n_components(from_format);
    source = malloc(babl_format_get_bytes_per_pixel(from_format));
    data_index = 0;

    to_format = babl_format_with_space(to, to_space);
    dest = malloc(babl_format_get_bytes_per_pixel(to_format));

    /* Re-looping through arguments, to be more flexible with argument orders.
     * In this second loop, we get the source components' values.
     */
    set_from = set_to = set_to_profile = set_from_profile = 0;
    for (i = 1, c = 0; i < argc; i++) {
        if (set_from) {
            set_from = 0;
            /* Pass. */
        }
        else if (set_to) {
            set_to = 0;
            /* Pass. */
        }
        else if (set_from_profile) {
            set_from_profile = 0;
            /* Pass. */
        }
        else if (set_to_profile) {
            set_to_profile = 0;
            /* Pass. */
        }
        else if (set_intent) {
            set_intent = 0;
            /* Pass. */
        }
        else if (!options_ended && strncmp(argv[i], "-", 1) == 0) {
            if (strcmp(argv[i], "--") == 0) {
                options_ended = 1;
            }
            else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
                /* Pass. */
            }
            else if (strcmp(argv[i], "--from") == 0 || strcmp(argv[i], "-f") == 0) {
                set_from = 1;
            }
            else if (strcmp(argv[i], "--to") == 0 || strcmp(argv[i], "-t") == 0) {
                set_to = 1;
            }
            else if (strcmp(argv[i], "--input-profile") == 0 || strcmp(argv[i], "--input-space") == 0 || strcmp(argv[i], "-i") == 0) {
                set_from_profile = 1;
            }
            else if (strcmp(argv[i], "--output-profile") == 0 || strcmp(argv[i], "--output-space") == 0 || strcmp(argv[i], "-o") == 0) {
                set_to_profile = 1;
            }
            else if (strcmp(argv[i], "--intent") == 0 || strcmp(argv[i], "-r") == 0) {
                set_intent = 1;
            }
            else if (strcmp(argv[i], "--brief") == 0 || strcmp(argv[i], "-b") == 0) {
                /* Pass. */
            }
            else {
                fprintf(stderr, "babl: unknown option: %s\n", argv[i]);
                babl_cli_print_usage(stderr);
                return 2;
            }
        }
        else {
            const Babl* arg_type;
            char* endptr = NULL;

            if (c >= n_components) {
                fprintf(stderr, "babl: unexpected component: %s\n", argv[i]);
                babl_cli_print_usage(stderr);
                return 2;
            }

            arg_type = babl_format_get_type(from_format, c);

            if (strcmp(babl_get_name(arg_type), "double") == 0) {
                double value = strtod(argv[i], &endptr);
                double* dsrc = (double*)(source + data_index);

                if (value == 0.0 && endptr == argv[i]) {
                    fprintf(stderr, "babl: expected type of component %d is '%s', invalid value: %s\n",
                        c, babl_get_name(arg_type), argv[i]);
                    return 3;
                }

                *dsrc = value;
                data_index += 8;
            }
            else if (strcmp(babl_get_name(arg_type), "float") == 0) {
                float value = strtof(argv[i], &endptr);
                float* fsrc = (float*)(source + data_index);

                if (value == 0.0f && endptr == argv[i]) {
                    fprintf(stderr, "babl: expected type of component %d is '%s', invalid value: %s\n",
                        c, babl_get_name(arg_type), argv[i]);
                    return 3;
                }

                *fsrc = value;
                data_index += 4;
            }
            else if (strcmp(babl_get_name(arg_type), "half") == 0) {
                float value = strtof(argv[i], &endptr);
                void* fsrc = (void*)(source + data_index);

                if (value == 0.0f && endptr == argv[i]) {
                    fprintf(stderr, "babl: expected type of component %d is '%s', invalid value: %s\n",
                        c, babl_get_name(arg_type), argv[i]);
                    return 3;
                }

                _babl_float_to_half(fsrc, &value, 1);
                data_index += 2;
            }
            else if (strncmp(babl_get_name(arg_type), "u", 1) == 0) {
                long int value = strtol(argv[i], &endptr, 10);

                if (value == 0 && endptr == argv[i]) {
                    fprintf(stderr, "babl: expected type of component %d is '%s', invalid value: %s\n",
                        c, babl_get_name(arg_type), argv[i]);
                    return 3;
                }

                if (strcmp(babl_get_name(arg_type), "u8") == 0) {
                    uint8_t* usrc = (uint8_t*)(source + data_index);

                    *usrc = value;
                    data_index += 1;
                }
                else if (strcmp(babl_get_name(arg_type), "u16") == 0) {
                    uint16_t* usrc = (uint16_t*)(source + data_index);

                    *usrc = value;
                    data_index += 2;
                }
                else if (strcmp(babl_get_name(arg_type), "u32") == 0) {
                    uint32_t* usrc = (uint32_t*)(source + data_index);

                    *usrc = value;
                    data_index += 4;
                }
                else {
                    fprintf(stderr, "babl: unsupported unsigned type '%s' of component %d: %s\n",
                        babl_get_name(arg_type), c, argv[i]);
                    return 4;
                }
            }
            else {
                fprintf(stderr, "babl: unsupported type '%s' of component %d: %s\n",
                    babl_get_name(arg_type), c, argv[i]);
                return 4;
            }

            c++;
        }
    }

    if (c != n_components) {
        fprintf(stderr, "babl: %d components expected, %d components were passed\n",
            n_components, c);
        babl_cli_print_usage(stderr);
        return 2;
    }

    /* Actual processing. */
    fish = babl_fish(from_format, to_format);
    babl_process(fish, source, dest, 1);

    /* Now displaying the result. */
    n_components = babl_format_get_n_components(to_format);
    data_index = 0;

    if (!brief_output)
        printf("Converting from \"%s\" to \"%s\":\n",
            babl_get_name(from_format),
            babl_get_name(to_format));

    for (c = 0; c < n_components; c++) {
        const Babl* arg_type = NULL;

        arg_type = babl_format_get_type(to_format, c);

        if (strcmp(babl_get_name(arg_type), "double") == 0) {
            double value = *((double*)(dest + data_index));

            data_index += 8;

            if (brief_output)
                printf("%s%f", c > 0 ? " " : "", value);
            else
                printf("- %f\n", value);
        }
        else if (strcmp(babl_get_name(arg_type), "float") == 0) {
            float value = *((float*)(dest + data_index));

            data_index += 4;

            if (brief_output)
                printf("%s%f", c > 0 ? " " : "", value);
            else
                printf("- %f\n", value);
        }
        else if (strcmp(babl_get_name(arg_type), "half") == 0) {
            void* value = (void*)(dest + data_index);
            float fvalue;

            _babl_half_to_float(&fvalue, value, 1);
            data_index += 2;

            if (brief_output)
                printf("%s%f", c > 0 ? " " : "", fvalue);
            else
                printf("- %f\n", fvalue);
        }
        else if (strcmp(babl_get_name(arg_type), "u8") == 0) {
            uint8_t value = *((uint8_t*)(dest + data_index));

            data_index += 1;

            if (brief_output)
                printf("%s%d", c > 0 ? " " : "", value);
            else
                printf("- %d\n", value);
        }
        else if (strcmp(babl_get_name(arg_type), "u16") == 0) {
            uint16_t value = *((uint16_t*)(dest + data_index));

            data_index += 2;

            if (brief_output)
                printf("%s%d", c > 0 ? " " : "", value);
            else
                printf("- %d\n", value);
        }
        else if (strcmp(babl_get_name(arg_type), "u32") == 0) {
            uint32_t value = *((uint32_t*)(dest + data_index));

            data_index += 4;

            if (brief_output)
                printf("%s%d", c > 0 ? " " : "", value);
            else
                printf("- %d\n", value);
        }
        else {
            fprintf(stderr, "babl: unsupported type '%s' of returned component %d.\n",
                babl_get_name(arg_type), c);
            return 5;
        }
    }

    babl_exit();

    free(source);
    free(dest);

    return 0;
}

static const Babl*
babl_cli_get_space(const char* path,
    BablIccIntent intent)
{
    const Babl* space;
    FILE* f;
    char* icc_data;
    long icc_length;
    const char* error = NULL;

    f = fopen(path, "r");

    if (f == NULL) {
        fprintf(stderr, "babl: failed to open '%s': %s\n",
            path, strerror(errno));
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    icc_length = ftell(f);
    fseek(f, 0, SEEK_SET);

    icc_data = malloc(icc_length);
    fread(icc_data, icc_length, 1, f);

    fclose(f);

    space = babl_space_from_icc(icc_data, icc_length, intent, &error);

    if (space == NULL) {
        fprintf(stderr, "babl: failed to load space from '%s': %s\n",
            path, error);
        return NULL;
    }

    return space;
}

static void
babl_cli_print_usage(FILE* stream)
{
    fprintf(stream,
        "Usage: babl [options] [c1 ..]\n"
        "Convert color data from a specific Babl format and space to another.\n"
        "\n"
        "  Options:\n"
        "     -h, --help            this help information\n"
        "\n"
        "     -f, --from            input Babl format\n"
        "\n"
        "     -t, --to              output Babl format\n"
        "\n"
        "     -i, --input-space     input profile or named space\n"
        "                           named spaced are asterisk-prefixed, i.e. '*Rec2020'\n"
        "                           as special-case, double the first asterisk if your profile path starts with '*'\n"
        "\n"
        "     -o, --output-space    output profile or named space\n"
        "                           named spaced are asterisk-prefixed, i.e. '*Rec2020'\n"
        "                           as special-case, double the first asterisk if your profile path starts with '*'\n"
        "\n"
        "     -r, --intent          rendering intent\n"
        "                           it only works with an output profile\n"
        "\n"
        "     -b, --brief           brief output\n"
        "                           it can be re-entered as input for chain conversions\n"
        "\n"
        "All parameters following -- are considered components values. "
        "This is useful to input negative components.\n\n"
        "The tool expects exactly the number of components of your input format.\n\n"
        "The default input and output formats are \"R'G'B' float\" (respectively \"CMYK float\" "
        "or \"Y' float\" if you specified a CMYK or grayscale profile).\n\n"
        "The default space is sRGB for RGB formats or a naive CMYK space for CMYK formats.\n"
        "Other spaces can be specified through an ICC profile or a named space prefixed by "
        "an asterisk.\n"
        "Known spaces: sRGB, scRGB (sRGB with linear TRCs), Rec2020, Adobish, Apple, ProPhoto, "
        "ACEScg and ACES2065-1.\n");
}
