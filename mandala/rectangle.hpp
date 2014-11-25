#pragma once

//mandala
#include "types.hpp"
#include "aabb.hpp"

namespace mandala
{
	namespace details
	{
		template<typename Scalar, typename Enable = void>
		struct rectangle_t;

		template<typename Scalar>
        struct rectangle_t<Scalar, typename std::enable_if<std::is_arithmetic<Scalar>::value>::type>
		{
            typedef Scalar scalar_type;
			typedef glm::detail::tvec2<scalar_type> vector_type;
			typedef rectangle_t<scalar_type> type;

			scalar_type x = scalar_type(0);
			scalar_type y = scalar_type(0);
			scalar_type width = scalar_type(0);
			scalar_type height = scalar_type(0);

            rectangle_t()
            {
            }

            rectangle_t(scalar_type x, scalar_type y, scalar_type width, scalar_type height) :
                x(x),
                y(y),
                width(width),
                height(height)
            {
            }

            template<typename Scalar>
            rectangle_t(const aabb2_t<Scalar>& aabb)
            {
                x = static_cast<scalar_type>(aabb.min.x);
                y = static_cast<scalar_type>(aabb.min.y);
                width = static_cast<scalar_type>(aabb.width());
                height = static_cast<scalar_type>(aabb.height());
            }

			scalar_type area() const
			{
				return width * height;
			}

			vector_type center() const
			{
				return vector_type(x + (width / 2), y + (height / 2));
			}

			vector_type size() const
			{
				return vector_type(width, height);
			}

			vector_type min() const
			{
				return vector_type(x, y);
			}

			vector_type max() const
			{
				return vector_type(x + width, y + height);
            }
		};
	}

	typedef details::rectangle_t<int8_t> rectangle_i8_t;
    typedef details::rectangle_t<int16_t> rectangle_i16_t;
    typedef details::rectangle_t<uint16_t> rectangle_u16_t;
	typedef details::rectangle_t<int32_t> rectangle_i32_t;
	typedef details::rectangle_t<int64_t> rectangle_i64_t;
	typedef details::rectangle_t<float32_t> rectangle_f32_t;
	typedef details::rectangle_t<float64_t> rectangle_f64_t;
	typedef rectangle_f32_t rectangle_t;
}
