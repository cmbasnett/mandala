#pragma once

//std
#include <array>

//mandala
#include "types.hpp"
#include "plane.hpp"

namespace mandala
{
	struct frustum_t
	{
		static const size_t plane_count = 6;
		static const size_t corner_count = 8;

		enum planes_t
		{
			left = 0,
			right,
			top,
			bottom,
			near,
			far
		};

		std::array<plane3_t, plane_count> planes;

		frustum_t();
		frustum_t(const mat4_t& matrix);

		std::array<vec3_t, corner_count> get_corners() const;

		frustum_t& operator=(const mat4_t& matrix);
	};
};
