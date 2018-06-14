#include "BasicOp.h"
namespace fast_math
{
    template <typename T>
    inline T fuse_mul_add(T a, T b, T c)
    {
        return T::fuse_mul_add(a, b, c);
    }

    template <typename T>
    inline T fuse_mul_sub(T a, T b, T c)
    {
        return T::fuse_mul_sub(a, b, c);
    }

    template <typename T>
    inline T fuse_nmul_add(T a, T b, T c)
    {
        return T::fuse_nmul_add(a, b, c);
    }

    template <typename T>
    inline T fuse_nmul_sub(T a, T b, T c)
    {
        return T::fuse_nmul_sub(a, b, c);
    }

    template <typename T>
    inline T abs(T v)
    {
        return T::abs(v);
    }

    template <typename T>
    inline T sign(T v)
    {
        return T::sign(v);
    }

    template <typename T>
    inline T min(T lhs, T rhs)
    {
        return T::min(lhs, rhs);
    }

    template <typename T>
    inline T max(T lhs, T rhs)
    {
        return T::max(lhs, rhs);
    }

    template <typename T>
    inline T clamp(T v, T a, T b)
    {
        return T::clamp(v, a, b);
    }

    template <typename T>
    inline T floor(T v)
    {
        return T::floor(v);
    }

    template <typename T>
    inline T ceil(T v)
    {
        return T::ceil(v);
    }

    template <typename T>
    inline T round(T v)
    {
        return T::round(v);
    }

    template <typename T>
    inline T sqrt(T v)
    {
        return T::sqrt(v);
    }

    template <typename T>
    inline T pow(T v, T e)
    {
        return T::pow(v, e);
    }

    template <typename T>
    inline T exp(T v)
    {
        return T::exp(v);
    }

    template <typename T>
    inline T log(T v)
    {
        return T::log(v);
    }

    template <typename T>
    inline T sin(T v)
    {
        return T::sin(v);
    }

    template <typename T>
    inline T cos(T v)
    {
        return T::cos(v);
    }

    template <typename T>
    inline T cdf(T v)
    {
        return T::cdf(v);
    }

    template <typename T>
    inline T pdf(T v)
    {
        return T::pdf(v);
    }

    template <typename T>
    inline T if_then_else(T pred, T s1, T s2)
    {
        return T::if_then_else(pred, s1, s2);
    }
}