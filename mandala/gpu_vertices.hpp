#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
	struct basic_gpu_vertex_t
    {
        typedef vec3_t location_type;
        typedef rgba_type color_type;

		basic_gpu_vertex_t() = default;
		basic_gpu_vertex_t(const location_type& location, color_type& color) :
            location(location),
			color(color)
		{
		}

		location_type location;
		color_type color;
	};

	struct position_texcoord_vertex_t
	{
        typedef vec3_t location_type;
        typedef vec2_t texcoord_type;

		position_texcoord_vertex_t() = default;
		position_texcoord_vertex_t(const location_type& position, const texcoord_type& texcoord) :
			position(position),
			texcoord(texcoord)
		{
		}

		location_type position;
		texcoord_type texcoord;
	};

	struct terrain_vertex_t
	{
		//TODO: layer weights
		typedef vec3_t location_type;

		terrain_vertex_t() = default;
		terrain_vertex_t(const location_type& position) :
			position(position)
		{
		}

		location_type position;
	};

	struct model_vertex_t
	{
		typedef vec3_t location_type;
		typedef vec3_t normal_type;
		typedef vec2_t texcoord_type;
		typedef vec4_i32_t bone_indices_type;
		typedef vec4_t bone_weights_type;

		location_type position;
		normal_type normal;
		normal_type tangent;
		texcoord_type texcoord;
		bone_indices_type bone_indices_0;
		bone_indices_type bone_indices_1;
		bone_weights_type bone_weights_0;
		bone_weights_type bone_weights_1;
	};

	struct voxel_vertex_t
	{
		typedef vec3_i32_t location_type;

		location_type position;
	};
}
