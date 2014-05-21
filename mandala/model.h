#pragma once

//mandala
#include "mandala.h"
#include "resource.h"

//std
#include <vector>
#include <map>

//boost
#include <boost\filesystem\path.hpp>

namespace mandala
{
	struct material_t;

	struct model_t : resource_t
	{
		struct bone_t
		{
			mat4_t bind_pose_matrix;
			mat4_t inverse_bind_pose_matrix;
		};

		struct mesh_t
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

			size_t index_count = 0;
			uint32_t vertex_buffer = 0;
			uint32_t index_buffer = 0;
			std::shared_ptr<material_t> material;

			mesh_t() { }

			void render(const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const std::vector<mat4_t>& bone_matrices) const;

		private:
			mesh_t(const mesh_t&) = delete;
			mesh_t& operator=(const mesh_t&) = delete;
		};

		model_t(std::istream& istream);

		std::vector<std::shared_ptr<mesh_t>> meshes;
		std::map<hash_t, uint8_t> bone_indices;
		std::vector<bone_t> bones;

		void render(const vec3_t& camera_position, const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const std::vector<mat4_t>& bone_matrices, const vec3_t& light_position) const;

	private:
		model_t(const model_t&);
		model_t& operator=(const model_t&);
	};
};
