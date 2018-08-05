#pragma once

//naga
#include "types.hpp"

namespace naga
{
    namespace details
    {
        template<typename Scalar, typename Enable = void>
        struct Circle;

        template<typename Scalar>
		struct Circle<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
        {
            typedef Scalar ScalarType;
			typedef glm::tvec2<ScalarType> VectorType;
			typedef Circle<Scalar> type;

			VectorType origin;
			ScalarType radius = 0;

            f32 area() const
            {
                return glm::pi<f32>() * glm::pow2(radius);
            }

            f32 circumfrence() const
            {
                return glm::pi<f32>() * radius * 2;
            }

			ScalarType diameter() const
            {
                return radius * 2;
            }
        };
    }
    
	typedef details::Circle<f32> Circle;
}
