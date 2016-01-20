#pragma once

//std
#include <iostream>

//naga
#include "types.hpp"
#include "aabb.hpp"

namespace naga
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct padding;

        template<typename Scalar>
        struct padding<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
            typedef Scalar scalar_type;
            typedef padding<scalar_type> type;
            typedef glm::detail::tvec2<scalar_type> size_type;

            scalar_type bottom = 0;
            scalar_type left = 0;
            scalar_type top = 0;
            scalar_type right = 0;

            padding() = default;
            padding(scalar_type bottom, scalar_type left, scalar_type top, scalar_type right) :
                bottom(bottom),
                left(left),
                top(top),
                right(right)
            {
            }

            padding(scalar_type all) :
                padding(all, all, all, all)
            {
            }

            scalar_type vertical() const
            {
                return bottom + top;
            }

            scalar_type horizontal() const
            {
                return left + right;
            }

            size_type size() const
            {
                return size_type(horizontal(), vertical());
            }

            template<typename U>
            type operator+(const padding<U>& rhs) const
            {
                auto sum = *this;
                sum += rhs;
                return sum;
            }

            template<typename U>
            type& operator+=(const padding<U>& rhs)
            {
                bottom += rhs.bottom;
                left += rhs.left;
                right += rhs.right;
                top += rhs.top;

                return *this;
            }

            template<typename U>
            type operator-(const padding<U>& rhs) const
            {
                auto sum = *this;
                sum -= rhs;
                return sum;
            }

            template<typename U>
            type& operator-=(const padding<U>& rhs)
            {
                bottom -= rhs.bottom;
                left -= rhs.left;
                right -= rhs.right;
                top -= rhs.top;

                return *this;
            }

            template<typename U>
            bool operator==(const padding<U>& rhs) const
            {
                return bottom == rhs.bottom && left == rhs.left && right == rhs.right && top == rhs.top;
            }

            template<typename U>
            bool operator!=(const padding<U>& rhs) const
            {
                return bottom != rhs.bottom || left != rhs.left || right != rhs.right || top != rhs.top;
            }

            template<typename U>
            operator glm::detail::tvec4<U>() const
            {
                return glm::detail::tvec4<U>(static_cast<U>(bottom), static_cast<U>(left), static_cast<U>(right), static_cast<U>(top));
            }
        };

        template<typename T>
        std::ostream& operator<<(std::ostream& ostream, const padding<T>& p)
        {
            return ostream << "(" << p.bottom << "," << p.left << "," << p.top << "," << p.right << ")";
        }
    }

    typedef details::padding<u8>	padding_u8;
    typedef details::padding<u16>   padding_u16;
    typedef details::padding<u32>   padding_u32;
    typedef details::padding<u64>   padding_u64;
    typedef details::padding<f32>   padding_f32;
    typedef details::padding<f64>   padding_f64;
    typedef padding_f32				padding;

    template<typename T, typename U>
    details::aabb2<T> operator-(const details::aabb2<T>& aabb, const details::padding<U>& padding)
    {
        return details::aabb2<T>(glm::detail::tvec2<T>(aabb.min.x + padding.left, aabb.min.y + padding.bottom), glm::detail::tvec2<T>(aabb.max.x - padding.right, aabb.max.y - padding.top));
    }

    template<typename T, typename U>
    void operator-=(details::aabb2<T>& aabb, const details::padding<U>& padding)
    {
        //TODO: verify correctness
        aabb.min.x += std::min(padding.left, aabb.width());
        aabb.min.y += std::min(padding.bottom, aabb.height());
        aabb.max.x -= std::min(padding.right, aabb.width());
        aabb.max.y -= std::min(padding.top, aabb.height());
    }

    template<typename T, typename U>
    details::aabb2<T> operator+(const details::aabb2<T>& aabb, const details::padding<U>& padding)
    {
        return details::aabb2<T>(glm::detail::tvec2<T>(aabb.min.x - padding.left, aabb.min.y - padding.bottom), glm::detail::tvec2<T>(aabb.max.x + padding.right, aabb.max.y + padding.top));
    }

    template<typename T, typename U>
    void operator+=(details::aabb2<T>& aabb, const details::padding<U>& padding)
    {
        aabb.min.x -= padding.left;
        aabb.min.y -= padding.bottom;
        aabb.max.x += padding.right;
        aabb.max.y += padding.top;
    }
}
