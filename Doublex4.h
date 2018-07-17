#ifndef _FASTMATH_DOUBLEX4_H
#define _FASTMATH_DOUBLEX4_H

#define DOUBLEx4
#define CLS_NAME Doublex4
#define VAL_TYPE __m256d
#define INT32_TYPE __m128i
#define VAL_ELE_TYPE double
#define INT_VAL_TYPE __m256i
#define INT_ELE_TYPE int64_t
#define ITEM_COUNT 4

#define EXP_ITER_CNT 12
#define LOG_ITER_CNT 7

#define EXP_BIT_COUNT 11
#define MAN_BIT_COUNT 52

#define API_PREFIX _mm256
#define API_SUBFIX _pd
#define INTAPI_SUBFIX _epi64
#define INT32API_SUBFIX _epi32
#define INT_VEC_TYPE __v4di
#define CONVERT_2_INT cvtpd
#define CONVERT_FROM_INT cvtepi64
#define SLOW_CONVERT

#define RECIP_MAGIC_NUMBER 0x5FE6EB50C7B537A9
#include "FloatTemplate.h"

#undef DOUBLEx4
#undef CLS_NAME
#undef VAL_TYPE
#undef VAL_ELE_TYPE
#undef INT_VAL_TYPE
#undef INT_ELE_TYPE
#undef ITEM_COUNT
#undef EXP_ITER_CNT
#undef LOG_ITER_CNT
#undef EXP_BIT_COUNT
#undef MAN_BIT_COUNT
#undef API_PREFIX
#undef API_SUBFIX
#undef INTAPI_SUBFIX
#undef INT_VEC_TYPE
#undef CONVERT_2_INT
#undef CONVERT_FROM_INT 
#undef RECIP_MAGIC_NUMBER
#undef INT32_TYPE
#endif
