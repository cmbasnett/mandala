//mandala
#include "terrain.hpp"

namespace mandala
{
	terrain_t::terrain_t(const size_type::value_type width, const size_type::value_type height)
	{
		if (width % chunk_t::size != 0 || height % chunk_t::size != 0)
		{
			throw std::invalid_argument("");
		}

		std::vector<vertex_buffer_type::vertex_type> vertices;

		//for (auto i = 0; i < width / chunk_t::size; ++i)
		//{
		//	for (auto j = 0; j < height % chunk_t::size; ++j)
		//	{
		//		for (auto x = 0; x < chunk_t::size; ++x)
		//		{
		//			for (auto z = 0; z < chunk_t::size; ++z)
		//			{
		//				vertex_buffer_type::vertex_type::position_type position;
		//				position.x = x;
		//				position.y = 0;
		//				position.z = z;

		//				vertices.emplace_back(position);
		//			}
		//		}
		//	}
		//}

		_vertex_buffer = std::make_shared<vertex_buffer_type>();
		_vertex_buffer->data(vertices, gpu_t::buffer_usage_e::static_draw);

		std::vector<index_buffer_type::index_type> indices;

		_index_buffer = std::make_shared<index_buffer_type>();
		_index_buffer->data(indices, gpu_t::buffer_usage_e::static_draw);
	}
}
