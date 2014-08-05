#pragma once

//std
#include <array>

//mandala
#include "types.hpp"
#include "plane.hpp"

namespace mandala
{
	namespace details
	{
		template<typename T, typename Enable = void>
		struct frustum_t;

		template<typename T>
		struct frustum_t<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
		{
			typedef frustum_t<T> type;
			typedef details::plane3_t<T> plane_type;
			typedef glm::detail::tmat4x4<T> matrix_type;
			typedef glm::detail::tvec3<T> corner_type;

			static const size_t plane_count = 6;
			static const size_t corner_count = 8;

			enum
			{
				plane_index_left,
				plane_index_right,
				plane_index_top,
				plane_index_bottom,
				plane_index_near,
				plane_index_far
			};

			enum
			{
				corner_index_left_top_near,
				corner_index_left_top_far,
				corner_index_left_bottom_near,
				corner_index_left_bottom_far,
				corner_index_right_top_near,
				corner_index_right_top_far,
				corner_index_right_bottom_near,
				corner_index_right_bottom_far
			};

			std::array<plane_type, plane_count> planes;

			frustum_t() = default;

			frustum_t(const matrix_type& matrix)
			{
				*this = matrix;
			}

			std::array<corner_type, corner_count> get_corners() const
			{
				//TODO: use initializer list so there's no temporary array
				std::array<corner_type, corner_count> corners;

				corners[corner_index_left_top_near] = intersect(planes[plane_index_left], planes[plane_index_top], planes[plane_index_near]);
				corners[corner_index_left_top_far] = intersect(planes[plane_index_left], planes[plane_index_top], planes[plane_index_far]);
				corners[corner_index_left_bottom_near] = intersect(planes[plane_index_left], planes[plane_index_bottom], planes[plane_index_near]);
				corners[corner_index_left_bottom_far] = intersect(planes[plane_index_left], planes[plane_index_bottom], planes[plane_index_far]);
				corners[corner_index_right_top_near] = intersect(planes[plane_index_right], planes[plane_index_top], planes[plane_index_near]);
				corners[corner_index_right_top_far] = intersect(planes[plane_index_right], planes[plane_index_top], planes[plane_index_far]);
				corners[corner_index_right_bottom_near] = intersect(planes[plane_index_right], planes[plane_index_bottom], planes[plane_index_near]);
				corners[corner_index_right_bottom_far] = intersect(planes[plane_index_right], planes[plane_index_bottom], planes[plane_index_far]);

				return corners;
			}

			type& operator=(const matrix_type& matrix)
			{
				auto& plane = planes[plane_index_left];
				plane.normal.x = matrix[3][0] + matrix[0][0];
				plane.normal.y = matrix[3][1] + matrix[0][1];
				plane.normal.z = matrix[3][2] + matrix[0][2];
				plane.distance = matrix[3][3] + matrix[0][3];

				plane = planes[plane_index_right];
				plane.normal.x = matrix[3][0] - matrix[0][0];
				plane.normal.y = matrix[3][1] - matrix[0][1];
				plane.normal.z = matrix[3][2] - matrix[0][2];
				plane.distance = matrix[3][3] - matrix[0][3];

				plane = planes[plane_index_top];
				plane.normal.x = matrix[3][0] - matrix[1][0];
				plane.normal.y = matrix[3][1] - matrix[1][1];
				plane.normal.z = matrix[3][2] - matrix[1][2];
				plane.distance = matrix[3][3] - matrix[1][3];

				plane = planes[plane_index_bottom];
				plane.normal.x = matrix[3][0] + matrix[1][0];
				plane.normal.y = matrix[3][1] + matrix[1][1];
				plane.normal.z = matrix[3][2] + matrix[1][2];
				plane.distance = matrix[3][3] + matrix[1][3];

				plane = planes[plane_index_near];
				plane.normal.x = matrix[2][0];
				plane.normal.y = matrix[2][1];
				plane.normal.z = matrix[2][2];
				plane.distance = matrix[2][3];

				plane = planes[plane_index_far];
				plane.normal.x = matrix[3][0] - matrix[2][0];
				plane.normal.y = matrix[3][1] - matrix[2][1];
				plane.normal.z = matrix[3][2] - matrix[2][2];
				plane.distance = matrix[3][3] - matrix[2][3];

				for (auto plane : planes)
				{
					plane.normalize();
				}

				return *this;
			}
		};
	}

	typedef details::frustum_t<float32_t> frustum_f32_t;
	typedef details::frustum_t<float64_t> frustum_f64_t;
	typedef frustum_f32_t frustum_t;
}
