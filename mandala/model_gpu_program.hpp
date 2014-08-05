#pragma once

//std
#include <memory>

//mandala
#include "gpu_program.hpp"

namespace mandala
{
	struct model_gpu_program_t : gpu_program_t
	{
		struct vertex_t
		{
			vec3_t position;
			vec3_t normal;
			vec3_t tangent;
			vec2_t texcoord;
			vec4_i32_t bone_indices_0;
			vec4_i32_t bone_indices_1;
			vec4_t bone_weights_0;
			vec4_t bone_weights_1;
		};

		typedef vertex_t vertex_type;

		static std::string vertex_shader_source;
		static std::string fragment_shader_source;

		model_gpu_program_t();

		void on_bind() override;
		void on_unbind() override;

		void world_matrix(const mat4_t&) const;
		void normal_matrix(const mat3_t&) const;
		void view_projection_matrix(const mat4_t&) const;
		void bone_matrices(const std::vector<mat4_t>&) const;
		void light_position(const vec3_t&) const;
		void camera_position(const vec3_t&) const;
		void diffuse_texture_index(uint32_t) const;
		void diffuse_color(const vec4_t&) const;
		void normal_texture_index(uint32_t) const;
		void specular_texture_index(uint32_t) const;
		void specular_color(const vec4_t&) const;
		void specular_intensity(float32_t) const;
		void emissive_texture_index(uint32_t) const;
		void emissive_color(const vec4_t&) const;
		void emissive_intensity(float32_t) const;

	private:
		gpu_t::attribute_location_type position_location = -1;
		gpu_t::attribute_location_type normal_location = -1;
		gpu_t::attribute_location_type tangent_location = -1;
		gpu_t::attribute_location_type texcoord_location = -1;
		gpu_t::attribute_location_type bone_indices_0_location = -1;
		gpu_t::attribute_location_type bone_indices_1_location = -1;
		gpu_t::attribute_location_type bone_weights_0_location = -1;
		gpu_t::attribute_location_type bone_weights_1_location = -1;
		gpu_t::uniform_location_type world_matrix_location = -1;
		gpu_t::uniform_location_type normal_matrix_location = -1;
		gpu_t::uniform_location_type view_projection_matrix_location = -1;
		gpu_t::uniform_location_type bone_matrices_location = -1;
		gpu_t::uniform_location_type light_position_location = -1;
		gpu_t::uniform_location_type camera_position_location = -1;
		gpu_t::uniform_location_type diffuse_texture_index_location = -1;
		gpu_t::uniform_location_type diffuse_color_location = -1;
		gpu_t::uniform_location_type normal_texture_index_location = -1;
		gpu_t::uniform_location_type specular_texture_index_location = -1;
		gpu_t::uniform_location_type specular_color_location = -1;
		gpu_t::uniform_location_type specular_intensity_location = -1;
		gpu_t::uniform_location_type emissive_texture_index_location = -1;
		gpu_t::uniform_location_type emissive_color_location = -1;
		gpu_t::uniform_location_type emissive_intensity_location = -1;
	};

	extern std::shared_ptr<model_gpu_program_t> model_gpu_program;
}
