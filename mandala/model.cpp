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

namespace mandala
{
	model_t::model_t(std::istream& istream)
	{
		//magic
		char magic[md5b::magic_length + 1];
		memset(magic, '\0', md5b::magic_length + 1);
		istream.read(magic, md5b::magic_length);

		if(strcmp(md5b::model_magic, magic) != 0) 
		{
			throw std::exception();
		}

		//version
		int32_t version = 0;
        istream.read(reinterpret_cast<char*>(&version), sizeof(version));

		if(version != md5b::model_version)
		{
			throw std::exception();
		}

		//bone count
		uint32_t bone_count;
        istream.read(reinterpret_cast<char*>(&bone_count), sizeof(bone_count));

		//bones
		std::vector<bone_info_t> bone_infos;

		bone_infos.resize(bone_count);

		for(uint8_t i = 0; i < bone_infos.size(); ++i)
		{
			auto& bone_info = bone_infos[i];

			std::getline(istream, bone_info.name, '\0');

			bone_indices.insert(std::make_pair(hash_t(bone_info.name), i));

            istream.read(reinterpret_cast<char*>(&bone_info.parent_index), sizeof(bone_info.parent_index));
            istream.read(reinterpret_cast<char*>(&bone_info.position.x), sizeof(bone_info.position.x));
            istream.read(reinterpret_cast<char*>(&bone_info.position.y), sizeof(bone_info.position.y));
            istream.read(reinterpret_cast<char*>(&bone_info.position.z), sizeof(bone_info.position.z));
            istream.read(reinterpret_cast<char*>(&bone_info.orientation.x), sizeof(bone_info.orientation.x));
            istream.read(reinterpret_cast<char*>(&bone_info.orientation.y), sizeof(bone_info.orientation.y));
            istream.read(reinterpret_cast<char*>(&bone_info.orientation.z), sizeof(bone_info.orientation.z));

			md5b::compute_quaternion_w(bone_info.orientation);
		}

		//mesh count
		uint8_t mesh_count = 0;
        istream.read(reinterpret_cast<char*>(&mesh_count), sizeof(mesh_count));

		std::vector<mesh_info_t> mesh_infos;
		mesh_infos.resize(mesh_count);

		for(auto& mesh : mesh_infos)
		{
			//shader
			std::getline(istream, mesh.shader, '\0');

			//vertex count
			uint32_t vertex_count;
            istream.read(reinterpret_cast<char*>(&vertex_count), sizeof(vertex_count));

			//vertices
			mesh.vertices.resize(vertex_count);

			for(auto& vertex : mesh.vertices)
			{
                istream.read(reinterpret_cast<char*>(&vertex.texcoord.x), sizeof(vertex.texcoord.x));
                istream.read(reinterpret_cast<char*>(&vertex.texcoord.y), sizeof(vertex.texcoord.y));
                istream.read(reinterpret_cast<char*>(&vertex.weight_index_start), sizeof(vertex.weight_index_start));
                istream.read(reinterpret_cast<char*>(&vertex.weight_count), sizeof(vertex.weight_count));
			}

			//index count
			uint32_t index_count; 
            istream.read(reinterpret_cast<char*>(&index_count), sizeof(index_count));

			//indices
			mesh.indices.resize(index_count);
            istream.read(reinterpret_cast<char*>(&mesh.indices[0]), sizeof(mesh.indices[0]) * mesh.indices.size());

			//weight count
			uint32_t weight_count;
            istream.read(reinterpret_cast<char*>(&weight_count), sizeof(weight_count));

			//weights
			mesh.weights.resize(weight_count);

			for(auto& weight : mesh.weights)
			{
                istream.read(reinterpret_cast<char*>(&weight.bone_index), sizeof(weight.bone_index));
                istream.read(reinterpret_cast<char*>(&weight.bias), sizeof(weight.bias));
                istream.read(reinterpret_cast<char*>(&weight.position), sizeof(weight.position));
			}
		}

		//meshes
		meshes.reserve(mesh_infos.size());

		for(auto& mesh_info : mesh_infos)
		{
			auto mesh = std::make_shared<mesh_t>();

			//index count
            mesh->index_count = mesh_info.indices.size();

            boost::filesystem::path shader_path(mesh_info.shader);

            auto material_name = shader_path.filename().replace_extension(".mat");

			//material
            mesh->material = resources.get<material_t>(hash_t(material_name.string()));	//TODO: don't resort to string concat

			//vertices
			std::vector<mesh_t::vertex_type> vertices;
			vertices.resize(mesh_info.vertices.size());

			for(size_t j = 0; j < mesh_info.vertices.size(); ++j)
			{
				const auto& vertex_info = mesh_info.vertices[j];
				auto& vertex = vertices[j];

				vertex.texcoord = vertex_info.texcoord;

				for(int k = 0; k < vertex_info.weight_count && k < 8; ++k)
				{
					const auto& weight = mesh_info.weights[vertex_info.weight_index_start + k];
					const auto& bone = bone_infos[weight.bone_index];

					if(k < 4)
					{
						vertex.bone_weights_0[k] = weight.bias;
						vertex.bone_indices_0[k] = weight.bone_index;
					}
					else
					{
						vertex.bone_weights_1[k % 4] = weight.bias;
						vertex.bone_indices_1[k % 4] = weight.bone_index;
					}

					auto rotated_position = bone.orientation * weight.position;
					vertex.position += (bone.position + rotated_position) * weight.bias;
				}
			}

#if defined(DEBUG)
			size_t bad_face_count = 0;
#endif

			//normals, tangents & binormals
			for(size_t j = 0; j < mesh_info.indices.size();)
			{
				auto& vertex_0 = vertices[mesh_info.indices[j++]];
				auto& vertex_1 = vertices[mesh_info.indices[j++]];
				auto& vertex_2 = vertices[mesh_info.indices[j++]];

                auto v0 = vertex_1.position - vertex_0.position;
                auto v1 = vertex_2.position - vertex_0.position;
				
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
				tangent.x = denominator * ((v0.x * t1.y)  + (v1.x * -t0.y));
				tangent.y = denominator * ((v0.y * t1.y)  + (v1.y * -t0.y));
				tangent.z = denominator * ((v0.z * t1.y)  + (v1.z * -t0.y));
				tangent = glm::normalize(tangent);

				vertex_0.tangent += tangent;
				vertex_1.tangent += tangent;
				vertex_2.tangent += tangent;
			}

			for(auto& vertex : vertices)
			{
				vertex.normal = glm::normalize(vertex.normal);

				if (vertex.tangent != mesh_t::vertex_type::normal_type(0))
				{
					vertex.tangent = glm::normalize(vertex.tangent);
				}
			}

            mesh->vertex_buffer = std::make_shared<mesh_t::vertex_buffer_type>();
            mesh->vertex_buffer->data(vertices, gpu_t::buffer_usage_e::static_draw);

            mesh->index_buffer = std::make_shared<mesh_t::index_buffer_type>();
            mesh->index_buffer->data(mesh_info.indices, gpu_t::buffer_usage_e::static_draw);

			meshes.push_back(mesh);
		}

		//bind pose bone matrices
		bones.resize(bone_count);

		for(uint8_t i = 0; i < bone_count; ++i)
		{
			const auto& bone_info = bone_infos[i];
			auto& bone = bones[i];

			bone.bind_pose_matrix = glm::translate(bone_info.position) * glm::toMat4(bone_info.orientation);
			bone.inverse_bind_pose_matrix = glm::inverse(bone.bind_pose_matrix);
		}
	}
	
	void model_t::render(const vec3_t& camera_position, const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const std::vector<mat4_t>& bone_matrices, const vec3_t& light_position) const
	{
		//blend
		auto blend_state = gpu.blend.get_state();
		blend_state.is_enabled = true;
		blend_state.src_factor = gpu_t::blend_factor_e::src_alpha;
		blend_state.dst_factor = gpu_t::blend_factor_e::one_minus_src_alpha;

		gpu.blend.push_state(blend_state);

		//depth
        auto depth_state = gpu.depth.get_state();
		depth_state.should_test = true;

		gpu.depth.push_state(depth_state);

		for(auto& mesh : meshes)
		{
            //culling
            auto culling_state = gpu.culling.get_state();

            culling_state.is_enabled = !mesh->material->is_two_sided;

            gpu.culling.push_state(culling_state);

			gpu.buffers.push(gpu_t::buffer_target_e::array, mesh->vertex_buffer);
			gpu.buffers.push(gpu_t::buffer_target_e::element_array, mesh->index_buffer);

            const auto gpu_program = gpu_programs.get<model_gpu_program_t>();

            gpu.programs.push(gpu_program);

            gpu_program->world_matrix(world_matrix);
            gpu_program->view_projection_matrix(view_projection_matrix);
            gpu_program->normal_matrix(glm::inverseTranspose(glm::mat3(world_matrix)));
            gpu_program->bone_matrices(bone_matrices);
            gpu_program->light_position(light_position);
            gpu_program->camera_position(camera_position);
            gpu_program->is_lit(mesh->material->is_lit);

            mesh->render(world_matrix, view_projection_matrix, bone_matrices);

			gpu.programs.pop();

			gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
			gpu.buffers.pop(gpu_t::buffer_target_e::array);

            gpu.culling.pop_state();
        }

        gpu.depth.pop_state();

        gpu.blend.pop_state();
	}

	void model_t::mesh_t::render(const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const std::vector<mat4_t>& bone_matrices) const
	{
		static const auto diffuse_texture_index = 0;
		static const auto normal_texture_index = 1;
		static const auto specular_texture_index = 2;
        static const auto emissive_texture_index = 3;

        const auto gpu_program = gpu_programs.get<model_gpu_program_t>();

		////cull face
		//if(mesh->material->is_two_sided)
		//{
		//	glDisable(GL_CULL_FACE); glCheckError();
		//}
		//else
		//{
		//	glEnable(GL_CULL_FACE); glCheckError();
		//}

		//material
		if(material != nullptr)
		{
			//diffuse
			const auto& diffuse = material->diffuse;

			//texture
            gpu.textures.bind(diffuse_texture_index, diffuse.texture);

            gpu_program->diffuse_texture_index(diffuse_texture_index);
            gpu_program->diffuse_color(diffuse.color);

			//normal
			const auto& normal = material->normal;

			//texture
            gpu.textures.bind(normal_texture_index, normal.texture);

            gpu_program->normal_texture_index(normal_texture_index);

			//specular
			auto& specular = material->specular;
				
            gpu.textures.bind(specular_texture_index, specular.texture);

            gpu_program->specular_texture_index(specular_texture_index);
            gpu_program->specular_color(specular.color);
            gpu_program->specular_intensity(specular.intensity);

			//emissive
			auto& emissive = material->emissive;

			//texture
            gpu.textures.bind(emissive_texture_index, emissive.texture);

            gpu_program->emissive_texture_index(emissive_texture_index);
            gpu_program->emissive_color(emissive.color);
            gpu_program->emissive_intensity(emissive.intensity);
		}

		gpu.draw_elements(gpu_t::primitive_type_e::triangles, index_count, gpu_t::index_type_e::unsigned_short, 0);

		//unbind textures
		gpu.textures.unbind(emissive_texture_index);
		gpu.textures.unbind(specular_texture_index);
		gpu.textures.unbind(normal_texture_index);
		gpu.textures.unbind(diffuse_texture_index);
	}
}
