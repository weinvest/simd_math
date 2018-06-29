#ifndef _FAST_MATH_INT32X4_H
#define _FAST_MATH_INT32X4_H

#define CLS_NAME Int32x4
#define VAL_TYPE __m128i
#define FLOAT_VAL_TYPE __m128
#define VAL_ELE_TYPE int32_t
#define INT_VEC_TYPE __v4si
#define ITEM_COUNT 4

#define API_PREFIX _mm
#define API_SUBFIX _epi32
#define FAPI_SUBFIX _ps
#include "IntTemplate.h"
#endif //_FAST_MATH_INTX4_H
