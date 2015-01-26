#pragma once

//mandala
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "gpu_vertices.hpp"
#include "material.hpp"

namespace mandala
{
	struct terrain_t
	{
		typedef vertex_buffer_t<terrain_vertex_t> vertex_buffer_type;
		typedef index_buffer_t<uint32_t> index_buffer_type;
		typedef vec2_u16_t size_type;

		struct layer_t
		{
			std::shared_ptr<material_t> material;
			float32_t texcoord_scale;
		};

		struct chunk_t
		{
			typedef aabb2_t aabb_type;

			static const size_t size = 8;

			std::shared_ptr<material_t> material;
			aabb_type aabb;
		};

		terrain_t(const size_type::value_type width, const size_type::value_type height);

		std::vector<layer_t> layers;

	private:
		std::shared_ptr<vertex_buffer_type> _vertex_buffer;
		std::shared_ptr<index_buffer_type> _index_buffer;
	};
}
