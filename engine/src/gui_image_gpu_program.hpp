#pragma once

//boost
#include <boost\shared_ptr.hpp>

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

	private:
		gpu_location_t location_location;
		gpu_location_t texcoord_location;
	};
}
