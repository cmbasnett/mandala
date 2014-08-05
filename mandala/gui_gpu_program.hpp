#pragma once

//std
#include <memory>

//mandala
#include "gpu_program.hpp"

namespace mandala
{
	struct gui_gpu_program_t : gpu_program_t
	{
		struct vertex_t
		{
			vec2_t position;
			vec2_t texcoord;
		};

		typedef vertex_t vertex_type;

		static std::string vertex_shader_source;
		static std::string fragment_shader_source;

		gui_gpu_program_t();

		void on_bind() override;
		void on_unbind() override;

		void world_matrix(const mat4_t&) const;
		void view_projection_matrix(const mat4_t&) const;
		void diffuse_texture_index(uint32_t) const;

	private:
		gpu_t::attribute_location_type position_location = -1;
		gpu_t::attribute_location_type texcoord_location = -1;
		gpu_t::uniform_location_type world_matrix_location = -1;
		gpu_t::uniform_location_type view_projection_matrix_location = -1;
		gpu_t::uniform_location_type diffuse_texture_index_location = -1;
	};

	extern std::shared_ptr<gui_gpu_program_t> gui_gpu_program;
}
