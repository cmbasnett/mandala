#pragma once

//std
#include <memory>

//mandala
#include "gpu_program.hpp"
#include "bitmap_font.hpp"

namespace mandala
{
	struct bitmap_font_gpu_program_t : gpu_program_t
	{
		typedef bitmap_font_t::vertex_type vertex_type;

		static std::string vertex_shader_source;
		static std::string fragment_shader_source;

		bitmap_font_gpu_program_t();

		void on_bind() override;
		void on_unbind() override;

		void world_matrix(const mat4_t&) const;
		void view_projection_matrix(const mat4_t&) const;
		void diffuse_texture_index(uint32_t) const;
		void font_line_height(float32_t) const;
		void font_base(float32_t) const;
		void font_diffuse_texture_index(uint32_t) const;
		void font_color_top(const vec4_t&) const;
		void font_color_bottom(const vec4_t&) const;
        void should_invert_rgb(bool should_invert_rgb) const;

	private:
		gpu_location_t location_location;
		gpu_location_t texcoord_location;
		gpu_location_t world_matrix_location;
		gpu_location_t view_projection_matrix_location;
		gpu_location_t font_line_height_location;
		gpu_location_t font_base_location;
		gpu_location_t font_diffuse_texture_index_location;
		gpu_location_t font_color_top_location;
		gpu_location_t font_color_bottom_location;
		gpu_location_t diffuse_texture_index_location;
        gpu_location_t should_invert_rgb_location;
	};
}
