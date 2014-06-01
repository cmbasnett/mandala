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

	frustum_t& frustum_t::operator = (const mat4_t& matrix)
	{
		//left
		auto& plane = planes[0];
		plane.normal.x = matrix[3][0] + matrix[0][0];
		plane.normal.y = matrix[3][1] + matrix[0][1];
		plane.normal.z = matrix[3][2] + matrix[0][2];
		plane.distance = matrix[3][3] + matrix[0][3];

		//right
		plane = planes[1];
		plane.normal.x = matrix[3][0] - matrix[0][0];
		plane.normal.y = matrix[3][1] - matrix[0][1];
		plane.normal.z = matrix[3][2] - matrix[0][2];
		plane.distance = matrix[3][3] - matrix[0][3];

		//top
		plane = planes[2];
		plane.normal.x = matrix[3][0] - matrix[1][0];
		plane.normal.y = matrix[3][1] - matrix[1][1];
		plane.normal.z = matrix[3][2] - matrix[1][2];
		plane.distance = matrix[3][3] - matrix[1][3];

		//bottom
		plane = planes[3];
		plane.normal.x = matrix[3][0] + matrix[1][0];
		plane.normal.y = matrix[3][1] + matrix[1][1];
		plane.normal.z = matrix[3][2] + matrix[1][2];
		plane.distance = matrix[3][3] + matrix[1][3];

		//near
		plane = planes[4];
		plane.normal.x = matrix[2][0];
		plane.normal.y = matrix[2][1];
		plane.normal.z = matrix[2][2];
		plane.distance = matrix[2][3];

		//far
		plane = planes[5];
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

		corners[0] = intersect(planes[0], planes[2], planes[4]);
		corners[1] = intersect(planes[0], planes[2], planes[5]);
		corners[2] = intersect(planes[0], planes[3], planes[4]);
		corners[3] = intersect(planes[0], planes[3], planes[5]);
		corners[4] = intersect(planes[1], planes[2], planes[4]);
		corners[5] = intersect(planes[1], planes[2], planes[5]);
		corners[6] = intersect(planes[1], planes[3], planes[4]);
		corners[7] = intersect(planes[1], planes[3], planes[5]);

		return corners;
	}
};
