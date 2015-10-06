#pragma once

//std
#include <array>

//mandala
#include "types.hpp"
#include "plane.hpp"
#include "range.hpp"

namespace mandala
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct aabb2;

        template<typename Scalar>
        struct aabb2<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type> : range_<glm::detail::tvec2<Scalar>>
        {
            typedef Scalar scalar_type;
            typedef aabb2<scalar_type> type;

            static const size_t CORNER_COUNT = 4;

            aabb2() = default;
            aabb2(const value_type& min, const value_type& max) :
                range_(min, max)
            {
            }

            inline scalar_type width() const
            {
                return max.x - min.x;
            }

            inline scalar_type height() const
            {
                return max.y - min.y;
            }

            inline value_type size() const
            {
                return max - min;
            }

            inline value_type center() const
            {
                return min + (size() / static_cast<scalar_type>(2));
            }

            inline value_type extents() const
            {
                return (max - min) / scalar_type(2);
            }

            std::array<value_type, CORNER_COUNT> get_corners() const
            {
                return { min,
                         value_type(min.x, max.y),
                         value_type(max.x, min.y),
                         max };
            }

            type operator-(const value_type& rhs) const
            {
                return type(min - rhs, max - rhs);
            }

            type& operator-=(const value_type& rhs)
            {
                *this = *this - rhs;

                return *this;
            }

            type operator+(const value_type& rhs) const
            {
                return type(min + rhs, max + rhs);
            }

            type& operator+=(const value_type& rhs)
            {
                *this = *this + rhs;

                return *this;
            }

            static type join(const type& lhs, const type& rhs)
            {
                return type(glm::min(lhs.min, rhs.min), glm::max(lhs.max, rhs.max));
            }

            type& join(const type& rhs)
            {
                *this = join(*this, rhs);

                return *this;
            }

            template<typename Scalar>
            bool operator==(const aabb2<Scalar>& rhs) const
            {
                return min == rhs.min && max == rhs.max;
            }

            template<typename Scalar>
            bool operator!=(const aabb2<Scalar>& rhs) const
            {
                return min != rhs.min || max != rhs.max;
            }

            template<typename Scalar>
            type& operator=(const aabb2<Scalar>& rhs) const
            {
                min = static_cast<vector_type>(rhs.min);
                max = static_cast<vector_type>(rhs.max);

                return *this;
            }

            template<typename T>
            operator aabb2<T>()
            {
                return aabb2<T>(static_cast<aabb2<T>::value_type>(min), static_cast<aabb2<T>::value_type>(max));
            }

            type operator<<(const type& rhs) const
            {
                return join(*this, rhs);
            }

            type& operator<<=(const type& rhs)
            {
                return this->join(rhs);
            }
        };

        template<typename T, typename Enable = void>
        struct aabb3;

        template<typename Scalar>
        struct aabb3<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type> : range_<glm::detail::tvec3<Scalar>>
        {
            typedef Scalar scalar_type;
            typedef aabb3<scalar_type> type;
            typedef plane3<f32> plane_type;

            static const size_t CORNER_COUNT = 8;
            static const size_t PLANE_COUNT = 6;

            aabb3() = default;
            aabb3(const value_type& min, const value_type& max) :
                range_(min, max)
            {
            }

            inline scalar_type width() const
            {
                return max.x - min.x;
            }

            inline scalar_type height() const
            {
                return max.y - min.y;
            }

            inline scalar_type depth() const
            {
                return max.z - min.z;
            }

            value_type size() const
            {
                return max - min;
            }

            value_type center() const
            {
                return min + ((max - min) / scalar_type(2));
            }

            value_type extents() const
            {
                return (max - min) / scalar_type(2);
            }

            std::array<plane_type, PLANE_COUNT> get_planes() const
            {
                return { { min, VEC3_RIGHT },
                         { max, VEC3_LEFT },
                         { min, VEC3_DOWN },
                         { max, VEC3_UP },
                         { min, VEC3_BACKWARD },
                         { max, VEC3_FORWARD } };
            }

            std::array<value_type, CORNER_COUNT> get_corners() const
            {
                return{ min,
                        value_type(min.x, min.y, max.z),
                        value_type(min.x, max.y, min.z),
                        value_type(min.x, max.y, max.z),
                        value_type(max.x, min.y, min.z),
                        value_type(max.x, min.y, max.z),
                        value_type(max.x, max.y, min.z),
                        max };
            }

            type operator-(const value_type& rhs) const
            {
                return type(min - rhs, max - rhs);
            }

            type& operator-=(const value_type& rhs)
            {
                *this = *this - rhs;

                return *this;
            }

            type operator+(const value_type& rhs) const
            {
                return type(min + rhs, max + rhs);
            }

            type& operator+=(const value_type& rhs)
            {
                *this = *this + rhs;

                return *this;
            }

            static type join(const type& lhs, const value_type& rhs)
            {
                return type(glm::min(lhs.min, rhs), glm::max(lhs.max, rhs));
            }

            static type join(const type& lhs, const type& rhs)
            {
                return type(glm::min(lhs.min, rhs.min), glm::max(lhs.max, rhs.max));
            };

            type& join(const value_type& rhs)
            {
                *this = join(*this, rhs);

                return *this;
            }

            type& join(const type& rhs)
            {
                *this = join(*this, rhs);

                return *this;
            }

            template<typename U>
            bool operator==(const aabb3<U>& rhs) const
            {
                return min == rhs.min && max == rhs.max;
            }

            template<typename U>
            bool operator!=(const aabb3<U>& rhs) const
            {
                return min != rhs.min || max != rhs.max;
            }

            template<typename U>
            type& operator=(const aabb3<U>& rhs)
            {
                min = static_cast<value_type>(rhs.min);
                max = static_cast<value_type>(rhs.max);
                return *this;
            }

            template<typename PointScalar, size_t N>
            type& operator=(const std::array<glm::detail::tvec3<PointScalar>, N>& points)
            {
                min = value_type(std::numeric_limits<scalar_type>::max());
                max = value_type(-std::numeric_limits<scalar_type>::max());

                for (const auto& point : points)
                {
                    *this <<= point;
                }

                return *this;
            }

            type operator<<(const type& rhs) const
            {
                return join(*this, rhs);
            }

            type operator<<(const value_type& rhs) const
            {
                return join(*this, rhs);
            }

            type& operator<<=(const value_type& rhs)
            {
                return this->join(rhs);
            }

            type& operator<<=(const type& rhs)
            {
                return this->join(rhs);
            }

            type operator<<(const glm::detail::tmat4x4<scalar_type>& rhs) const
            {
                type aabb;
                aabb.min = value_type(std::numeric_limits<scalar_type>::max());
                aabb.max = value_type(-std::numeric_limits<scalar_type>::max());

                for (const auto& corner : get_corners())
                {
                    auto corner_transformed = glm::detail::tvec4<scalar_type>(corner, scalar_type(0));
                    corner_transformed = rhs * corner_transformed;

                    aabb <<= value_type(corner_transformed.x, corner_transformed.y, corner_transformed.z);
                }

                return aabb;
            }

            type& operator<<=(const glm::detail::tmat4x4<scalar_type>& rhs)
            {
                *this = *this << rhs;

                return *this;
            }
        };
    }

    typedef details::aabb2<i8>	aabb2_i8;
    typedef details::aabb2<i16> aabb2_i16;
    typedef details::aabb2<i32> aabb2_i32;
    typedef details::aabb2<i64> aabb2_i64;
    typedef details::aabb2<f32> aabb2_f32;
    typedef details::aabb2<f64> aabb2_f36;
    typedef aabb2_f32           aabb2;

    typedef details::aabb3<i8>  aabb3_i8;
    typedef details::aabb3<i16> aabb3_i16;
    typedef details::aabb3<i32> aabb3_i32;
    typedef details::aabb3<i64> aabb3_i64;
    typedef details::aabb3<f32> aabb3_f32;
    typedef details::aabb3<f64> aabb3_f36;
    typedef aabb3_f32  			aabb3;
}
