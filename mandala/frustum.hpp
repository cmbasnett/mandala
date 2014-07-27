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

		std::array<plane3_t, plane_count> planes;

		frustum_t();
		frustum_t(const mat4_t& matrix);

		std::array<vec3_t, corner_count> get_corners() const;

		frustum_t& operator=(const mat4_t& matrix);
	};
};
