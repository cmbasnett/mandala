#pragma once

//std
#include <memory>

//mandala
#include "gpu_program.hpp"

namespace mandala
{
	struct bsp_gpu_program_t : gpu_program_t
	{
		struct vertex_t
		{
			vec3_t position;
			vec2_t diffuse_texcoord;
			vec2_t lightmap_texcoord;
		};

		typedef vertex_t vertex_type;

		static std::string vertex_shader_source;
		static std::string fragment_shader_source;

		bsp_gpu_program_t();

		void on_bind() override;
		void on_unbind() override;

		void world_matrix(const mat4_t&) const;
		void view_projection_matrix(const mat4_t&) const;
		void diffuse_texture_index(uint32_t) const;
		void lightmap_texture_index(uint32_t) const;
		void lightmap_gamma(float32_t) const;
		void alpha(float32_t) const;
		void should_test_alpha(bool) const;

	private:
		gpu_t::attribute_location_type position_location = -1;
		gpu_t::attribute_location_type diffuse_texcoord_location = -1;
		gpu_t::attribute_location_type lightmap_texcoord_location = -1;
		gpu_t::uniform_location_type world_matrix_location = -1;
		gpu_t::uniform_location_type view_projection_matrix_location = -1;
		gpu_t::uniform_location_type diffuse_texture_location = -1;
		gpu_t::uniform_location_type lightmap_texture_location = -1;
		gpu_t::uniform_location_type lightmap_gamma_location = -1;
		gpu_t::uniform_location_type alpha_location = -1;
		gpu_t::uniform_location_type should_test_alpha_location = -1;
	};

	extern std::shared_ptr<bsp_gpu_program_t> bsp_gpu_program;
}