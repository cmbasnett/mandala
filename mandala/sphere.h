#pragma once

//mandala
#include "types.h"

namespace mandala
{
	namespace details
	{
		template<typename T>
		struct __sphere_t__
		{
			typedef __sphere_t__<T> type;
			typedef glm::detail::tvec3<T> origin_type;
			typedef float32_t radius_type;

			origin_type origin;
			radius_type radius = 0;

			radius_type area() const
			{
				return 4.0f * glm::pi() * glm::pow2(radius);			
			}

			radius_type volume() const
			{
				return (4.0f / 3.0f) * glm::pi() * glm::pow2(radius);
			}

			radius_type circumfrence() const
			{
				return 2.0f * glm::pi() * radius;
			}

			T diameter() const
			{
				return radius * 2;
			}
		};
	};

	typedef details::__sphere_t__<int8_t> sphere_i8_t;
	typedef details::__sphere_t__<int16_t> sphere_i16_t;
	typedef details::__sphere_t__<int32_t> sphere_i32_t;
	typedef details::__sphere_t__<int64_t> sphere_i64_t;
	typedef details::__sphere_t__<float32_t> sphere_f32_t;
	typedef details::__sphere_t__<float64_t> sphere_f64_t;
};