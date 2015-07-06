#pragma once

#include "types.hpp"

#include <vector>
#include <array>

namespace mandala
{
	inline vec2_t bezier(std::array<vec2_t, 3> points, float32_t t)
	{
		return points[0] + ((points[2] - points[1]) * t - (points[1] - points[0]) * t) * t;
	}
}
