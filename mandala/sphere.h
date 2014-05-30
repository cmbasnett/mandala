#pragma once

//mandala
#include "types.h"

namespace mandala
{
	namespace details
    {
        template<typename T, typename Enable = void>
        struct sphere_t;

		template<typename T>
        struct sphere_t<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
		{
            typedef sphere_t<T> type;
			typedef glm::detail::tvec3<T> origin_type;
			typedef float32_t radius_type;

			origin_type origin;
			radius_type radius = 0;

			radius_type area() const
			{
                return 4.0f * glm::pi<float32_t>() * glm::pow2(radius);
			}

			radius_type volume() const
			{
				return (4.0f / 3.0f) * glm::pi<float32_t>() * glm::pow2(radius);
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

    typedef details::sphere_t<int8_t> sphere_i8_t;
    typedef details::sphere_t<int16_t> sphere_i16_t;
    typedef details::sphere_t<int32_t> sphere_i32_t;
    typedef details::sphere_t<int64_t> sphere_i64_t;
    typedef details::sphere_t<float32_t> sphere_f32_t;
    typedef details::sphere_t<float64_t> sphere_f64_t;
};