#pragma once

//naga
#include "types.hpp"

namespace naga
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct Plane2;

        template<typename Scalar>
        struct Plane2<Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type>
        {
            typedef Scalar ScalarType;
            typedef Plane2<ScalarType> Type;
            typedef glm::tvec2<ScalarType> VectorType;

            VectorType normal;
            ScalarType distance = ScalarType(0);

            Plane2() = default;

            Plane2(const VectorType& normal, ScalarType distance) :
                normal(normal),
                distance(distance)
            {
            }

            Plane2(const VectorType& origin, const VectorType& normal) :
                normal(normal)
            {
                distance = glm::dot(normal, origin);
            }

            Plane2(const glm::tvec3<ScalarType>& vec3) :
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

            inline VectorType origin() const
            {
                return normal * distance;
            }

			Type operator-() const
            {
				return Type(-plane.normal, -plane.distance);
            }
        };
    }
    typedef details::Plane2<f32> Plane2;

    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct Plane3;

        template<typename Scalar>
        struct Plane3<Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type>
        {
            typedef Scalar ScalarType;
			typedef Plane3<ScalarType> Type;
            typedef glm::tvec3<ScalarType> VectorType;

            VectorType normal;
            ScalarType distance = 0;

            Plane3() = default;

            Plane3(const VectorType& normal, ScalarType distance) :
                normal(normal),
                distance(distance)
            {
            }

            Plane3(const VectorType& origin, const VectorType& normal) :
                normal(normal),
                distance(glm::dot(normal, origin))
            {
            }

            Plane3(const VectorType& v0, const VectorType& v1, const VectorType& v2)
            {
                const auto a = v1 - v0;
                const auto b = v2 - v0;

                normal = glm::normalize(glm::cross(a, b));
                distance = glm::dot(normal, v0);
            }

            Plane3(const glm::tvec4<ScalarType>& vec4) :
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

            inline VectorType origin() const
            {
                return normal * distance;
            }

			Type operator-() const
            {
				return Type(-plane.normal, -plane.distance);
            }
        };
    }

	typedef details::Plane3<f32> Plane3;
}
