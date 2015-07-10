//std
#include <fstream>

//boost
#include <boost\filesystem\path.hpp>

//glm
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtx\norm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>    

//mandala
#include "model.hpp"
#include "resource_mgr.hpp"
#include "gpu_program_mgr.hpp"
#include "md5b.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "gpu_program.hpp"
#include "gpu.hpp"
#include "model_gpu_program.hpp"
#include "io.hpp"

#define MD5M_MAGIC_LENGTH       (4)
#define MD5M_MAGIC              (std::array<char, MD5M_MAGIC_LENGTH> { 'M', 'D', '5', 'M' })
#define MD5M_VERSION            (1)

namespace mandala
{
    struct bone_info_t
    {
        std::string name;
        uint8_t parent_index = 0;
        pose3 pose;
    };

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
            vec3_t location;
        };

        std::string shader;
        std::vector<vertex_t> vertices;
        std::vector<uint16_t> indices;
        std::vector<weight_t> weights;
    };

    model_t::model_t(std::istream& istream)
    {
        //magic
        std::array<char, MD5M_MAGIC_LENGTH> magic;
        read(istream, magic);

        if (magic != MD5M_MAGIC)
        {
            throw std::exception();
        }

        //version
        int32_t version = 0;
        read(istream, version);

        if (version != MD5M_VERSION)
        {
            throw std::exception();
        }

        //bone count
        uint32_t bone_count;
        read(istream, bone_count);

        //bones
        std::vector<bone_info_t> bone_infos;

        bone_infos.resize(bone_count);

        for (uint8_t i = 0; i < bone_infos.size(); ++i)
        {
            auto& bone_info = bone_infos[i];

            std::getline(istream, bone_info.name, '\0');

            bone_names.push_back(bone_info.name);

            bone_indices.insert(std::make_pair(hash_t(bone_info.name), i));

            read(istream, bone_info.parent_index);
            read(istream, bone_info.pose.location.x);
            read(istream, bone_info.pose.location.y);
            read(istream, bone_info.pose.location.z);
            read(istream, bone_info.pose.rotation.x);
            read(istream, bone_info.pose.rotation.y);
            read(istream, bone_info.pose.rotation.z);

            md5b::compute_quaternion_w(bone_info.pose.rotation);
        }

        //mesh count
        uint8_t mesh_count = 0;
        read(istream, mesh_count);

        std::vector<mesh_info_t> mesh_infos;
        mesh_infos.resize(mesh_count);

        for (auto& mesh : mesh_infos)
        {
            //shader
            std::getline(istream, mesh.shader, '\0');

            //vertex count
            uint32_t vertex_count;
            read(istream, vertex_count);

            //vertices
            mesh.vertices.resize(vertex_count);

            for (auto& vertex : mesh.vertices)
            {
                read(istream, vertex.texcoord.x);
                read(istream, vertex.texcoord.y);
                read(istream, vertex.weight_index_start);
                read(istream, vertex.weight_count);
            }

            //index count
            uint32_t index_count;
            read(istream, index_count);
            read(istream, mesh.indices, index_count);

            //weight count
            uint32_t weight_count;
            read(istream, weight_count);

            //weights
            mesh.weights.resize(weight_count);

            for (auto& weight : mesh.weights)
            {
                read(istream, weight.bone_index);
                read(istream, weight.bias);
                read(istream, weight.location.x);
                read(istream, weight.location.y);
                read(istream, weight.location.z);
            }
        }

        //meshes
        meshes.reserve(mesh_infos.size());

        for (auto& mesh_info : mesh_infos)
        {
            auto mesh = boost::make_shared<mesh_t>();

            //index count
            mesh->index_count = mesh_info.indices.size();

            //material
            mesh->material = resources.get<material_t>(hash_t(mesh_info.shader));

            //vertices
            std::vector<mesh_t::vertex_type> vertices;
            vertices.resize(mesh_info.vertices.size());

            for (size_t j = 0; j < mesh_info.vertices.size(); ++j)
            {
                const auto& vertex_info = mesh_info.vertices[j];
                auto& vertex = vertices[j];

                vertex.texcoord = vertex_info.texcoord;

                for (int k = 0; k < vertex_info.weight_count && k < 8; ++k)
                {
                    const auto& weight = mesh_info.weights[vertex_info.weight_index_start + k];
                    const auto& bone = bone_infos[weight.bone_index];

                    if (k < 4)
                    {
                        vertex.bone_weights_0[k] = weight.bias;
                        vertex.bone_indices_0[k] = weight.bone_index;
                    }
                    else
                    {
                        vertex.bone_weights_1[k % 4] = weight.bias;
                        vertex.bone_indices_1[k % 4] = weight.bone_index;
                    }

                    auto rotated_location = bone.pose.rotation * weight.location;
                    vertex.location += (bone.pose.location + rotated_location) * weight.bias;
                }
            }

#if defined(DEBUG)
            size_t bad_face_count = 0;
#endif

            //normals, tangents & binormals
            for (size_t j = 0; j < mesh_info.indices.size();)
            {
                auto& vertex_0 = vertices[mesh_info.indices[j++]];
                auto& vertex_1 = vertices[mesh_info.indices[j++]];
                auto& vertex_2 = vertices[mesh_info.indices[j++]];

                auto v0 = vertex_1.location - vertex_0.location;
                auto v1 = vertex_2.location - vertex_0.location;

                //normal
                auto normal = glm::cross(v0, v1);
                normal = glm::normalize(normal);

                vertex_0.normal += normal;
                vertex_1.normal += normal;
                vertex_2.normal += normal;

                //tangent
                auto t0 = vertex_2.texcoord - vertex_0.texcoord;
                auto t1 = vertex_1.texcoord - vertex_0.texcoord;

                auto denominator = (t0.x * t1.y) - (t1.x * t0.y);

                if (t0 == mesh_t::vertex_type::texcoord_type(0) || t1 == mesh_t::vertex_type::texcoord_type(0) || denominator == 0.0f)
                {
#if defined(DEBUG)
                    ++bad_face_count;
#endif
                    continue;
                }

                denominator = 1.0f / denominator;

                mesh_t::vertex_type::normal_type tangent;
                tangent.x = denominator * ((v0.x * t1.y) + (v1.x * -t0.y));
                tangent.y = denominator * ((v0.y * t1.y) + (v1.y * -t0.y));
                tangent.z = denominator * ((v0.z * t1.y) + (v1.z * -t0.y));
                tangent = glm::normalize(tangent);

                vertex_0.tangent += tangent;
                vertex_1.tangent += tangent;
                vertex_2.tangent += tangent;
            }

            for (auto& vertex : vertices)
            {
                vertex.normal = glm::normalize(vertex.normal);

                if (vertex.tangent != mesh_t::vertex_type::normal_type(0))
                {
                    vertex.tangent = glm::normalize(vertex.tangent);
                }
            }

            mesh->vertex_buffer = boost::make_shared<mesh_t::vertex_buffer_type>();
            mesh->vertex_buffer->data(vertices, gpu_t::buffer_usage_e::STATIC_DRAW);

            mesh->index_buffer = boost::make_shared<mesh_t::index_buffer_type>();
            mesh->index_buffer->data(mesh_info.indices, gpu_t::buffer_usage_e::STATIC_DRAW);

            meshes.push_back(mesh);
        }

        //bind pose bone matrices
        bones.resize(bone_count);

        for (uint8_t i = 0; i < bone_count; ++i)
        {
            const auto& bone_info = bone_infos[i];
            auto& bone = bones[i];

            bone.bind_pose_matrix = bone_info.pose.to_matrix();
            bone.inverse_bind_pose_matrix = glm::inverse(bone.bind_pose_matrix);
        }
    }

    void model_t::render(const vec3_t& camera_location, const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const std::vector<mat4_t>& bone_matrices, const vec3_t& light_location) const
    {
        //blend
        auto blend_state = gpu.blend.get_state();
        blend_state.is_enabled = true;
        blend_state.src_factor = gpu_t::blend_factor_e::SRC_ALPHA;
        blend_state.dst_factor = gpu_t::blend_factor_e::ONE_MINUS_SRC_ALPHA;

        gpu.blend.push_state(blend_state);

        //depth
        auto depth_state = gpu.depth.get_state();
        depth_state.should_test = true;

        gpu.depth.push_state(depth_state);

        for (auto& mesh : meshes)
        {
            //culling
            auto culling_state = gpu.culling.get_state();

            culling_state.is_enabled = !mesh->material->get_is_two_sided();

            gpu.culling.push_state(culling_state);

            gpu.buffers.push(gpu_t::buffer_target_e::ARRAY, mesh->vertex_buffer);
            gpu.buffers.push(gpu_t::buffer_target_e::ELEMENT_ARRAY, mesh->index_buffer);

            const auto gpu_program = gpu_programs.get<model_gpu_program_t>();

            gpu.programs.push(gpu_program);

            gpu.set_uniform("world_matrix", world_matrix);
            gpu.set_uniform("view_projection_matrix", view_projection_matrix);
            gpu.set_uniform("normal_matrix", glm::inverseTranspose(glm::mat3(world_matrix)));
            gpu.set_uniform("bone_matrices", bone_matrices);
            gpu.set_uniform("light_location", light_location);
            gpu.set_uniform("camera_location", camera_location);

            gpu_program->is_lit(mesh->material->get_is_lit());

            mesh->render(world_matrix, view_projection_matrix, bone_matrices);

            gpu.programs.pop();

            gpu.buffers.pop(gpu_t::buffer_target_e::ELEMENT_ARRAY);
            gpu.buffers.pop(gpu_t::buffer_target_e::ARRAY);

            gpu.culling.pop_state();
        }

        gpu.depth.pop_state();

        gpu.blend.pop_state();
    }

    boost::optional<size_t> model_t::get_bone_index(const hash_t & bone_hash) const
    {
        boost::optional<size_t> bone_index;

        auto bone_indices_itr = bone_indices.find(bone_hash);

        if (bone_indices_itr != bone_indices.end())
        {
            bone_index = bone_indices_itr->second;
        }

        return bone_index;
    }

    void model_t::mesh_t::render(const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const std::vector<mat4_t>& bone_matrices) const
    {
        static const auto DIFFUSE_TEXTURE_INDEX = 0;
        static const auto NORMAL_TEXTURE_INDEX = 1;
        static const auto SPECULAR_TEXTURE_INDEX = 2;
        static const auto EMISSIVE_TEXTURE_INDEX = 3;

        const auto gpu_program = gpu_programs.get<model_gpu_program_t>();

        //material
        if (material != nullptr)
        {
            //diffuse
            const auto& diffuse = material->get_diffuse();

            //texture
            gpu.textures.bind(DIFFUSE_TEXTURE_INDEX, diffuse.texture);

            gpu.set_uniform("diffuse.texture", DIFFUSE_TEXTURE_INDEX);
            gpu.set_uniform("diffuse.color", diffuse.color);

            //normal
            const auto& normal = material->get_normal();

            //texture
            gpu.textures.bind(NORMAL_TEXTURE_INDEX, normal.texture);

            gpu.set_uniform("normal.texture", NORMAL_TEXTURE_INDEX);

            //specular
            auto& specular = material->get_specular();

            gpu.textures.bind(SPECULAR_TEXTURE_INDEX, specular.texture);

            gpu.set_uniform("specular.texture", SPECULAR_TEXTURE_INDEX);
            gpu.set_uniform("specular.color", specular.color);
            gpu.set_uniform("specular.intensity", specular.intensity);

            //emissive
            const auto& emissive = material->get_emissive();

            //texture
            gpu.textures.bind(EMISSIVE_TEXTURE_INDEX, emissive.texture);

            gpu.set_uniform("emissive.texture", EMISSIVE_TEXTURE_INDEX);
            gpu.set_uniform("emissive.color", emissive.color);
            gpu.set_uniform("emissive.intensity", emissive.intensity);
        }

        gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLES, index_count, index_buffer_type::DATA_TYPE, 0);

        //unbind textures
        gpu.textures.unbind(EMISSIVE_TEXTURE_INDEX);
        gpu.textures.unbind(SPECULAR_TEXTURE_INDEX);
        gpu.textures.unbind(NORMAL_TEXTURE_INDEX);
        gpu.textures.unbind(DIFFUSE_TEXTURE_INDEX);
    }
}
