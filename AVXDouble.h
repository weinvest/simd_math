//
// Created by shugan.li on 18-5-22.
//

#ifndef ARAGOPROJECT_AVXDOUBLE_H
#define ARAGOPROJECT_AVXDOUBLE_H

#include <immintrin.h>
#include <avx2intrin.h>
namespace avx2 {
    using doublex4_t = __m256d;
    using int64x4_t = __m256i;
    using int64x2_t = __m128i;
    using int32x4_t = __m128i;

    //==== initializer =======
    [[gnu::always_inline]]
    inline doublex4_t doublex4_zero(void) {
        return _mm256_setzero_pd();
    }

    [[gnu::always_inline]]
    inline doublex4_t doublex4_const(double v) {
        return _mm256_set1_pd(v);
    }

    [[gnu::always_inline]]
    inline doublex4_t doublex4_const(int64_t v) {
        return _mm256_set1_pd(*(double*)&v);
    }

    [[gnu::always_inline]]
    inline doublex4_t doublex4_const(uint64_t v) {
        return _mm256_set1_pd(*(double*)&v);
    }

    [[gnu::always_inline]]
    inline doublex4_t to_doublex4(double v1, double v2, double v3, double v4)
    {
	return _mm256_set_pd(v1, v2, v3, v4);
    }    

    [[gnu::always_inline]]
    inline int64x4_t int64x4_const(int64_t v){
	return _mm256_set1_epi64x(v);
    }

    [[gnu::always_inline]]
    inline int64x4_t to_int64x4(int64_t v1, int64_t v2, int64_t  v3, int64_t v4){
	return _mm256_set_epi64x(v1, v2, v3, v4);
    }

    [[gnu::always_inline]]
    inline int32x4_t to_int32x4(int64x4_t v)
    {
	int32x4_t result;
	int32_t* presult = (int32_t*)&result;
	int32_t* pv = (int32_t*)&v;
        presult[0] = pv[0];
        presult[1] = pv[2];
        presult[2] = pv[4];
        presult[3] = pv[6];

	return result;
    }

    static const doublex4_t zero = doublex4_zero();
    static const doublex4_t half = doublex4_const(0.5);
    static const doublex4_t one = doublex4_const(1.0);
    static const doublex4_t truex4 = doublex4_const(0xFFFFFFFFFFFFFFFF);
    static const doublex4_t falsex4 = zero;

    static const int64x4_t izero = int64x4_const(0);
    static const int64x4_t ione = int64x4_const(1);
    static const int64x4_t itruex4 = int64x4_const(0xFFFFFFFFFFFFFFFF);
    static const int64x4_t ifalsex4 = izero;

    //converter
    [[gnu::always_inline]]
    inline doublex4_t to_doublex4(int64x4_t v)
    {
        return *(doublex4_t*)&v;
    }

    [[gnu::always_inline]]
    inline int64x4_t to_int64x4(doublex4_t v)
    {
        return *(int64x4_t*)&v;
    }
   
    //===== arithmetic operation ===========
    [[gnu::always_inline]]
    inline doublex4_t add(doublex4_t v1, doublex4_t v2) {
        return _mm256_add_pd(v1, v2);
    }

    [[gnu::always_inline]]
    inline doublex4_t sub(doublex4_t v1, doublex4_t v2) {
        return _mm256_sub_pd(v1, v2);
    }

    [[gnu::always_inline]]
    inline doublex4_t mul(doublex4_t v1, doublex4_t v2) {
        return _mm256_mul_pd(v1, v2);
    }

    [[gnu::always_inline]]
    inline doublex4_t div(doublex4_t v1, doublex4_t v2) {
        return _mm256_div_pd(v1, v2);
    }

    [[gnu::always_inline]]
    inline doublex4_t fuse_mul_add(doublex4_t a, doublex4_t b, doublex4_t c) {
        return _mm256_fmadd_pd(a, b, c);
    }

    //====== logic operation ============
    [[gnu::always_inline]]
    inline doublex4_t and_(doublex4_t v1, doublex4_t v2) {
        return _mm256_and_pd(v1, v2);
    }

    [[gnu::always_inline]]
    inline doublex4_t or_(doublex4_t v1, doublex4_t v2) {
        return _mm256_or_pd(v1, v2);
    }

    //[[gnu::always_inline]]
    //inline doublex4_t not_(doublex4_t v1, doublex4_t v2) {
    //    return _mm256_not_pd(v1, v2);
    //}

    [[gnu::always_inline]]
    inline doublex4_t xor_(doublex4_t v1, doublex4_t v2) {
        return _mm256_xor_pd(v1, v2);
    }

    [[gnu::always_inline]]
    inline doublex4_t and_not(doublex4_t v1, doublex4_t v2) {
        return _mm256_andnot_pd(v1, v2);
    }

    [[gnu::always_inline]]
    inline int64x4_t and_(int64x4_t v1, int64x4_t v2) {
        return _mm256_and_si256(v1, v2);
    }

    [[gnu::always_inline]]
    inline int64x4_t or_(int64x4_t v1, int64x4_t v2) {
        return _mm256_or_si256(v1, v2);
    }

    //[[gnu::always_inline]]
    //inline int64x4_t not_(int64x4_t v1, int64x4_t v2) {
    //    return _mm256_not_si256(v1, v2);
    //}

    [[gnu::always_inline]]
    inline int64x4_t xor_(int64x4_t v1, int64x4_t v2) {
        return _mm256_xor_si256(v1, v2);
    }

    [[gnu::always_inline]]
    inline int64x4_t and_not(int64x4_t v1, int64x4_t v2) {
        return _mm256_andnot_si256(v1, v2);
    }
    // ==== double struct ===================
    [[gnu::always_inline]]
    inline doublex4_t sign(doublex4_t v) {
        //>0 return 1
        //<0 return -1
        static const auto mm1 = doublex4_const(0x8000000000000000);
        static const auto mm2 = doublex4_const(0x3ff0000000000000);

        return _mm256_or_pd(_mm256_and_pd(v, mm1), mm2);
    }

    [[gnu::always_inline]]
    inline doublex4_t sign_bit(doublex4_t v) {
        //>0 return 0 (0x0000000000000000)
        //<0 return -0(0x1000000000000000)
        //if_then_else(pref < 0, v1, v2) <=> if_then_else(sign_bit(pred), v1, v2)
        static const auto mm1 = doublex4_const(0x8000000000000000);

        return _mm256_and_pd(v, mm1);
    }

    [[gnu::always_inline]]
    inline doublex4_t rsign_bit(doublex4_t v) {
        //>0 return -0 (0x1000000000000000)
        //<0 return 0 (0x0000000000000000)
        //if_then_else(pref >= 0, v1, v2) <=> if_then_else(rsign_bit(pred), v1, v2)
        static const auto mm1 = doublex4_const(0x8000000000000000);

        return and_not(v, mm1);
    }

    [[gnu::always_inline]]
    inline doublex4_t base_bit(doublex4_t v)
    {
        static const auto BASE_MASK = doublex4_const(0x7FF0000000000000LU);
        static const auto MANT_BITS = _mm_set_epi64x( 0, 52);
        auto base_rep = and_(v, BASE_MASK);
        auto result_rep = _mm256_srl_epi64(*(int64x4_t*)&base_rep, MANT_BITS) - 1023;
        return *(doublex4_t*)&result_rep;
    }

    [[gnu::always_inline]]
    inline doublex4_t mantissa_bit(doublex4_t v)
    {
        static const auto MANT_MASK = doublex4_const(0x000FFFFFFFFFFFFFLU);
        static const auto MANT_COMP_MASK = doublex4_const(0x0010000000000000LU);
        return or_(and_(v, MANT_MASK), MANT_COMP_MASK);
    }

    [[gnu::always_inline]]
    inline int64x4_t sign(int64x4_t v) {
        //>0 return 1
        //<0 return -1
        static const auto mm1 = int64x4_const(0x8000000000000000);
        static const auto mm2 = int64x4_const(0x3ff0000000000000);

        return or_(and_(v, mm1), mm2);
    }

    [[gnu::always_inline]]
    inline int64x4_t sign_bit(int64x4_t v) {
        //>0 return 0 (0x0000000000000000)
        //<0 return -0(0x1000000000000000)
        //if_then_else(pref < 0, v1, v2) <=> if_then_else(sign_bit(pred), v1, v2)
        static const auto mm1 = int64x4_const(0x8000000000000000);

        return and_(v, mm1);
    }

    [[gnu::always_inline]]
    inline int64x4_t rsign_bit(int64x4_t v) {
        //>0 return -0 (0x1000000000000000)
        //<0 return 0 (0x0000000000000000)
        //if_then_else(pref >= 0, v1, v2) <=> if_then_else(rsign_bit(pred), v1, v2)
        static const auto mm1 = int64x4_const(0x8000000000000000);

        return and_not(v, mm1);
    }

    [[gnu::always_inline]]
    inline int64x4_t base_bit(int64x4_t v)
    {
        static const auto BASE_MASK = int64x4_const(0x7FF0000000000000LU);
        static const auto MANT_BITS = _mm_set_epi64x( 0, 52);
        auto base_rep = and_(v, BASE_MASK);
        auto result_rep = _mm256_srl_epi64(base_rep, MANT_BITS) - 1023;
        return result_rep;
    }

    [[gnu::always_inline]]
    inline int64x4_t mantissa_bit(int64x4_t v)
    {
        static const auto MANT_MASK = int64x4_const(0x000FFFFFFFFFFFFFLU);
        static const auto MANT_COMP_MASK = int64x4_const(0x0010000000000000LU);
        return or_(and_(v, MANT_MASK), MANT_COMP_MASK);
    }


    //======= compare result can be used as first param of if_then_else
    [[gnu::always_inline]]
    inline doublex4_t less_than_zero(doublex4_t v)
    {
        return v;
    }

    [[gnu::always_inline]]
    inline doublex4_t great_equal_zero(doublex4_t v)
    {
        return -v;
    }

    [[gnu::always_inline]]
    inline doublex4_t less_than(doublex4_t first, doublex4_t second)
    {
        return _mm256_cmp_pd(first, second, _CMP_LT_OQ);
    }

    [[gnu::always_inline]]
    inline doublex4_t great_than(doublex4_t first, doublex4_t second)
    {
        return _mm256_cmp_pd(first, second, _CMP_GT_OQ);
    }

    [[gnu::always_inline]]
    inline doublex4_t less_equal(doublex4_t first, doublex4_t second)
    {
        return _mm256_cmp_pd(first, second, _CMP_LE_OQ);
    }

    [[gnu::always_inline]]
    inline doublex4_t great_equal(doublex4_t first, doublex4_t second)
    {
        return _mm256_cmp_pd(first, second, _CMP_GE_OQ);
    }

    [[gnu::always_inline]]
    inline doublex4_t equal(doublex4_t first, doublex4_t second)
    {
        return _mm256_cmp_pd(first, second, _CMP_EQ_OQ);
    }

    [[gnu::always_inline]]
    inline doublex4_t non_equal(doublex4_t first, doublex4_t second)
    {
        return _mm256_cmp_pd(first, second, _CMP_NEQ_OQ);
    }

    //========== if_then_else======================
    [[gnu::always_inline]]
    inline doublex4_t if_then_else(doublex4_t pred, doublex4_t first, doublex4_t second)
    {
        return _mm256_blendv_pd(second, first, pred);
    }

    [[gnu::always_inline]]
    inline doublex4_t pred_2_num(doublex4_t pred)
    {
        //true(<0) ==> 1.0
        //false(>0) ==> 0.0
        //return add(mul(-half, sign(pred)), half);
        return if_then_else(pred, one, zero);
    }

    [[gnu::always_inline]]
    inline doublex4_t num_2_pred(doublex4_t num)
    {
        //1.0 ==> true(-nan)
        //0.0 ==> false(0.0)
        //return add(mul(-half, sign(pred)), half);
        return if_then_else(equal(num, zero), falsex4, truex4);
    }

    [[gnu::always_inline]]
    inline int64x4_t shift_right(int64x4_t v, int32_t cnt)
    {
        int64x2_t shift_cnt = _mm_set_epi64x( 0, cnt);
        return _mm256_srl_epi64(v, shift_cnt);
    }

    [[gnu::always_inline]]
    inline int64x4_t shift_left(int64x4_t v, int32_t cnt)
    {
        int64x2_t shift_cnt = _mm_set_epi64x(0, cnt);
        return _mm256_sll_epi64(v, shift_cnt);
    }

    [[gnu::always_inline]]
    inline int64x4_t shiftv_right(int64x4_t v, int64x4_t shift_cnt)
    {
        return _mm256_srlv_epi64(v, shift_cnt);
    }

    [[gnu::always_inline]]
    inline int64x4_t shiftv_left(int64x4_t v, int64x4_t shift_cnt)
    {
        return _mm256_sllv_epi64(v, shift_cnt);
    }

    [[gnu::always_inline]]
    inline doublex4_t shift_right(doublex4_t v, int32_t cnt)
    {
        return to_doublex4(shift_right(to_int64x4(v), cnt));
    }

    [[gnu::always_inline]]
    inline doublex4_t shift_left(doublex4_t v, int32_t cnt)
    {
        return to_doublex4(shift_left(to_int64x4(v), cnt));
    }

    [[gnu::always_inline]]
    inline doublex4_t shiftv_right(doublex4_t v, int64x4_t shift_cnt)
    {
        return to_doublex4(shiftv_right(to_int64x4(v), shift_cnt));
    }

    [[gnu::always_inline]]
    inline doublex4_t shiftv_left(doublex4_t v, int64x4_t shift_cnt)
    {
        return to_doublex4(shiftv_left(to_int64x4(v), shift_cnt));
    }    
    
    //=====min/max abs=================
    [[gnu::always_inline]]
    inline doublex4_t abs(doublex4_t v) {
        static const auto mm1 = doublex4_const(0x7FFFFFFFFFFFFFFF);
        return _mm256_and_pd(v, mm1);
    }

    [[gnu::always_inline]]
    inline doublex4_t min(doublex4_t v1, doublex4_t v2) {
        return _mm256_min_pd(v1, v2);
    }

    [[gnu::always_inline]]
    inline doublex4_t max(doublex4_t v1, doublex4_t v2) {
        return _mm256_max_pd(v1, v2);
    }

    [[gnu::always_inline]]
    inline doublex4_t clamp(doublex4_t minV, doublex4_t maxV, doublex4_t v) {
        auto v1 = _mm256_max_pd(minV, v);
        auto v2 = _mm256_min_pd(maxV, v1);
        return v2;
    }


    [[gnu::always_inline]]
    inline int64x4_t abs(int64x4_t v) {
        //static const auto mm1 = int64x4_const(0x7FFFFFFFFFFFFFFF);
        //return and_(v, mm1);
        return v > 0 ? v : -v;
    }

    [[gnu::always_inline]]
    inline int64x4_t min(int64x4_t v1, int64x4_t v2) {
        //return _mm256_min_epi64(v1, v2);
        return v1 > v2 ? v2 : v1; //shift_right(v1 + v2 - abs(v1 - v2), 1);
    }

    [[gnu::always_inline]]
    inline int64x4_t max(int64x4_t v1, int64x4_t v2) {
        return v1 > v2 ? v1 : v2;
        //return _mm256_max_epi64(v1, v2);
    }

    [[gnu::always_inline]]
    inline int64x4_t clamp(int64x4_t minV, int64x4_t maxV, int64x4_t v) {
        auto v1 = max(minV, v);
        auto v2 = min(maxV, v1);
        return v2;
    }

    //======select from array ========
    [[gnu::always_inline]]
    inline doublex4_t select(double* base, int64x4_t idx)
    {
#if 1
        return _mm256_i64gather_pd(base, idx, 8); 
#else
	doublex4_t result;
	result[0] = base[iidx[0]];
	result[1] = base[iidx[2]];
	result[2] = base[iidx[4]];
	result[3] = base[iidx[6]];
	return result;
#endif
    }

    // ==== advance math ============
    [[gnu::always_inline]]
    inline doublex4_t sqrt(doublex4_t v){
        return _mm256_sqrt_pd(v);
    }

    [[gnu::always_inline]]
    inline doublex4_t recip_sqrt(doublex4_t v)
    {
        return div(one, sqrt(v));
    }

    [[gnu::always_inline]]
    inline doublex4_t recip_sqrt_fast(doublex4_t v) {
        auto vhalf = mul(half, v);
        int64x4_t i = *(int64x4_t*)&v;
        i = 0x5fe6ec85e7de30da - shift_right(i, 1L);;
        v = *(doublex4_t*)&i;
        v = mul(v, 1.5 - mul(mul(vhalf,v),v));
        return v;
    }

    [[gnu::always_inline]]
    inline doublex4_t recip(doublex4_t v){
        return div(one, sqrt(v));
    }

    [[gnu::always_inline]]
    inline doublex4_t recip_fast(doublex4_t v){
        auto v1 = recip_sqrt_fast(v);
        return mul(v1, v1);
    }

    doublex4_t exp(doublex4_t v)
    {
        #if 0
        static const auto PKAG_BITS = 4LU;
        static const auto STD_INDEX = 13;
        static const auto LN_2 = _mm256_set1_pd(1.0 / std::log(2.0));
        static const auto INDEX_O = 2*PKAG_BITS-1;
        static const auto MOD3_MASK = int64x4_const(0x3UL);
        static const auto MODF_MASK = int64x4_const(0xFUL);
        static const auto MAX_INDEX = int64x4_const(256);

        auto result = one;
        x = mul(x, LN_2);

        auto x_rep = *(int64x4_t*)&x;
        auto sign_bits = sign_bit(x_rep);
        auto ult_offset = shift_right(sign_bits, 55);
        auto mantissa_rep = mantissa_bit(x_rep);
        auto base = base_bit(x_rep);
        auto base_mod_4 = and_(base, MOD3_MASK);
        auto mantissa = shiftv_left(mantissa_rep, base_mod_4);
        mantissa = shift_right(mantissa, 4*(13 - STD_INDEX));

        auto beg_idx = shift_right(INDEX_O - base, 2);
        auto end_idx = 2 - beg_idx;
        for(int32_t i = 0; i <= STD_INDEX; ++i)
        {
            auto mantissa_modf = and_(mantissa, MODF_MASK);
            int64x4_t idx = shift_left(end_idx, 4) + mantissa_modf;
            idx = clamp(izero, MAX_INDEX, idx);
            idx += ult_offset;
            auto step_multi = select(POSITIVE_ULT, idx); 
            result = mul(result, step_multi);

            mantissa = shift_right(mantissa, 4);
            end_idx += ione;
        }

        return result
        #else
        return zero;
        #endif 
    }

    doublex4_t log(doublex4_t v)
    {

    }

    doublex4_t cdf(doublex4_t x)
    {
        static const auto a1 = doublex4_const(0.254829592);
        static const auto a2 = doublex4_const(-0.284496736);
        static const auto a3 = doublex4_const(1.421413741);
        static const auto a4 = doublex4_const(-1.453152027);
        static const auto a5 = doublex4_const(1.061405429);
        static const auto p = doublex4_const(0.3275911);
        static const auto s = doublex4_const(0.7071067811865475);  //1.0/sqrt(2.0)

        // Save the sign of x
        auto sig = sign(x);
        auto x1 = mul(mul(x, s), sig);

        // A&S formula 7.1.26
        auto t = div(one, fuse_mul_add(p, x, one));
        auto y = sub(one,
                 mul(mul(fuse_mul_add(fuse_mul_add(fuse_mul_add(fuse_mul_add(a5, t, a4), t, a3), t, a2), t, a1), t),
                     exp(-mul(x, x))));
        //double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

        return mul(half, fuse_mul_add(sig, y, one));
    }

}

#endif //ARAGOPROJECT_AVXDOUBLE_H

