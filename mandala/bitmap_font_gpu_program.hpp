#pragma once

//std
#include <memory>

//mandala
#include "gpu_program.hpp"

namespace mandala
{
	struct bitmap_font_gpu_program_t : gpu_program_t
	{
		struct vertex_t
		{
			vec2_t position;
			vec2_t texcoord;
		};

		typedef vertex_t vertex_type;

		static std::string vertex_shader_source;
		static std::string fragment_shader_source;

		bitmap_font_gpu_program_t();

		void on_bind() override;
		void on_unbind() override;

		void world_matrix(const mat4_t&) const;
		void view_projection_matrix(const mat4_t&) const;
		void font_line_height(float32_t) const;
		void font_base(float32_t) const;
		void font_diffuse_texture_index(uint32_t) const;
		void font_color_top(const vec4_t&) const;
		void font_color_bottom(const vec4_t&) const;

	private:
		gpu_t::attribute_location_type position_location = -1;
		gpu_t::attribute_location_type texcoord_location = -1;
		gpu_t::uniform_location_type world_matrix_location = -1;
		gpu_t::uniform_location_type view_projection_matrix_location = -1;
		gpu_t::uniform_location_type font_line_height_location = -1;
		gpu_t::uniform_location_type font_base_location = -1;
		gpu_t::uniform_location_type font_diffuse_texture_index_location = -1;
		gpu_t::uniform_location_type font_color_top_location = -1;
		gpu_t::uniform_location_type font_color_bottom_location = -1;
	};

	extern std::shared_ptr<bitmap_font_gpu_program_t> bitmap_font_gpu_program;
}