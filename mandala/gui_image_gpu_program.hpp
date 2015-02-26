#pragma once

//std
#include <memory>

//mandala
#include "gpu_program.hpp"
#include "gpu_vertices.hpp"

namespace mandala
{
	struct gui_image_gpu_program_t : gpu_program_t
	{
		typedef basic_gpu_vertex_t vertex_type;

		static std::string vertex_shader_source;
		static std::string fragment_shader_source;

		gui_image_gpu_program_t();

		void on_bind() override;
		void on_unbind() override;

		void world_matrix(const mat4_t&) const;
		void view_projection_matrix(const mat4_t&) const;
		void diffuse_texture_index(uint32_t) const;
		void color(const vec4_t&) const;
		void region_min(const vec2_t&) const;
		void region_size(const vec2_t&) const;

	private:
		gpu_location_t location_location;
		gpu_location_t texcoord_location;
		gpu_location_t world_matrix_location;
		gpu_location_t view_projection_matrix_location;
		gpu_location_t diffuse_texture_index_location;
		gpu_location_t color_location;
        gpu_location_t region_min_location;
        gpu_location_t region_size_location;
	};
}
