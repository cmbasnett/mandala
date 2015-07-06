#pragma once

//boost
#include <boost\shared_ptr.hpp>

//mandala
#include "gpu_program.hpp"

namespace mandala
{
	struct model_gpu_program_t : gpu_program_t
	{
		struct vertex_t
		{
			vec3_t location;
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

        void is_lit(bool) const;

	private:
		gpu_location_t location_location;
		gpu_location_t normal_location;
		gpu_location_t tangent_location;
		gpu_location_t texcoord_location;
		gpu_location_t bone_indices_0_location;
		gpu_location_t bone_indices_1_location;
		gpu_location_t bone_weights_0_location;
		gpu_location_t bone_weights_1_location;

		gpu_location_t calculate_lighting_function_location;
		gpu_index_t calculate_lighting_lit_subroutine_index;
		gpu_index_t calculate_lighting_unlit_subroutine_index;
	};
}
