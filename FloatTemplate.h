#include <cmath>
#include <limits>
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
        return VAL_TYPE{BOOST_PP_REPEAT(ITEM_COUNT, PARAM_V, ~)};
    #undef PARAM_V
    }
    
    [[gnu::always_inline]]
    static inline constexpr INT_VAL_TYPE const_rep(INT_ELE_TYPE srep)
    {
    #define PARAM_V(z, n, d) BOOST_PP_COMMA_IF(n) srep
        return (INT_VAL_TYPE)INT_VEC_TYPE{BOOST_PP_REPEAT(ITEM_COUNT, PARAM_V, ~)};
    #undef PARAM_V
    }
    
    
    [[gnu::always_inline]]
    static inline constexpr VAL_TYPE rep_2_val(INT_ELE_TYPE srep)
    {
        auto rep = const_rep(srep);
        return (VAL_TYPE)rep;
    }
    
    [[gnu::always_inline]]
    static inline INT_VAL_TYPE convert_2_int(VAL_TYPE v)
    {
    #ifndef SLOW_CONVERT
        return SIMDAPI(CONVERT_2_INT, API_PREFIX, INTAPI_SUBFIX)(v);
    #else
        auto v1 = _mm256_cvtpd_epi32(v);
        return _mm256_cvtepi32_epi64(v1);
    #endif
    }
    
    [[gnu::always_inline]]
    static inline VAL_TYPE int_2_val(INT_VAL_TYPE iv)
    {
    #ifndef SLOW_CONVERT
        return SIMDAPI(CONVERT_FROM_INT, API_PREFIX, API_SUBFIX)(iv);
    #else
        static const auto i64_2_i32 = _mm256_set_epi32(0,0,0,0,6,4,2,0);
        auto v1 = _mm256_permutevar8x32_epi32(iv, i64_2_i32);
        auto v2 = _mm256_castsi256_si128(v1);
        return _mm256_cvtepi32_pd(v2);
    #endif
    }
};
struct CLS_NAME
{
    VAL_TYPE v;

    //==== constructor =======
    [[gnu::always_inline]]
    CLS_NAME()
            :v(SIMDAPI(setzero, API_PREFIX, API_SUBFIX)())
    {}

    [[gnu::always_inline]]
    CLS_NAME(VAL_ELE_TYPE vv)
    :v(HELP_FUNC::const_val(vv))
    {}

    [[gnu::always_inline]]
    CLS_NAME(BOOST_PP_ENUM_PARAMS(ITEM_COUNT, VAL_ELE_TYPE v))
            :v(SIMDAPI(set, API_PREFIX, API_SUBFIX)(BOOST_PP_ENUM_PARAMS(ITEM_COUNT, v)))
    {}

    [[gnu::always_inline]]
    CLS_NAME(VAL_TYPE vv)
            :v(vv)
    {}

    [[gnu::always_inline]]
    inline operator VAL_TYPE () const { return v; }

    [[gnu::always_inline]]
    inline INT_VAL_TYPE to_int() const { return HELP_FUNC::convert_2_int(v); }

    [[gnu::always_inline]]
    inline INT32_TYPE to_int32() const { return SIMDAPI(CONVERT_2_INT,API_PREFIX, INT32API_SUBFIX)(v);}

    [[gnu::always_inline]]
    inline INT_VAL_TYPE to_int64() const { return HelpDoublex4::convert_2_int(v); }

    //==== const values ======
    static constexpr int32_t EXPO_BIT_CNT = EXP_BIT_COUNT;
    static constexpr int32_t MANT_BIT_CNT = MAN_BIT_COUNT;
    static constexpr int32_t STEP_CNT = ITEM_COUNT;


    static constexpr INT_ELE_TYPE SINGLE_SIGN_BIT_MASK = 1UL << (sizeof(VAL_ELE_TYPE)*8-1);
    static constexpr INT_ELE_TYPE SINGLE_EXPO_BIT_MASK = ((1UL << EXPO_BIT_CNT) - 1) << MANT_BIT_CNT;
    static constexpr INT_ELE_TYPE SINGLE_MANT_BIT_MASK = ((1UL << MANT_BIT_CNT) - 1);

    static constexpr INT_ELE_TYPE SINGLE_MAX_VALUE_REP = (((1UL << EXPO_BIT_CNT) - 2) << MANT_BIT_CNT) | SINGLE_MANT_BIT_MASK;
    static constexpr INT_ELE_TYPE SINGLE_MIN_VALUE_REP = SINGLE_MAX_VALUE_REP | SINGLE_SIGN_BIT_MASK;
    static constexpr INT_ELE_TYPE SINGLE_MIN_NORM_VALUE_REP = (1UL << MANT_BIT_CNT);

    static constexpr VAL_ELE_TYPE SINGLE_MAX_VALUE = std::numeric_limits<VAL_ELE_TYPE>::max();
    static constexpr VAL_ELE_TYPE SINGLE_MIN_VALUE = -std::numeric_limits<VAL_ELE_TYPE>::max();
    static constexpr VAL_ELE_TYPE SINGLE_MIN_NORM_VALUE = std::numeric_limits<VAL_ELE_TYPE>::min();
    static constexpr VAL_ELE_TYPE SINGLE_INF_VALUE = std::numeric_limits<VAL_ELE_TYPE>::infinity();

    static constexpr VAL_TYPE CONST_m3 = HELP_FUNC::const_val(-3.0);
    static constexpr VAL_TYPE CONST_m2p5 = HELP_FUNC::const_val(-2.5);
    static constexpr VAL_TYPE CONST_m2 = HELP_FUNC::const_val(-2.0);
    static constexpr VAL_TYPE CONST_m1p5 = HELP_FUNC::const_val(-1.5);
    static constexpr VAL_TYPE CONST_m1 = HELP_FUNC::const_val(-1.0);
    static constexpr VAL_TYPE CONST_m0p5 = HELP_FUNC::const_val(-0.5);
    static constexpr VAL_TYPE CONST_0 = HELP_FUNC::const_val(0.0);
    static constexpr VAL_TYPE CONST_0p5 = HELP_FUNC::const_val(0.5);
    static constexpr VAL_TYPE CONST_1 = HELP_FUNC::const_val(1.0);
    static constexpr VAL_TYPE CONST_1p5 = HELP_FUNC::const_val(1.5);
    static constexpr VAL_TYPE CONST_2 = HELP_FUNC::const_val(2.0);
    static constexpr VAL_TYPE CONST_2p5 = HELP_FUNC::const_val(2.5);
    static constexpr VAL_TYPE CONST_3 = HELP_FUNC::const_val(3.0);

    static constexpr VAL_TYPE CONST_max = HELP_FUNC::const_val(SINGLE_MAX_VALUE);
    static constexpr VAL_TYPE CONST_min = HELP_FUNC::const_val(SINGLE_MIN_VALUE);
    static constexpr VAL_TYPE CONST_min_norm = HELP_FUNC::const_val(SINGLE_MIN_NORM_VALUE);
    static constexpr VAL_TYPE CONST_inf = HELP_FUNC::const_val(SINGLE_INF_VALUE);


    static constexpr VAL_TYPE CONST_true = HELP_FUNC::rep_2_val(-1L);
    static constexpr VAL_TYPE CONST_false = CONST_0;

    static constexpr VAL_TYPE SIGN_BIT_MASK = HELP_FUNC::rep_2_val(SINGLE_SIGN_BIT_MASK);
    static constexpr VAL_TYPE EXPO_BIT_MASK = HELP_FUNC::rep_2_val(SINGLE_EXPO_BIT_MASK);
    static constexpr VAL_TYPE MANT_BIT_MASK = HELP_FUNC::rep_2_val(SINGLE_MANT_BIT_MASK);

    static constexpr VAL_TYPE INV_SIGN_BIT_MASK = HELP_FUNC::rep_2_val(~SINGLE_SIGN_BIT_MASK);
    static constexpr VAL_TYPE INV_EXPO_BIT_MASK = HELP_FUNC::rep_2_val(~SINGLE_EXPO_BIT_MASK);
    static constexpr VAL_TYPE INV_MANT_BIT_MASK = HELP_FUNC::rep_2_val(-SINGLE_MANT_BIT_MASK);

    static constexpr INT_VAL_TYPE BIAS = HELP_FUNC::const_rep((1U << (EXPO_BIT_CNT-1))-1);

    //==== load store =========
    [[gnu::always_inline]]
    void load(VAL_ELE_TYPE* addr)
    {
        v = SIMDAPI(load, API_PREFIX, API_SUBFIX)(addr);
    }

    [[gnu::always_inline]]
    void store(VAL_ELE_TYPE* addr)
    {
        SIMDAPI(store, API_PREFIX, API_SUBFIX)(addr, v);
    }

    static void fill(VAL_ELE_TYPE* addr, VAL_ELE_TYPE v, int32_t cnt) {
        CLS_NAME vv(v);
        auto restCnt = cnt % STEP_CNT;
        auto fastCnt = (cnt - restCnt) / STEP_CNT;
        for(auto i = 0; i < fastCnt; i += STEP_CNT)
        {
            vv.store(addr + i);
        }

        for(auto i = cnt - 1; 0 != restCnt; --i, --restCnt)
        {
            addr[i] = v;
        }
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
        return SIMDAPI(div, API_PREFIX, API_SUBFIX)(v1, v2);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE fuse_mul_add(VAL_TYPE a, VAL_TYPE b, VAL_TYPE c) {
        return SIMDAPI(fmadd, API_PREFIX, API_SUBFIX)(a, b, c);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE fuse_mul_sub(VAL_TYPE a, VAL_TYPE b, VAL_TYPE c) {
        return SIMDAPI(fmsub, API_PREFIX, API_SUBFIX)(a, b, c);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE fuse_nmul_add(VAL_TYPE a, VAL_TYPE b, VAL_TYPE c) {
        return SIMDAPI(fnmadd, API_PREFIX, API_SUBFIX)(a, b, c);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE fuse_nmul_sub(VAL_TYPE a, VAL_TYPE b, VAL_TYPE c) {
        return SIMDAPI(fnmsub, API_PREFIX, API_SUBFIX)(a, b, c);
    }

    //====== logic operation ============
    [[gnu::always_inline]]
    static inline INT_VAL_TYPE rep(VAL_TYPE v)
    {
        return *(INT_VAL_TYPE*)&v;
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE shift_right(VAL_TYPE v, int32_t cnt)
    {
        auto rrep = (SIMDAPI(srli, API_PREFIX, INTAPI_SUBFIX)(rep(v), cnt));
        return *(VAL_TYPE *)&rrep;
    }

    [[gnu::always_inline]]
    static inline INT_VAL_TYPE shift_right(INT_VAL_TYPE v, int32_t cnt)
    {
        return (SIMDAPI(srli, API_PREFIX, INTAPI_SUBFIX)(v, cnt));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE shift_left(VAL_TYPE v, int32_t cnt)
    {
        auto rrep = (SIMDAPI(slli, API_PREFIX, INTAPI_SUBFIX)(rep(v), cnt));
        return *(VAL_TYPE *)&rrep;
    }

    [[gnu::always_inline]]
    static inline INT_VAL_TYPE shift_left(INT_VAL_TYPE v, int32_t cnt)
    {
        return (SIMDAPI(slli, API_PREFIX, INTAPI_SUBFIX)(v, cnt));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE shiftv_right(VAL_TYPE v, INT_VAL_TYPE cnt)
    {
        auto rrep = (SIMDAPI(srlv, API_PREFIX, INTAPI_SUBFIX)(rep(v), cnt));
        return *(VAL_TYPE *)&rrep;
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE shiftv_left(VAL_TYPE v, INT_VAL_TYPE cnt)
    {
        auto rrep = (SIMDAPI(sllv, API_PREFIX, INTAPI_SUBFIX)(rep(v), cnt));
        return *(VAL_TYPE *)&rrep;
    }


    [[gnu::always_inline]]
    static inline VAL_TYPE _and(VAL_TYPE v1, VAL_TYPE v2) {
        return SIMDAPI(and, API_PREFIX, API_SUBFIX)(v1, v2);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE _or(VAL_TYPE v1, VAL_TYPE v2) {
        return SIMDAPI(or, API_PREFIX, API_SUBFIX)(v1, v2);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE _xor(VAL_TYPE v1, VAL_TYPE v2) {
        return SIMDAPI(xor, API_PREFIX, API_SUBFIX)(v1, v2);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE and_not(VAL_TYPE v1, VAL_TYPE v2) {
        return SIMDAPI(andnot, API_PREFIX, API_SUBFIX)(v1, v2);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE _not(VAL_TYPE v) {
        auto int_rep = ~rep(v);
        return *(VAL_TYPE*)&int_rep;
    }

    [[gnu::always_inline]]
    inline CLS_NAME operator- () const { return CLS_NAME(_xor(v, SIGN_BIT_MASK)); }


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
        //<0 return -0(0x8000000000000000)
        //if_then_else(pref < 0, v1, v2) <=> if_then_else(sign_bit(pred), v1, v2)
        return _and(v, SIGN_BIT_MASK);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE rsign_bit(VAL_TYPE v) {
        //>0 return -0 (0x8000000000000000)
        //<0 return 0 (0x0000000000000000)
        //if_then_else(pref >= 0, v1, v2) <=> if_then_else(rsign_bit(pred), v1, v2)
        return and_not(v, SIGN_BIT_MASK);
    }

    [[gnu::always_inline]]
    static inline INT_VAL_TYPE base_bit(VAL_TYPE v)
    {
        auto base_rep = _and(v, EXPO_BIT_MASK);
        auto result_rep = shift_right(*(INT_VAL_TYPE*)&base_rep, MANT_BIT_CNT);
        return result_rep;
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE mantissa_bit(VAL_TYPE v)
    {
        return _and(v, MANT_BIT_MASK);
    }

    //======= compare result can be used as first param of if_then_else
    [[gnu::always_inline]]
    static inline VAL_TYPE less_than(VAL_TYPE first, VAL_TYPE second)
    {
        return SIMDAPI(cmp, API_PREFIX, API_SUBFIX)(first, second, _CMP_LT_OQ);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE great_than(VAL_TYPE first, VAL_TYPE second)
    {
        return SIMDAPI(cmp, API_PREFIX, API_SUBFIX)(first, second, _CMP_GT_OQ);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE less_equal(VAL_TYPE first, VAL_TYPE second)
    {
        return SIMDAPI(cmp, API_PREFIX, API_SUBFIX)(first, second, _CMP_LE_OQ);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE great_equal(VAL_TYPE first, VAL_TYPE second)
    {
        return SIMDAPI(cmp, API_PREFIX, API_SUBFIX)(first, second, _CMP_GE_OQ);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE equal(VAL_TYPE first, VAL_TYPE second)
    {
        return SIMDAPI(cmp, API_PREFIX, API_SUBFIX)(first, second, _CMP_EQ_OQ);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE not_equal(VAL_TYPE first, VAL_TYPE second)
    {
        return SIMDAPI(cmp, API_PREFIX, API_SUBFIX)(first, second, _CMP_NEQ_OQ);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE is_nan(VAL_TYPE v)
    {
        VAL_TYPE x0 = mul(CONST_0, v);
        return not(equal(CONST_0, x0));
    }
    //========== if_then_else======================
    [[gnu::always_inline]]
    static inline VAL_TYPE if_then_else(VAL_TYPE pred, VAL_TYPE first, VAL_TYPE second)
    {
        return SIMDAPI(blendv, API_PREFIX, API_SUBFIX)(second, first, pred);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE if_elif_else(VAL_TYPE ifm, VAL_TYPE first, VAL_TYPE elifm, VAL_TYPE second, VAL_TYPE third)
    {
        return if_then_else(ifm, first, if_then_else(elifm, second, third));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE if_elif_else(VAL_TYPE pred1, VAL_TYPE v1, VAL_TYPE pred2, VAL_TYPE v2, VAL_TYPE pred3, VAL_TYPE v3, VAL_TYPE v4)
    {
        return if_then_else(pred1, v1, if_then_else(pred2, v2, if_then_else(pred3, v3, v4)));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE pred_2_num(VAL_TYPE pred)
    {
        //true(<0) ==> 1.0
        //false(>0) ==> 0.0
        //return add(mul(-half, sign(pred)), half);
        return if_then_else(pred, CONST_1, CONST_0);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE num_2_pred(VAL_TYPE num)
    {
        //1.0 ==> true(-nan)
        //0.0 ==> false(0.0)
        //return add(mul(-half, sign(pred)), half);
        return if_then_else(equal(num, CONST_0), CONST_false, CONST_true);
    }

    //=====min/max abs=================
    [[gnu::always_inline]]
    static inline VAL_TYPE abs(VAL_TYPE v) {
        return _and(v, INV_SIGN_BIT_MASK);
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

    [[gnu::always_inline]]
    static inline VAL_TYPE min(VAL_TYPE v1, VAL_TYPE v2, VAL_TYPE v3) {
        return min(v1, min(v2, v3));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE max(VAL_TYPE v1, VAL_TYPE v2, VAL_TYPE v3) {
        return max(v1, max(v2, v3));
    }    
    
    // ==== round ===================
    [[gnu::always_inline]]
    static inline VAL_TYPE floor(VAL_TYPE v) {
        return SIMDAPI(floor, API_PREFIX, API_SUBFIX)(v);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE ceil(VAL_TYPE v) {
        return SIMDAPI(ceil, API_PREFIX, API_SUBFIX)(v);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE round(VAL_TYPE v) {
        return floor(add(v, CONST_0p5));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE round(VAL_TYPE v, VAL_TYPE def) {
        
        return if_then_else(is_nan(v), def, floor(add(v, CONST_0p5)));
    }

    //reduce
    [[gnu::always_inline]]
    static inline VAL_ELE_TYPE reduce_add(VAL_TYPE a){
#ifdef FLOATx8
    __m256 t1 = _mm256_hadd_ps(a,a);
    __m256 t2 = _mm256_hadd_ps(t1,t1);
    __m128 t3 = _mm256_extractf128_ps(t2,1);
    __m128 t4 = _mm_add_ss(_mm256_castps256_ps128(t2),t3);
    return _mm_cvtss_f32(t4);
#elif defined(DOUBLEx4)
    __m256d t1 = _mm256_hadd_pd(a,a);
    __m128d t2 = _mm256_extractf128_pd(t1,1);
    __m128d t3 = _mm_add_sd(_mm256_castpd256_pd128(t1),t2);
    return _mm_cvtsd_f64(t3);
#endif
    }

    //======sugar=================

    // ==== advance math ============
    [[gnu::always_inline]]
    static inline VAL_TYPE sqrt(VAL_TYPE v){
        return SIMDAPI(sqrt, API_PREFIX, API_SUBFIX)(v);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE recip_sqrt(VAL_TYPE v)
    {
        return div(CONST_1, sqrt(v));
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE recip_sqrt_fast(VAL_TYPE v) {
        static INT_VAL_TYPE recip_magic_number = HELP_FUNC::const_rep(RECIP_MAGIC_NUMBER);
        auto vhalf = mul(CONST_0p5, v);
        auto i = *(INT_VAL_TYPE*)&v;
        i =  recip_magic_number - shift_right(i, 1); //float:0x5F375A86
        v = *(VAL_TYPE*)&i;
        v = mul(v, CONST_1p5 - mul(mul(vhalf,v),v));
        return v;
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE recip(VAL_TYPE v){
        return div(CONST_1, v);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE recip_fast(VAL_TYPE v){
        auto v1 = recip_sqrt_fast(v);
        return mul(v1, v1);
    }

    static VAL_TYPE exp(VAL_TYPE x)
    {
        static const VAL_TYPE const_max_param = HELP_FUNC::const_val(std::log(SINGLE_MAX_VALUE));
        static const VAL_TYPE const_min_param = HELP_FUNC::const_val(std::log(SINGLE_MIN_NORM_VALUE));

        static const VAL_TYPE recip_ln2 = HELP_FUNC::const_val(1.44269504088896341);
        static const VAL_TYPE const_ln2 = HELP_FUNC::const_val(0.6931471805599453);

        static const VAL_TYPE recip_factorial_16 = HELP_FUNC::const_val(1.0/20922789888000.0);
        static const VAL_TYPE recip_factorial_15 = HELP_FUNC::const_val(1.0/1307674368000.0);
        static const VAL_TYPE recip_factorial_14 = HELP_FUNC::const_val(1.0/87178291200.0);
        static const VAL_TYPE recip_factorial_13 = HELP_FUNC::const_val(1.0/6227020800.0);
        static const VAL_TYPE recip_factorial_12 = HELP_FUNC::const_val(1.0/479001600.0);
        static const VAL_TYPE recip_factorial_11 = HELP_FUNC::const_val(1.0/39916800.0);
        static const VAL_TYPE recip_factorial_10 = HELP_FUNC::const_val(1.0/3628800.0);
        static const VAL_TYPE recip_factorial_9 = HELP_FUNC::const_val(1.0/362880.0);
        static const VAL_TYPE recip_factorial_8 = HELP_FUNC::const_val(1.0/40320.0);
        static const VAL_TYPE recip_factorial_7 = HELP_FUNC::const_val(1.0/5040.0);
        static const VAL_TYPE recip_factorial_6 = HELP_FUNC::const_val(1.0/720.0);
        static const VAL_TYPE recip_factorial_5 = HELP_FUNC::const_val(1.0/120.0);
        static const VAL_TYPE recip_factorial_4 = HELP_FUNC::const_val(1.0/24.0);
        static const VAL_TYPE recip_factorial_3 = HELP_FUNC::const_val(1.0/6.0);
        static const VAL_TYPE recip_factorial_2 = HELP_FUNC::const_val(1.0/2.0);
        static const VAL_TYPE recip_factorial_1 = CONST_1;
        static const VAL_TYPE recip_factorial_0 = CONST_1;

        x = clamp(x, const_min_param, const_max_param);

        /* express exp(x) as exp(g + n*log(2)) */
        auto fx = mul(x, recip_ln2);
        fx = floor(fx);
        x = fuse_nmul_add(fx, const_ln2, x);

        auto y = BOOST_PP_CAT(recip_factorial_, EXP_ITER_CNT);
#define DO_TAYLOR_4_EXP(z, n, data) y=fuse_mul_add(y, x, BOOST_PP_CAT(recip_factorial_, BOOST_PP_SUB(EXP_ITER_CNT, BOOST_PP_ADD(n,1))));
        BOOST_PP_REPEAT(EXP_ITER_CNT, DO_TAYLOR_4_EXP, ~);
#undef DO_TAYLOR_4_EXP
        /* build 2^n */

        INT_VAL_TYPE imm0 = HELP_FUNC::convert_2_int(fx);
        // another two AVX2 instructions
        imm0 = SIMDAPI(add, API_PREFIX, INTAPI_SUBFIX)(imm0, BIAS);
        imm0 = shift_left(imm0, MANT_BIT_CNT);
        auto pow2n = SIMDAPI(castsi256, API_PREFIX, API_SUBFIX)(imm0);
        y = mul(y, pow2n);
        return y;

    }

    static VAL_TYPE log(VAL_TYPE x)
    {
        static const VAL_TYPE const_ln2 = HELP_FUNC::const_val(0.6931471805599453);
        static const VAL_TYPE recip_odd_12 = HELP_FUNC::const_val(2.0/23.0);
        static const VAL_TYPE recip_odd_11 = HELP_FUNC::const_val(2.0/21.0);
        static const VAL_TYPE recip_odd_10 = HELP_FUNC::const_val(2.0/19.0);
        static const VAL_TYPE recip_odd_9 = HELP_FUNC::const_val(2.0/17.0);
        static const VAL_TYPE recip_odd_8 = HELP_FUNC::const_val(2.0/15.0);
        static const VAL_TYPE recip_odd_7 = HELP_FUNC::const_val(2.0/13.0);
        static const VAL_TYPE recip_odd_6 = HELP_FUNC::const_val(2.0/11.0);
        static const VAL_TYPE recip_odd_5 = HELP_FUNC::const_val(2.0/9.0);
        static const VAL_TYPE recip_odd_4 = HELP_FUNC::const_val(2.0/7.0);
        static const VAL_TYPE recip_odd_3 = HELP_FUNC::const_val(2.0/5.0);
        static const VAL_TYPE recip_odd_2 = HELP_FUNC::const_val(2.0/3.0);
        static const VAL_TYPE recip_odd_1 = HELP_FUNC::const_val(2.0);

        auto invalid_mask = less_than(x, CONST_0);

        auto e = SIMDAPI(sub, API_PREFIX, INTAPI_SUBFIX)(base_bit(x), BIAS);
        x = mantissa_bit(x);
        x = _or(x, CONST_1);

        auto xn1 = sub(x, CONST_1);
        auto xa1 = add(x, CONST_1);
        x = div(xn1, xa1);

        auto z = mul(x,x);

        auto y = BOOST_PP_CAT(recip_odd_, LOG_ITER_CNT);
#define DO_TAYLOR_4_LOG(x, n, data) y=fuse_mul_add(y, z, BOOST_PP_CAT(recip_odd_, BOOST_PP_SUB(LOG_ITER_CNT, BOOST_PP_ADD(n,1))));
        BOOST_PP_REPEAT(BOOST_PP_SUB(LOG_ITER_CNT,1), DO_TAYLOR_4_LOG, ~);
#undef DO_TAYLOR_4_LOG

        y = mul(y, x);
        auto k = HELP_FUNC::int_2_val(e);
        y = fuse_mul_add(k, const_ln2, y);

        y = _or(y, invalid_mask); // negative arg will be NAN
        return y;

    }

    [[gnu::always_inline]]
    static inline VAL_TYPE pow(VAL_TYPE a, VAL_TYPE b)
    {
        b = log(b);
        a = mul(a, b);
        return exp(a);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE sin(VAL_TYPE v)
    {
        return v;
//        return SIMDAPI(sin, API_PREFIX, API_SUBFIX)(v);
    }

    [[gnu::always_inline]]
    static inline VAL_TYPE cos(VAL_TYPE v)
    {
        return v;
//        return SIMDAPI(cos, API_PREFIX, API_SUBFIX)(v);
    }

    static VAL_TYPE pdf(VAL_TYPE v)
    {
        static const auto recip_sqrt_2pi = HELP_FUNC::const_val(0.3989422804014327);
        v = mul(v, v);
        v = mul(v, CONST_m0p5);
        return mul(recip_sqrt_2pi, exp(v));
    }

    static VAL_TYPE cdf(VAL_TYPE x)
    {
        static const auto a1 = HELP_FUNC::const_val(0.254829592);
        static const auto a2 = HELP_FUNC::const_val(-0.284496736);
        static const auto a3 = HELP_FUNC::const_val(1.421413741);
        static const auto a4 = HELP_FUNC::const_val(-1.453152027);
        static const auto a5 = HELP_FUNC::const_val(1.061405429);
        static const auto p = HELP_FUNC::const_val(0.3275911);
        static const auto s = HELP_FUNC::const_val(0.7071067811865475);  //1.0/sqrt(2.0)

        // Save the sign of x
        auto sig = sign(x);
        x = mul(abs(x), s);

        // A&S formula 7.1.26
        auto t = div(CONST_1, fuse_mul_add(p, x, CONST_1));
        auto z = exp(-mul(x, x));
        auto y = a5;
        y = fuse_mul_add(y, t, a4);
        y = fuse_mul_add(y, t, a3);
        y = fuse_mul_add(y, t, a2);
        y = fuse_mul_add(y, t, a1);
        y = mul(y, t);
        y = fuse_nmul_add(y, z, CONST_1);
        //double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

        return mul(CONST_0p5, fuse_mul_add(sig, y, CONST_1));
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

#undef SIMDAPI
#undef HELP_FUNC
