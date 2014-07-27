#pragma once

//mandala
#include "frustum.hpp"
#include "collision.hpp"

namespace mandala
{
	frustum_t::frustum_t()
	{
	}

	frustum_t::frustum_t(const mat4_t& matrix)
	{
		*this = matrix;
	}

    frustum_t& frustum_t::operator=(const mat4_t& matrix)
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

	std::array<vec3_t, frustum_t::corner_count> frustum_t::get_corners() const
	{
		//TODO: use initializer list so there's no temporary array
        std::array<vec3_t, frustum_t::corner_count> corners;

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
};
