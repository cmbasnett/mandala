#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
    namespace details
    {
        template<typename T, typename Enable = void>
        struct sphere;

        template<typename T>
        struct sphere<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
        {
            typedef sphere<T> type;
            typedef glm::detail::tvec3<T> origin_type;
            typedef f32 radius_type;

            origin_type origin;
            radius_type radius = 0;

            sphere() = default;
            sphere(const origin_type& origin, radius_type radius) :
                origin(origin),
                radius(radius)
            {
            }

            radius_type area() const
            {
                return 4.0f * glm::pi<radius_type>() * glm::pow2(radius);
            }

            radius_type volume() const
            {
                return (4.0f / 3.0f) * glm::pi<radius_type>() * glm::pow2(radius);
            }

            radius_type circumfrence() const
            {
                return 2.0f * glm::pi<radius_type>() * radius;
            }

            radius_type diameter() const
            {
                return radius * 2;
            }
        };
    }

    typedef details::sphere<i8> sphere_i8;
    typedef details::sphere<i16> sphere_i16;
    typedef details::sphere<i32> sphere_i32;
    typedef details::sphere<i64> sphere_i64;
    typedef details::sphere<f32> sphere_f32;
    typedef details::sphere<f64> sphere_f64;
    typedef sphere_f32 sphere;
}
