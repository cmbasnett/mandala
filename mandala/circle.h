#pragma once

//mandala
#include "types.h"

namespace mandala
{
	namespace details
	{
		template<typename T, typename Enable = void>
		struct circle_t;

		template<typename T>
		struct circle_t<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
		{
			typedef circle_t<T> type_t;
			typedef glm::detail::tvec2<T> origin_type_t;
			typedef T radius_type_t;
			typedef float32_t real_type_t;

			origin_type_t origin;
			radius_type_t radius = 0;

			real_type_t area() const
			{
				return glm::pi<real_type_t>() * std::pow(radius, 2);
			}

			real_type_t circumfrence() const
			{
				return glm::pi<real_type_t>() * radius * 2;
			}

			radius_type_t diameter() const
			{
				return radius * 2;
			}
		};
	};

	typedef details::circle_t<int8_t> circle_i8_t;
	typedef details::circle_t<int16_t> circle_i16_t;
	typedef details::circle_t<int32_t> circle_i32_t;
	typedef details::circle_t<int64_t> circle_i64_t;
	typedef details::circle_t<float32_t> circle_f32_t;
	typedef details::circle_t<float64_t> circle_f64_t;
	typedef circle_f32_t circle_t;
};