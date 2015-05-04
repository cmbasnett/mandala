#pragma once

//std
#include <vector>
#include <map>

//mandala
#include "pose.hpp"
#include "resource.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "gpu_vertices.hpp"

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
			typedef uint16_t index_type;
			typedef model_vertex_t vertex_type;

            typedef vertex_buffer_t<vertex_type> vertex_buffer_type;
            typedef index_buffer_t<index_type> index_buffer_type;

			size_t index_count = 0;
            boost::shared_ptr<vertex_buffer_type> vertex_buffer;
            boost::shared_ptr<index_buffer_type> index_buffer;
			boost::shared_ptr<material_t> material;

			mesh_t() = default;

			void render(const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const std::vector<mat4_t>& bone_matrices) const;

		private:
			mesh_t(const mesh_t&) = delete;
			mesh_t& operator=(const mesh_t&) = delete;
		};

		model_t(std::istream& istream);

		void render(const vec3_t& camera_location, const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const std::vector<mat4_t>& bone_matrices, const vec3_t& light_location) const;

        boost::optional<size_t> get_bone_index(const hash_t& bone_hash) const;

        std::vector<boost::shared_ptr<mesh_t>> meshes;
        std::map<hash_t, uint8_t> bone_indices;
        std::vector<bone_t> bones;

	private:
		model_t(const model_t&) = delete;
		model_t& operator=(const model_t&) = delete;
	};
}
