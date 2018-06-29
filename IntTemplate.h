#include <cmath>
#include <limits>
#include <smmintrin.h>
#include <immintrin.h>
#include <avx2intrin.h>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#define SIMDAPI(name, prefix, subfix) BOOST_PP_CAT(prefix, BOOST_PP_CAT(BOOST_PP_CAT(_, name),subfix))
#define HELP_FUNC BOOST_PP_CAT(Help, CLS_NAME)
struct HELP_FUNC
{
    [[gnu::always_inline]]
    static inline constexpr VAL_TYPE const_val(VAL_ELE_TYPE v)
    {
#define PARAM_V(z, n, d) BOOST_PP_COMMA_IF(n) v
        return (VAL_TYPE)INT_VEC_TYPE{BOOST_PP_REPEAT(ITEM_COUNT, PARAM_V, ~)};
#undef PARAM_V
    }
};

struct CLS_NAME {
    VAL_TYPE v;

    //==== constructor =======
    [[gnu::always_inline]]
    CLS_NAME()
            : v(HELP_FUNC::const_val(0)) {}

    [[gnu::always_inline]]
    CLS_NAME(VAL_ELE_TYPE vv)
            : v(HELP_FUNC::const_val(vv)) {}

    [[gnu::always_inline]]
    CLS_NAME (BOOST_PP_ENUM_PARAMS(ITEM_COUNT, VAL_ELE_TYPE
            v))
            :v(SIMDAPI(set, API_PREFIX, API_SUBFIX)(BOOST_PP_ENUM_PARAMS(ITEM_COUNT, v))) {}

    [[gnu::always_inline]]
    CLS_NAME(VAL_TYPE vv)
            : v(vv) {}

    [[gnu::always_inline]]
    inline operator VAL_TYPE() const { return v; }

    //==== const values ======
    static constexpr int32_t STEP_CNT = ITEM_COUNT;
    static constexpr int32_t BIT_CNT = sizeof(VAL_TYPE)*8;

    static constexpr VAL_TYPE CONST_m3 = HELP_FUNC::const_val(-3);
    static constexpr VAL_TYPE CONST_m2 = HELP_FUNC::const_val(-2);
    static constexpr VAL_TYPE CONST_m1 = HELP_FUNC::const_val(-1);
    static constexpr VAL_TYPE CONST_0 = HELP_FUNC::const_val(0);
    static constexpr VAL_TYPE CONST_1 = HELP_FUNC::const_val(1);
    static constexpr VAL_TYPE CONST_2 = HELP_FUNC::const_val(2);
    static constexpr VAL_TYPE CONST_3 = HELP_FUNC::const_val(3);

    static constexpr VAL_TYPE CONST_max = HELP_FUNC::const_val(std::numeric_limits<VAL_ELE_TYPE>::max());
    static constexpr VAL_TYPE CONST_min = HELP_FUNC::const_val(std::numeric_limits<VAL_ELE_TYPE>::min());


    static constexpr VAL_TYPE CONST_true = HELP_FUNC::const_val(-1L);
    static constexpr VAL_TYPE CONST_false = CONST_0;

    static constexpr VAL_TYPE SIGN_BIT_MASK = CONST_min;
    static constexpr VAL_TYPE INV_SIGN_BIT_MASK = CONST_max;

    //==== load store =========
    [[gnu::always_inline]]
    void load(VAL_ELE_TYPE *addr) {
        v = *(VAL_TYPE*)addr;
    }

    [[gnu::always_inline]]
    void store(VAL_ELE_TYPE *addr) {
        *(VAL_TYPE*)addr = v;
    }

    //===== arithmetic operation ===========
    [[gnu::always_inline]]
    static inline VAL_TYPE add(VAL_TYPE v1, VAL_TYPE v2) {
        return SIMDAPI(add, API_PREFIX, API_SUBFIX)(v1, v2);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE sub(VAL_TYPE v1, VAL_TYPE v2) {
        return SIMDAPI(sub, API_PREFIX, API_SUBFIX)(v1, v2);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE mul(VAL_TYPE v1, VAL_TYPE v2) {
        return SIMDAPI(mul, API_PREFIX, API_SUBFIX)(v1, v2);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE div(VAL_TYPE v1, VAL_TYPE v2) {
        return v1;
        //return SIMDAPI(div, API_PREFIX, API_SUBFIX)(v1, v2);
    }

    //====== logic operation ============
    [[gnu::always_inline]]
    static inline VAL_TYPE shift_right(VAL_TYPE v, int32_t cnt) {
        return (SIMDAPI(srli, API_PREFIX, API_SUBFIX)(v, cnt));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE shift_left(VAL_TYPE v, int32_t cnt) {
        return (SIMDAPI(slli, API_PREFIX, API_SUBFIX)(v, cnt));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE shiftv_right(VAL_TYPE v, VAL_TYPE cnt) {
        auto rrep = (SIMDAPI(srlv, API_PREFIX, API_SUBFIX)(v, cnt));
        return *(VAL_TYPE * ) & rrep;
    }


    [[gnu::always_inline]]
    static inline VAL_TYPE shiftv_left(VAL_TYPE v, VAL_TYPE cnt) {
        auto rrep = (SIMDAPI(sllv, API_PREFIX, API_SUBFIX)(v, cnt));
        return *(VAL_TYPE * ) & rrep;
    }

    [[gnu::always_inline]]
    static inline FLOAT_VAL_TYPE float_rep(VAL_TYPE v)
    {
        return *(FLOAT_VAL_TYPE*)&v;
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE rep_2_val(FLOAT_VAL_TYPE v)
    {
        return *(VAL_TYPE*)&v;
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE _and(VAL_TYPE v1, VAL_TYPE v2) {
        return v1 & v2;
        return rep_2_val(SIMDAPI( and , API_PREFIX, FAPI_SUBFIX)(float_rep(v1), float_rep(v2)));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE _or(VAL_TYPE v1, VAL_TYPE v2) {
        return v1 | v2;
        return rep_2_val(SIMDAPI( or , API_PREFIX, FAPI_SUBFIX)(float_rep(v1), float_rep(v2)));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE _xor(VAL_TYPE v1, VAL_TYPE v2) {
        return v1 ^ v2;
        return rep_2_val(SIMDAPI(xor, API_PREFIX, FAPI_SUBFIX)(float_rep(v1), float_rep(v2)));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE and_not(VAL_TYPE v1, VAL_TYPE v2) {
        return rep_2_val(SIMDAPI(andnot, API_PREFIX, FAPI_SUBFIX)(float_rep(v1), float_rep(v2)));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE _not(VAL_TYPE v) {
        return ~v;
    }

    // ==== double struct ===================
    [[gnu::always_inline]]
    static inline VAL_TYPE sign(VAL_TYPE v) {
        //>0 return 1
        //<0 return -1
        static const VAL_TYPE BASE_BIT_MASK = HELP_FUNC::const_val(1.0);
        return _or(_and(v, SIGN_BIT_MASK), BASE_BIT_MASK);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE sign_bit(VAL_TYPE v) {
        //>0 return 0 (0x0000000000000000)
        //<0 return CONST_min (0x8000000000000000)
        //if_then_else(pref < 0, v1, v2) <=> if_then_else(sign_bit(pred), v1, v2)
        return _and(v, SIGN_BIT_MASK);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE rsign_bit(VAL_TYPE v) {
        //>0 return CONST_min (0x8000000000000000)
        //<0 return 0 (0x0000000000000000)
        //if_then_else(pref >= 0, v1, v2) <=> if_then_else(rsign_bit(pred), v1, v2)
        return and_not(v, SIGN_BIT_MASK);
    }

    //======= compare result can be used as first param of if_then_else
    [[gnu::always_inline]]
    static inline VAL_TYPE less_than(VAL_TYPE first, VAL_TYPE second) {
        return SIMDAPI(cmplt, API_PREFIX, API_SUBFIX)(first, second);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE great_than(VAL_TYPE first, VAL_TYPE second) {
        return SIMDAPI(cmpgt, API_PREFIX, API_SUBFIX)(first, second);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE less_equal(VAL_TYPE first, VAL_TYPE second) {
        return _not(great_than(first, second));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE great_equal(VAL_TYPE first, VAL_TYPE second) {
        return _not(less_than(first, second));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE equal(VAL_TYPE first, VAL_TYPE second) {
        return SIMDAPI(cmpeq, API_PREFIX, API_SUBFIX)(first, second);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE not_equal(VAL_TYPE first, VAL_TYPE second) {
        return _not(equal(first, second));
    }

    //========== if_then_else======================
    [[gnu::always_inline]]
    static inline VAL_TYPE if_then_else(VAL_TYPE pred, VAL_TYPE first, VAL_TYPE second) {
        return rep_2_val(SIMDAPI(blendv, API_PREFIX, FAPI_SUBFIX)(float_rep(second), float_rep(first), float_rep(pred)));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE pred_2_num(VAL_TYPE pred) {
        //true(<0) ==> 1.0
        //false(>0) ==> 0.0
        //return add(mul(-half, sign(pred)), half);
        return if_then_else(pred, CONST_1, CONST_0);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE num_2_pred(VAL_TYPE num) {
        //1.0 ==> true(-nan)
        //0.0 ==> false(0.0)
        //return add(mul(-half, sign(pred)), half);
        return if_then_else(equal(num, CONST_0), CONST_false, CONST_true);
    }

    //=====min/max abs=================
    [[gnu::always_inline]]
    static inline VAL_TYPE abs(VAL_TYPE v) {
        return if_then_else(v, -v ,v);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE min(VAL_TYPE v1, VAL_TYPE v2) {
        return SIMDAPI(min, API_PREFIX, API_SUBFIX)(v1, v2);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE max(VAL_TYPE v1, VAL_TYPE v2) {
        return SIMDAPI(max, API_PREFIX, API_SUBFIX)(v1, v2);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE clamp(VAL_TYPE v, VAL_TYPE minV, VAL_TYPE maxV) {
        auto v1 = SIMDAPI(max, API_PREFIX, API_SUBFIX)(minV, v);
        auto v2 = SIMDAPI(min, API_PREFIX, API_SUBFIX)(maxV, v1);
        return v2;
    }
};

[[gnu::always_inline]]
inline  CLS_NAME operator+ (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::add(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator- (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::sub(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator* (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::mul(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator/ (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::div(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator< (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::less_than(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator<= (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::less_equal(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator> (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::great_than(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator>= (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::great_equal(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator== (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::equal(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator!= (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::not_equal(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator& (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::_and(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator| (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::_or(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator^ (CLS_NAME lhs, CLS_NAME rhs)
{
    return CLS_NAME(CLS_NAME::_xor(lhs.v, rhs.v));
}

[[gnu::always_inline]]
inline CLS_NAME operator~ (CLS_NAME v)
{
    return CLS_NAME(CLS_NAME::_not(v.v));
}