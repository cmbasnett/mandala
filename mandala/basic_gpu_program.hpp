#pragma once

//std
#include <memory>

//mandala
#include "gpu_program.hpp"
#include "gpu_vertices.hpp"

namespace mandala
{
	struct basic_gpu_program_t : gpu_program_t
	{
		typedef basic_gpu_vertex_t vertex_type;

		static std::string vertex_shader_source;
		static std::string fragment_shader_source;

		basic_gpu_program_t();

		void on_bind() override;
		void on_unbind() override;

	private:
		gpu_location_t location_location;
		gpu_location_t color_location;
	};
}
