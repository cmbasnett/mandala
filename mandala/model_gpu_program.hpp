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
        void is_lit(bool) const;

	private:
		gpu_location_t position_location;
		gpu_location_t normal_location;
		gpu_location_t tangent_location;
		gpu_location_t texcoord_location;
		gpu_location_t bone_indices_0_location;
		gpu_location_t bone_indices_1_location;
		gpu_location_t bone_weights_0_location;
		gpu_location_t bone_weights_1_location;
		gpu_location_t world_matrix_location;
		gpu_location_t normal_matrix_location;
		gpu_location_t view_projection_matrix_location;
		gpu_location_t bone_matrices_location;
		gpu_location_t light_position_location;
		gpu_location_t camera_position_location;
		gpu_location_t diffuse_texture_index_location;
		gpu_location_t diffuse_color_location;
		gpu_location_t normal_texture_index_location;
		gpu_location_t specular_texture_index_location;
		gpu_location_t specular_color_location;
		gpu_location_t specular_intensity_location;
		gpu_location_t emissive_texture_index_location;
		gpu_location_t emissive_color_location;
		gpu_location_t emissive_intensity_location;
		gpu_location_t calculate_lighting_function_location;
		gpu_index_t calculate_lighting_lit_subroutine_index;
		gpu_index_t calculate_lighting_unlit_subroutine_index;
	};
}
