namespace fast_math
{
    template <typename T>
    inline T operator+ (T lhs, T rhs)
    {
        return T(T::add(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator- (T lhs, T rhs)
    {
        return T(T::sub(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator* (T lhs, T rhs)
    {
        return T(T::mul(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator/ (T lhs, T rhs)
    {
        return T(T::div(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator< (T lhs, T rhs)
    {
        return T(T::less_than(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator<= (T lhs, T rhs)
    {
        return T(T::less_equal(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator> (T lhs, T rhs)
    {
        return T(T::great_than(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator>= (T lhs, T rhs)
    {
        return T(T::great_equal(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator== (T lhs, T rhs)
    {
        return T(T::equal(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator!= (T lhs, T rhs)
    {
        return T(T::not_equal(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator& (T lhs, T rhs)
    {
        return T(T::_and(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator| (T lhs, T rhs)
    {
        return T(T::_or(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator^ (T lhs, T rhs)
    {
        return T(T::_xor(lhs.v, rhs.v));
    }

    template <typename T>
    inline T operator~ (T v)
    {
        return T(T::_not(v.v));
    }

    template <typename T>
    inline T operator>> (T v, int32_t cnt)
    {
        return T(T::shift_right(v.v, cnt));
    }

//    template <typename T>
//    inline T operator<< (T v, int32_t cnt)
//    {
//        return T::shift_left(v, cnt);
//    }
//
//    template <typename T>
//    inline operator>> (T v, T cnt)
//    {
//        return T::shiftv_right(v, cnt);
//    }

//    template <typename T>
//    inline T operator<< (T v, T cnt)
//    {
//        return T(T::shiftv_left(v.v, cnt));
//    }
}