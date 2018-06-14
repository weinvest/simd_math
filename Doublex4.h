#ifndef _FASTMATH_DOUBLEX4_H
#define _FASTMATH_DOUBLEX4_H

#define CLS_NAME Doublex4
#define VAL_TYPE __m256d
#define VAL_ELE_TYPE double
#define INT_VAL_TYPE __m256i
#define INT_ELE_TYPE int64_t
#define ITEM_COUNT 4
#define EXP_ITER_CNT 11
#define LOG_ITER_CNT 7
#define EXPO_BIT_CNT 11
#define MANT_BIT_CNT 52
#define API_PREFIX _m256
#define API_SUBFIX _pd
#define INTAPI_SUBFIX _epi64

#include "FloatTemplate.h"
#endif
