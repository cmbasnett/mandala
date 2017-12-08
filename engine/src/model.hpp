#pragma once

//std
#include <vector>
#include <map>

//naga
#include "pose.hpp"
#include "resource.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "model_gpu_program.hpp"

namespace naga
{
    struct Material;
	struct MaterialInstance;

	struct Model : Resource
    {
        struct Bone
        {
            mat4 bind_pose_matrix;
            mat4 inverse_bind_pose_matrix;
        };

        struct Mesh
        {
            typedef u16 IndexType;
            typedef model_gpu_program::VertexType VertexType;
			typedef VertexBuffer<VertexType> VertexBufferType;
			typedef IndexBuffer<IndexType> IndexBufferType;

            size_t index_count = 0;
			boost::shared_ptr<VertexBufferType> vertex_buffer;
			boost::shared_ptr<IndexBufferType> index_buffer;
            boost::shared_ptr<Material> material;

			Mesh() = default;

            void render(const mat4& world_matrix, const mat4& view_projection_matrix, const std::vector<mat4>& bone_matrices, const boost::shared_ptr<MaterialInstance>& material) const;

        private:
			Mesh(const Mesh&) = delete;
			Mesh& operator=(const Mesh&) = delete;
        };

        Model(std::istream& istream);

        void render(const vec3& camera_location, const mat4& world_matrix, const mat4& view_projection_matrix, const std::vector<mat4>& bone_matrices, const vec3& light_location, const std::vector<boost::shared_ptr<MaterialInstance>>& mesh_materials) const;

        boost::optional<size_t> get_bone_index(const std::string& bone_name) const;

        const std::vector<boost::shared_ptr<const Mesh>> get_meshes() const { return meshes; }
        const std::vector<const Bone>& get_bones() const { return bones; }
        const std::vector<const std::string>& get_bone_names() const { return bone_names; }

    private:
        std::vector<boost::shared_ptr<const Mesh>> meshes;
        std::map<std::string, const u8> bone_indices;
        std::vector<const Bone> bones;
        std::vector<const std::string> bone_names;

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;
    };
}
