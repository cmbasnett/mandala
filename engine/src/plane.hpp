#pragma once

//naga
#include "types.hpp"

namespace naga
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct plane2;

        template<typename Scalar>
        struct plane2<Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type>
        {
            typedef Scalar scalar_type;
            typedef plane2<scalar_type> type;
            typedef glm::detail::tvec2<scalar_type> vector_type;

            vector_type normal;
            scalar_type distance = scalar_type(0);

            plane2() = default;

            plane2(const vector_type& normal, scalar_type distance) :
                normal(normal),
                distance(distance)
            {
            }

            plane2(const vector_type& origin, const vector_type& normal) :
                normal(normal)
            {
                distance = glm::dot(normal, origin);
            }

            plane2(const glm::detail::tvec3<scalar_type>& vec3) :
                normal(glm::swizzle<glm::X, glm::Y>(vec3)),
                distance(vec3.z)
            {
            }

            void normalize()
            {
                const auto length = glm::length(normal);
                normal /= length;
                distance /= length;
            }

            inline vector_type origin() const
            {
                return normal * distance;
            }

            type operator-() const
            {
                return type(-plane.normal, -plane.distance);
            }
        };
    }

    typedef details::plane2<f32> plane2_f32;
    typedef details::plane2<f64> plane2_f64;
    typedef plane2_f32 plane2;

    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct plane3;

        template<typename Scalar>
        struct plane3<Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type>
        {
            typedef Scalar scalar_type;
            typedef plane3<scalar_type> type;
            typedef glm::detail::tvec3<scalar_type> vector_type;

            vector_type normal;
            scalar_type distance = 0;

            plane3() = default;

            plane3(const vector_type& normal, scalar_type distance) :
                normal(normal),
                distance(distance)
            {
            }

            plane3(const vector_type& origin, const vector_type& normal) :
                normal(normal),
                distance(glm::dot(normal, origin))
            {
            }

            plane3(const vector_type& v0, const vector_type& v1, const vector_type& v2)
            {
                const auto a = v1 - v0;
                const auto b = v2 - v0;

                normal = glm::normalize(glm::cross(a, b));
                distance = glm::dot(normal, v0);
            }

            plane3(const glm::detail::tvec4<scalar_type>& vec4) :
                normal(glm::swizzle<glm::X, glm::Y, glm::Z>(vec4)),
                distance(vec4.z)
            {
            }

            void normalize()
            {
                const auto length = glm::length(normal);
                normal /= length;
                distance /= length;
            }

            inline vector_type origin() const
            {
                return normal * distance;
            }

            type operator-() const
            {
                return type(-plane.normal, -plane.distance);
            }
        };
    }

    typedef details::plane3<f32> plane3_f32;
    typedef details::plane3<f64> plane3_f64;
    typedef plane3_f32 plane3;
}
