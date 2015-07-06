#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
	namespace details
	{
		template<typename Scalar, typename Enable = void>
		struct circle_t;

		template<typename Scalar>
		struct circle_t<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
		{
			typedef Scalar scalar_type;
			typedef glm::detail::tvec2<Scalar> origin_type;
			typedef circle_t<Scalar> type;

            origin_type origin;
			scalar_type radius = 0;

            float32_t area() const
			{
                return glm::pi<float32_t>() * std::pow(radius, 2);
			}

            float32_t circumfrence() const
			{
                return glm::pi<float32_t>() * radius * 2;
			}

			scalar_type diameter() const
			{
				return radius * 2;
			}
		};
	}
	
    typedef details::circle_t<int8_t> circle_i8_t;
	typedef details::circle_t<int16_t> circle_i16_t;
	typedef details::circle_t<int32_t> circle_i32_t;
	typedef details::circle_t<int64_t> circle_i64_t;
	typedef details::circle_t<float32_t> circle_f32_t;
	typedef details::circle_t<float64_t> circle_f64_t;
	typedef circle_f32_t circle_t;
}
