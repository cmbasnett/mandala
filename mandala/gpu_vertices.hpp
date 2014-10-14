#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
	struct basic_gpu_vertex_t
    {
        typedef vec3_t position_type;
        typedef rgba_type color_type;

		basic_gpu_vertex_t() = default;
		basic_gpu_vertex_t(const vec3_t& position, vec4_t& color) :
			position(position),
			color(color)
		{
		}

		vec3_t position;
        rgba_type color;
	};

	struct position_texcoord_vertex_t
	{
        typedef vec3_t position_type;
        typedef vec2_t texcoord_type;

		position_texcoord_vertex_t() = default;
		position_texcoord_vertex_t(const vec3_t& position, const vec2_t& texcoord) :
			position(position),
			texcoord(texcoord)
		{
		}

		vec3_t position;
		vec2_t texcoord;
	};
}