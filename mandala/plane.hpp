#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
	namespace details
	{
		template<typename T, typename Enable = void>
		struct plane2_t;

		template<typename T>
		struct plane2_t<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
		{
			typedef plane2_t<T> type;
			typedef T distance_type;
			typedef glm::detail::tvec2<T> vector_type;

			vector_type normal;
			distance_type distance = distance_type(0);

			plane2_t()
			{
			}

			plane2_t(const vector_type& normal, distance_type distance) :
				normal(normal),
				distance(distance)
			{
			}

			plane2_t(const vector_type& origin, const vector_type& normal) :
				normal(normal)
			{
				distance = glm::dot(normal, origin);
			}

			void normalize()
			{
				auto length = glm::length(normal);
				normal /= length;
				distance /= length;
			}

			inline vector_type origin() const
			{
				return normal * distance;
			}

			type operator-() const
			{
				type plane;
				plane.normal = -normal;
				plane.distance = -distance;
				return plane;
			}
		};
	}

	typedef details::plane2_t<float32_t> plane2_f32_t;
	typedef details::plane2_t<float64_t> plane2_f64_t;
	typedef plane2_f32_t plane2_t;

	namespace details
	{
		template<typename T, typename Enable = void>
		struct plane3_t;

		template<typename T>
		struct plane3_t<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
		{
			typedef plane3_t<T> type;
			typedef T distance_type;
			typedef glm::detail::tvec3<T> vector_type;

			vector_type normal;
			distance_type distance = 0;

			plane3_t()
			{
			}

			plane3_t(const vector_type& normal, distance_type distance) :
				normal(normal),
				distance(distance)
			{
			}

			plane3_t(const vector_type& origin, const vector_type& normal) :
				normal(normal)
			{
				distance = glm::dot(normal, origin);
			}

			plane3_t(const vector_type& v0, const vector_type& v1, const vector_type& v2)
			{
				auto a = v1 - v0;
				auto b = v2 - v0;

				normal = glm::normalize(glm::cross(a, b));
				distance = glm::dot(normal, v0);
			}

			void normalize()
			{
				auto length = glm::length(normal);
				normal /= length;
				distance /= length;
			}

			inline vector_type origin() const
			{
				return normal * distance;
			}

			type operator-() const
			{
				type plane;
				plane.normal = -normal;
				plane.distance = -distance;
				return plane;
			}
		};
	}

	typedef details::plane3_t<float32_t> plane3_f32_t;
	typedef details::plane3_t<float64_t> plane3_f64_t;
	typedef plane3_f32_t plane3_t;
}
