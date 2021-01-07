#pragma once

#include "dim.hpp"

namespace yama
{

template <size_t D, typename T>
class boxnt
{
public:
    static const size_t dimension = D;
    using dim_vector = typename dim<D>::template vector_t<T>;

    ////////////////////////////////////////////////////////
    // named constructors

    static boxnt zero()
    {
        boxnt ret;

        ret.min = dim_vector::zero();
        ret.max = dim_vector::zero();

        return ret;
    }

    static boxnt inverted()
    {
        boxnt ret;

        ret.min = dim_vector::uniform(std::numeric_limits<T>::max());
        ret.max = dim_vector::uniform(std::numeric_limits<T>::lowest());;

        return ret;
    }

    static boxnt min_max(const dim_vector& _min, const dim_vector& _max)
    {
        boxnt ret;

        ret.min = _min;
        ret.max = _max;

        return ret;
    }

    static boxnt pos_size(const dim_vector& pos, const dim_vector& size)
    {
        dim_vector end = pos + size;

        boxnt ret;

        ret.min = yama::min(pos, end);
        ret.max = yama::max(pos, end);

        return ret;
    }

    ////////////////////////////////////////////////////////
    // functions

    bool is_inside(const dim_vector& point) const
    {
        for(size_t i=0; i<dimension; ++i)
        {
            if (point.at(i) < min.at(i) || point.at(i) >= max.at(i))
            {
                return false;
            }
        }

        return true;
    }

    bool intersects(const boxnt& other) const
    {
        for(size_t i=0; i<dimension; ++i)
        {
            if (min.at(i) >= other.max.at(i))
                return false;

            if (max.at(i) <= other.min.at(i))
                return false;
        }

        return true;
    }

    dim_vector size() const
    {
        return max - min;
    }

    void add_point(const dim_vector& point)
    {
        min = yama::min(min, point);
        max = yama::max(max, point);
    }

    void add_point_max_complement(const dim_vector& point, const dim_vector& complement = dim_vector::uniform(1))
    {
        min = yama::min(min, point);
        max = yama::max(max, point + complement);
    }

    bool is_valid() const
    {
        for(size_t i=0; i<dimension; ++i)
        {
            if (max.at(i) <= min.at(i)) return false;
        }

        return true;
    }

    void merge(const boxnt& other)
    {
        min = yama::min(min, other.min);
        max = yama::max(max, other.max);
    }

    dim_vector center() const
    {
        return (max + min) / T(2);
    }

    boxnt operator+(const dim_vector& translation) const
    {
        return {min + translation, max + translation};
    }

    dim_vector min;
    dim_vector max;
};

template <size_t D, typename T>
bool operator==(const boxnt<D, T>& a, const boxnt<D, T>& b) {
    return a.min == b.min && a.max == b.max;
}

template <size_t D, typename T>
boxnt<D, T> intersection(const boxnt<D, T>& a, const boxnt<D, T>& b)
{
    boxnt<D, T> ret;

    YAMA_ASSERT_WARN(a.intersects(b), "can't find intersection of non-intersecting boxes");

    ret.min = max(a.min, b.min);
    ret.max = min(a.max, b.max);

    return ret;
}

} // namespace yama
