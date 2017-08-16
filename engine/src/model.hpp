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
    struct material;
    struct material_instance;

    struct model : resource
    {
        struct bone
        {
            mat4 bind_pose_matrix;
            mat4 inverse_bind_pose_matrix;
        };

        struct mesh
        {
            typedef u16 index_type;
            typedef model_gpu_program::vertex_type vertex_type;
            typedef vertex_buffer<vertex_type> vertex_buffer_type;
            typedef index_buffer<index_type> index_buffer_type;

            size_t index_count = 0;
            boost::shared_ptr<vertex_buffer_type> vertex_buffer;
            boost::shared_ptr<index_buffer_type> index_buffer;
            boost::shared_ptr<material> material;

            mesh() = default;

            void render(const mat4& world_matrix, const mat4& view_projection_matrix, const std::vector<mat4>& bone_matrices, const boost::shared_ptr<material_instance>& material) const;

        private:
            mesh(const mesh&) = delete;
            mesh& operator=(const mesh&) = delete;
        };

        model(std::istream& istream);

        void render(const vec3& camera_location, const mat4& world_matrix, const mat4& view_projection_matrix, const std::vector<mat4>& bone_matrices, const vec3& light_location, const std::vector<boost::shared_ptr<material_instance>>& mesh_materials) const;

        boost::optional<size_t> get_bone_index(const std::string& bone_name) const;

        const std::vector<boost::shared_ptr<const mesh>> get_meshes() const { return meshes; }
        const std::vector<const bone>& get_bones() const { return bones; }
        const std::vector<const std::string>& get_bone_names() const { return bone_names; }

    private:
        std::vector<boost::shared_ptr<const mesh>> meshes;
        std::map<std::string, const u8> bone_indices;
        std::vector<const bone> bones;
        std::vector<const std::string> bone_names;

        model(const model&) = delete;
        model& operator=(const model&) = delete;
    };
}
