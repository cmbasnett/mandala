#pragma once

//std
#include <memory>

//mandala
#include "gpu_program.hpp"
#include "gpu_vertices.hpp"

namespace mandala
{
	struct blur_horizontal_gpu_program_t : gpu_program_t
	{
		static std::string vertex_shader_source;
		static std::string fragment_shader_source;

		typedef basic_gpu_vertex_t vertex_type;

		blur_horizontal_gpu_program_t();

		void on_bind() override;
		void on_unbind() override;

	private:
		gpu_location_t location_location;
		gpu_location_t texcoord_location;
	};
}
