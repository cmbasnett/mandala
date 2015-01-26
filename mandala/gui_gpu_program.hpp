#pragma once

//std
#include <memory>

//mandala
#include "gpu_program.hpp"
#include "gpu_vertices.hpp"

namespace mandala
{
	struct gui_gpu_program_t : gpu_program_t
	{
		typedef position_texcoord_vertex_t vertex_type;

		static std::string vertex_shader_source;
		static std::string fragment_shader_source;

		gui_gpu_program_t();

		void on_bind() override;
		void on_unbind() override;

		void world_matrix(const mat4_t&) const;
		void view_projection_matrix(const mat4_t&) const;
		void diffuse_texture_index(uint32_t) const;
		void color(const vec4_t&) const;

	private:
		gpu_location_t position_location;
		gpu_location_t texcoord_location;
		gpu_location_t world_matrix_location;
		gpu_location_t view_projection_matrix_location;
		gpu_location_t diffuse_texture_index_location;
		gpu_location_t color_location;
	};
}
