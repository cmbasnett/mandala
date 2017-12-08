#pragma once

//naga
#include "types.hpp"

namespace naga
{
    namespace details
    {
		template<typename Scalar, typename Enable = void>
        struct Sphere;

		template<typename Scalar>
		struct Sphere<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
			typedef Scalar ScalarType;
			typedef Sphere<ScalarType> Type;
			typedef glm::detail::tvec3<ScalarType> VectorType;

			VectorType origin;
			ScalarType radius = 0;

			Sphere() = default;
			Sphere(const VectorType& origin, ScalarType radius) :
                origin(origin),
                radius(radius)
            {
            }

			ScalarType area() const
            {
				return 4.0f * glm::pi<ScalarType>() * glm::pow2(radius);
            }

			ScalarType volume() const
            {
				return (4.0f / 3.0f) * glm::pi<ScalarType>() * glm::pow2(radius);
            }

			ScalarType circumfrence() const
            {
				return 2.0f * glm::pi<ScalarType>() * radius;
            }

			ScalarType diameter() const
            {
                return radius * 2;
            }
        };
    }

	typedef details::Sphere<f32> Sphere;
}
