#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct circle;

        template<typename Scalar>
        struct circle<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
            typedef Scalar scalar_type;
            typedef glm::detail::tvec2<Scalar> origin_type;
            typedef circle<Scalar> type;

            origin_type origin;
            scalar_type radius = 0;

            f32 area() const
            {
                return glm::pi<f32>() * glm::pow2(radius);
            }

            f32 circumfrence() const
            {
                return glm::pi<f32>() * radius * 2;
            }

            scalar_type diameter() const
            {
                return radius * 2;
            }
        };
    }
    
    typedef details::circle<i8> circle_i8;
    typedef details::circle<i16> circle_i16;
    typedef details::circle<i32> circle_i32;
    typedef details::circle<i64> circle_i64;
    typedef details::circle<f32> circle_f32;
    typedef details::circle<f64> circle_f64;
    typedef circle_f32 circle;
}
