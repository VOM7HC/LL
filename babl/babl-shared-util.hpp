#ifndef _BABL_SHARED_UTIL_HPP
#define _BABL_SHARED_UTIL_HPP

void _babl_float_to_half(void* halfp, const float* floatp, int numel);
void _babl_half_to_float(float* floatp, const void* halfp, int numel);

#endif
