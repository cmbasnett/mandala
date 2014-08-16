#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
	struct position_texcoord_vertex_t
	{
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