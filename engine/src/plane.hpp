#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct plane2_t;

        template<typename Scalar>
        struct plane2_t<Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type>
        {
            typedef Scalar scalar_type;
            typedef plane2_t<scalar_type> type;
            typedef glm::detail::tvec2<scalar_type> vector_type;

            vector_type normal;
            scalar_type distance = scalar_type(0);

            plane2_t() = default;

            plane2_t(const vector_type& normal, scalar_type distance) :
                normal(normal),
                distance(distance)
            {
            }

            plane2_t(const vector_type& origin, const vector_type& normal) :
                normal(normal)
            {
                distance = glm::dot(normal, origin);
            }

            plane2_t(const glm::detail::tvec3<scalar_type>& vec3) :
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

    typedef details::plane2_t<float32_t> plane2_f32_t;
    typedef details::plane2_t<float64_t> plane2_f64_t;
    typedef plane2_f32_t plane2_t;

    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct plane3_t;

        template<typename Scalar>
        struct plane3_t<Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type>
        {
            typedef Scalar scalar_type;
            typedef plane3_t<scalar_type> type;
            typedef glm::detail::tvec3<scalar_type> vector_type;

            vector_type normal;
            scalar_type distance = 0;

            plane3_t() = default;

            plane3_t(const vector_type& normal, scalar_type distance) :
                normal(normal),
                distance(distance)
            {
            }

            plane3_t(const vector_type& origin, const vector_type& normal) :
                normal(normal),
                distance(glm::dot(normal, origin))
            {
            }

            plane3_t(const vector_type& v0, const vector_type& v1, const vector_type& v2)
            {
                const auto a = v1 - v0;
                const auto b = v2 - v0;

                normal = glm::normalize(glm::cross(a, b));
                distance = glm::dot(normal, v0);
            }

            plane3_t(const glm::detail::tvec4<scalar_type>& vec4) :
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

    typedef details::plane3_t<float32_t> plane3_f32_t;
    typedef details::plane3_t<float64_t> plane3_f64_t;
    typedef plane3_f32_t plane3_t;
}
