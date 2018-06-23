#include "BasicOp.h"
namespace fast_math
{
    template <typename T>
    [[gnu::always_inline]]
    inline T fuse_mul_add(T a, T b, T c)
    {
        return T(T::fuse_mul_add(a.v, b.v, c.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T fuse_mul_sub(T a, T b, T c)
    {
        return T(T::fuse_mul_sub(a.v, b.v, c.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T fuse_nmul_add(T a, T b, T c)
    {
        return T(T::fuse_nmul_add(a.v, b.v, c.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T fuse_nmul_sub(T a, T b, T c)
    {
        return T(T::fuse_nmul_sub(a.v, b.v, c.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T abs(T v)
    {
        return T(T::abs(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T sign(T v)
    {
        return T(T::sign(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T min(T lhs, T rhs)
    {
        return T(T::min(lhs.v, rhs.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T max(T lhs, T rhs)
    {
        return T(T::max(lhs.v, rhs.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T clamp(T v, T a, T b)
    {
        return T(T::clamp(v.v, a.v, b.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T floor(T v)
    {
        return T(T::floor(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T ceil(T v)
    {
        return T(T::ceil(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T round(T v)
    {
        return T(T::round(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T sqrt(T v)
    {
        return T(T::sqrt(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T pow(T v, T e)
    {
        return T(T::pow(v.v, e.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T exp(T v)
    {
        return T(T::exp(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T log(T v)
    {
        return T(T::log(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T sin(T v)
    {
        return T(T::sin(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T cos(T v)
    {
        return T(T::cos(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T cdf(T v)
    {
        return T(T::cdf(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T pdf(T v)
    {
        return T(T::pdf(v.v));
    }

    template <typename T>
    [[gnu::always_inline]]
    inline T if_then_else(T pred, T s1, T s2)
    {
        return T(T::if_then_else(pred.v, s1.v, s2.v));
    }
}
