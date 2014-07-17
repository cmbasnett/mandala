#pragma once

//std
#include <vector>
#include <map>

//mandala
#include "resource.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

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
            
            typedef uint16_t index_type;
            typedef vertex_t vertex_type;

            typedef vertex_buffer_t<vertex_t> vertex_buffer_type;
            typedef index_buffer_t<index_type> index_buffer_type;

			size_t index_count = 0;
            std::shared_ptr<vertex_buffer_type> vertex_buffer;
            std::shared_ptr<index_buffer_type> index_buffer;
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
        struct mesh_info_t
        {
            struct vertex_t
            {
                vec2_t texcoord;
                uint16_t weight_index_start = 0;
                uint8_t weight_count = 0;
            };

            struct weight_t
            {
                uint8_t bone_index = 0;
                float32_t bias = 0;
                vec3_t position;
            };

            std::string shader;
            std::vector<vertex_t> vertices;
            std::vector<uint16_t> indices;
            std::vector<weight_t> weights;
        };

        struct bone_info_t
        {
            std::string name;
            uint8_t parent_index = 0;
            vec3_t position;
            quat_t orientation;
        };

		model_t(const model_t&);
		model_t& operator=(const model_t&);
	};
};
