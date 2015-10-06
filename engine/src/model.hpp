#pragma once

//std
#include <vector>
#include <map>

//mandala
#include "pose.hpp"
#include "resource.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "model_gpu_program.hpp"

namespace mandala
{
    struct material_t;
    struct material_instance_t;

    struct model_t : resource_t
    {
        struct bone_t
        {
            mat4 bind_pose_matrix;
            mat4 inverse_bind_pose_matrix;
        };

        struct mesh_t
        {
            typedef u16 index_type;
            typedef model_gpu_program_t::vertex_type vertex_type;
            typedef vertex_buffer_t<vertex_type> vertex_buffer_type;
            typedef index_buffer_t<index_type> index_buffer_type;

            size_t index_count = 0;
            boost::shared_ptr<vertex_buffer_type> vertex_buffer;
            boost::shared_ptr<index_buffer_type> index_buffer;
            boost::shared_ptr<material_t> material;

            mesh_t() = default;

            void render(const mat4& world_matrix, const mat4& view_projection_matrix, const std::vector<mat4>& bone_matrices, const boost::shared_ptr<material_instance_t>& material) const;

        private:
            mesh_t(const mesh_t&) = delete;
            mesh_t& operator=(const mesh_t&) = delete;
        };

        model_t(std::istream& istream);

        void render(const vec3& camera_location, const mat4& world_matrix, const mat4& view_projection_matrix, const std::vector<mat4>& bone_matrices, const vec3& light_location, const std::vector<boost::shared_ptr<material_instance_t>>& mesh_materials) const;

        boost::optional<size_t> get_bone_index(const hash_t& bone_hash) const;

        const std::vector<boost::shared_ptr<const mesh_t>> get_meshes() const { return meshes; }
        const std::vector<const bone_t>& get_bones() const { return bones; }
        const std::vector<const std::string>& get_bone_names() const { return bone_names; }

    private:
        std::vector<boost::shared_ptr<const mesh_t>> meshes;
        std::map<const hash_t, const u8> bone_indices;
        std::vector<const bone_t> bones;
        std::vector<const std::string> bone_names;

        model_t(const model_t&) = delete;
        model_t& operator=(const model_t&) = delete;
    };
}
