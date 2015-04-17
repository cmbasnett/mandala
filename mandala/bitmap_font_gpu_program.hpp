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

	private:
		gpu_location_t location_location;
		gpu_location_t texcoord_location;
	};
}
