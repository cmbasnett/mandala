#pragma once

//naga
#include "types.hpp"
#include "aabb.hpp"

namespace naga
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct rectangle;

        template<typename Scalar>
        struct rectangle<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
            typedef Scalar scalar_type;
            typedef glm::detail::tvec2<scalar_type> vector_type;
            typedef rectangle<scalar_type> type;

            scalar_type x = 0;
            scalar_type y = 0;
            scalar_type width = 0;
            scalar_type height = 0;

            rectangle() = default;

            template<typename Scalar>
            rectangle(Scalar x, Scalar y, Scalar width, Scalar height) :
                x(static_cast<scalar_type>(x)),
                y(static_cast<scalar_type>(y)),
                width(static_cast<scalar_type>(width)),
                height(static_cast<scalar_type>(height))
            {
            }
            
            template<typename Scalar>
            rectangle(const glm::detail::tvec4<Scalar>& v) :
                            x(static_cast<scalar_type>(v.x)),
                            y(static_cast<scalar_type>(v.y)),
                            width(static_cast<scalar_type>(v.z)),
                            height(static_cast<scalar_type>(v.w))
            {
            }

            template<typename Scalar>
            rectangle(const aabb2<Scalar>& aabb)
            {
                x = static_cast<scalar_type>(aabb.min.x);
                y = static_cast<scalar_type>(aabb.min.y);
                width = static_cast<scalar_type>(aabb.width());
                height = static_cast<scalar_type>(aabb.height());
            }

            scalar_type area() const
            {
                return width * height;
            }

            vector_type center() const
            {
                return vector_type(x + (width / 2), y + (height / 2));
            }

            vector_type size() const
            {
                return vector_type(width, height);
            }

            vector_type min() const
            {
                return vector_type(x, y);
            }

            vector_type max() const
            {
                return vector_type(x + width, y + height);
            }

            template<typename Scalar>
            operator glm::detail::tvec4<Scalar>() const
            {
                return glm::detail::tvec4<Scalar>(static_cast<Scalar>(x), static_cast<Scalar>(y), static_cast<Scalar>(width), static_cast<Scalar>(height));
            }
        };
    }

    typedef details::rectangle<i8> rectangle_i8;
    typedef details::rectangle<i16> rectangle_i16;
    typedef details::rectangle<u16> rectangle_u16;
    typedef details::rectangle<u8> rectangle_u8;
    typedef details::rectangle<i16> rectangle_i16;
    typedef details::rectangle<u16> rectangle_u16;
    typedef details::rectangle<i32> rectangle_i32;
    typedef details::rectangle<u32> rectangle_u32;
    typedef details::rectangle<i64> rectangle_i64;
    typedef details::rectangle<u64> rectangle_u64;
    typedef details::rectangle<f32> rectangle_f32;
    typedef details::rectangle<f64> rectangle_f64;
    typedef rectangle_f32 rectangle;
}
