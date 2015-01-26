#pragma once

//std
#include <memory>

//mandala
#include "gpu_program.hpp"
#include "bsp.hpp"

namespace mandala
{
	struct bsp_gpu_program_t : gpu_program_t
	{
		typedef bsp_t::vertex_type vertex_type;

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
		gpu_location_t position_location = -1;
		gpu_location_t diffuse_texcoord_location = -1;
		gpu_location_t lightmap_texcoord_location = -1;
		gpu_location_t world_matrix_location = -1;
		gpu_location_t view_projection_matrix_location = -1;
		gpu_location_t diffuse_texture_location = -1;
		gpu_location_t lightmap_texture_location = -1;
		gpu_location_t lightmap_gamma_location = -1;
		gpu_location_t alpha_location = -1;
		gpu_location_t should_test_alpha_location = -1;
	};
}