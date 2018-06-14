namespace fast_math
{
    template <typename T>
    inline T operator+ (T lhs, T rhs)
    {
        return T::add(lhs, rhs);
    }

    template <typename T>
    inline T operator- (T lhs, T rhs)
    {
        return T::sub(lhs, rhs);
    }

    template <typename T>
    inline T operator* (T lhs, T rhs)
    {
        return T::mul(lhs, rhs);
    }

    template <typename T>
    inline T operator/ (T lhs, T rhs)
    {
        return T::div(lhs, rhs);
    }

    template <typename T>
    inline T operator< (T lhs, T rhs)
    {
        return T::less_than(lhs, rhs);
    }

    template <typename T>
    inline T operator<= (T lhs, T rhs)
    {
        return T::less_equal(lhs, rhs);
    }

    template <typename T>
    inline T operator> (T lhs, T rhs)
    {
        return T::great_than(lhs, rhs);
    }

    template <typename T>
    inline T operator>= (T lhs, T rhs)
    {
        return T::great_equal(lhs, rhs);
    }

    template <typename T>
    inline T operator== (T lhs, T rhs)
    {
        return T::equal(lhs, rhs);
    }

    template <typename T>
    inline T operator!= (T lhs, T rhs)
    {
        return T::not_equal(lhs, rhs);
    }

    template <typename T>
    inline T operator& (T lhs, T rhs)
    {
        return T::_and(lhs, rhs);
    }

    template <typename T>
    inline T operator| (T lhs, T rhs)
    {
        return T::_or(lhs, rhs);
    }

    template <typename T>
    inline T operator^ (T lhs, T rhs)
    {
        return T::_xor(lhs, rhs);
    }

    template <typename T>
    inline T operator~ (T v)
    {
        return T::_not(v);
    }

    template <typename T>
    inline T operator>> (T v, int32_t cnt)
    {
        return T::shift_right(v, cnt);
    }

    template <typename T>
    inline T operator<< (T v, int32_t cnt)
    {
        return T::shift_left(v, cnt);
    }

    typename <typename T>
    inline operator>> (T v, T cnt)
    {
        return T::shiftv_right(v, cnt);
    }

    template <typename T>
    inline T operator<< (T v, T cnt)
    {
        return T::shiftv_left(v, cnt);
    }
}