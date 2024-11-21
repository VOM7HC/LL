#ifndef _BABL_REF_PIXELS_HPP
#define _BABL_REF_PIXELS_HPP

int babl_get_num_path_test_pixels(void);
int babl_get_num_conversion_test_pixels(void);
int babl_get_num_format_test_pixels(void);
int babl_get_num_model_test_pixels(void);
int babl_get_num_type_test_pixels(void);

const double* babl_get_path_test_pixels(void);
const double* babl_get_conversion_test_pixels(void);
const double* babl_get_format_test_pixels(void);
const double* babl_get_model_test_pixels(void);
const double* babl_get_type_test_pixels(void);

#endif