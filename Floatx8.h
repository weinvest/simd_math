#ifndef _FAST_MATH_FLOATX8_H
#define _FAST_MATH_FLOATX8_H

#define CLS_NAME Floatx8
#define VAL_TYPE __m256
#define VAL_ELE_TYPE float
#define INT_VAL_TYPE __m256i
#define INT_ELE_TYPE int32_t
#define ITEM_COUNT 8

#define EXP_ITER_CNT 8
#define LOG_ITER_CNT 5

#define EXP_BIT_COUNT 8
#define MAN_BIT_COUNT 23

#define API_PREFIX _mm256
#define API_SUBFIX _ps
#define INTAPI_SUBFIX _epi32

#define CONVERT_2_INT cvtps

#define RECIP_MAGIC_NUMBER 0x5F375A86
#include "FloatTemplate.h"

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
#undef CONVERT_2_INT
#undef RECIP_MAGIC_NUMBER

#endif
