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
#include "opengl.hpp"
#include "model.hpp"
#include "app.hpp"
#include "md5b.hpp"
#include "material.hpp"
#include "texture.hpp"
#include "gpu_program.hpp"
#include "gpu.hpp"

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
            mesh->material = app.resources.get<material_t>(hash_t(material_name.string()));	//TODO: don't resort to string concat

			//vertices
			std::vector<mesh_t::vertex_t> vertices;
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
#ifdef _DEBUG
			size_t bad_face_count = 0;
#endif //_DEBUG
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

				if(t0 == vec2_t(0) || t1 == vec2_t(0) || denominator == 0.0f)
				{
#ifdef _DEBUG
					++bad_face_count;
#endif //_DEBUG
					continue;
				}

				denominator = 1.0f / denominator;

				vec3_t tangent;
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

				if (vertex.tangent != vec3_t(0))
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
		//blending
		GLboolean blend;
		glGetBooleanv(GL_BLEND, &blend);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		std::shared_ptr<gpu_program_t> gpu_program;

		for(auto& mesh : meshes)
		{
			if(mesh->material->gpu_program != gpu_program)
			{
				gpu_program = mesh->material->gpu_program;

                const auto world_matrix_location = glGetUniformLocation(gpu_program->id, "world_matrix");
                const auto normal_matrix_location = glGetUniformLocation(gpu_program->id, "normal_matrix");
                const auto view_projection_matrix_location = glGetUniformLocation(gpu_program->id, "view_projection_matrix");
                const auto bone_matrices_location = glGetUniformLocation(gpu_program->id, "bone_matrices");
                const auto light_position_location = glGetUniformLocation(gpu_program->id, "light_position");
                const auto camera_position_location = glGetUniformLocation(gpu_program->id, "camera_position");

				gpu.programs.push(gpu_program);

				//world matrix
				if (world_matrix_location != -1)
				{
					glUniformMatrix4fv(world_matrix_location, 1, GL_FALSE, glm::value_ptr(world_matrix));
				}

				//view projection matrix
				if (view_projection_matrix_location != -1)
				{
					glUniformMatrix4fv(view_projection_matrix_location, 1, GL_FALSE, glm::value_ptr(view_projection_matrix));
				}
		
				//normal matrix
				if (normal_matrix_location != -1)
				{
					auto normal_matrix = glm::inverseTranspose(glm::mat3(world_matrix));
					glUniformMatrix3fv(normal_matrix_location, 1, false, glm::value_ptr(normal_matrix));
				}

				//bone matrices
				if (bone_matrices_location != -1)
				{
					glUniformMatrix4fv(bone_matrices_location, static_cast<GLsizei>(bone_matrices.size()), GL_FALSE, (float*)(bone_matrices.data()));
				}

				//light position
				if (light_position_location != -1)
				{
					glUniform3fv(light_position_location, 1, glm::value_ptr(light_position));
				}

				//camera position
				if (camera_position_location != -1)
				{
					glUniform3fv(camera_position_location, 1, glm::value_ptr(camera_position));
				}

				//cull face
				if(mesh->material->is_two_sided)
				{
					glDisable(GL_CULL_FACE);
				}
				else
				{
					glEnable(GL_CULL_FACE);
                }
            }

            mesh->render(world_matrix, view_projection_matrix, bone_matrices);
        }

        gpu.programs.pop();
	}

	void model_t::mesh_t::render(const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const std::vector<mat4_t>& bone_matrices) const
	{
        static const auto vertex_size = sizeof(mesh_t::vertex_type);
        static const auto position_offset = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, position));
        static const auto normal_offset = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, normal));
        static const auto tangent_offset = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, tangent));
        static const auto texcoord_offset = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, texcoord));
        static const auto bone_indices_0_offset = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, bone_indices_0));
        static const auto bone_indices_1_offset = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, bone_indices_1));
        static const auto bone_weights_0_offset = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, bone_weights_0));
        static const auto bone_weights_1_offset = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, bone_weights_1));

		auto program = material->gpu_program->id;

		//attribute locations
        const auto position_location = glGetAttribLocation(program, "position");
        const auto normal_location = glGetAttribLocation(program, "normal");
        const auto tangent_location = glGetAttribLocation(program, "tangent");
        const auto texcoord_location = glGetAttribLocation(program, "texcoord");
        const auto bone_indices_0_location = glGetAttribLocation(program, "bone_indices_0");
        const auto bone_indices_1_location = glGetAttribLocation(program, "bone_indices_1");
        const auto bone_weights_0_location = glGetAttribLocation(program, "bone_weights_0");
        const auto bone_weights_1_location = glGetAttribLocation(program, "bone_weights_1");
        const auto diffuse_texture_location = glGetUniformLocation(program, "diffuse.texture");
        const auto diffuse_color_location = glGetUniformLocation(program, "diffuse.color");
        const auto normal_texture_location = glGetUniformLocation(program, "normal.texture");
        const auto specular_texture_location = glGetUniformLocation(program, "specular.texture");
        const auto specular_color_location = glGetUniformLocation(program, "specular.color");
        const auto specular_intensity_location = glGetUniformLocation(program, "specular.intensity");
        const auto emissive_texture_location = glGetUniformLocation(program, "emissive.texture");
        const auto emissive_color_location = glGetUniformLocation(program, "emissive.color");
        const auto emissive_intensity_location = glGetUniformLocation(program, "emissive.intensity");

        static const auto diffuse_texture_index = 0;
        static const auto normal_texture_index = 1;
        static const auto specular_texture_index = 2;
        static const auto emissive_texture_index = 3;

		//bind buffers
        gpu.buffers.push(gpu_t::buffer_target_e::array, vertex_buffer);
        gpu.buffers.push(gpu_t::buffer_target_e::element_array, index_buffer);

        glEnableVertexAttribArray(position_location); glCheckError();
        glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, vertex_size, position_offset); glCheckError();

        if (normal_location != -1)
        {
            glEnableVertexAttribArray(normal_location); glCheckError();
            glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, vertex_size, normal_offset); glCheckError();
        }

        if (tangent_location != -1)
        {
            glEnableVertexAttribArray(tangent_location); glCheckError();
            glVertexAttribPointer(tangent_location, 3, GL_FLOAT, GL_FALSE, vertex_size, tangent_offset); glCheckError();
        }

        glEnableVertexAttribArray(texcoord_location); glCheckError();
        glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, GL_FALSE, vertex_size, texcoord_offset); glCheckError();

        glEnableVertexAttribArray(bone_indices_0_location); glCheckError();
        glVertexAttribIPointer(bone_indices_0_location, 4, GL_INT, vertex_size, bone_indices_0_offset); glCheckError();

        glEnableVertexAttribArray(bone_indices_1_location); glCheckError();
        glVertexAttribIPointer(bone_indices_1_location, 4, GL_INT, vertex_size, bone_indices_1_offset); glCheckError();

        glEnableVertexAttribArray(bone_weights_0_location); glCheckError();
        glVertexAttribPointer(bone_weights_0_location, 4, GL_FLOAT, GL_FALSE, vertex_size, bone_weights_0_offset); glCheckError();

        glEnableVertexAttribArray(bone_weights_1_location); glCheckError();
        glVertexAttribPointer(bone_weights_1_location, 4, GL_FLOAT, GL_FALSE, vertex_size, bone_weights_1_offset); glCheckError();

		//material
		if(material.get() != nullptr)
		{
			//diffuse
			{
				auto& diffuse = material->diffuse;

				//texture
                gpu.textures.bind(diffuse_texture_index, diffuse.texture);

                glUniform1i(diffuse_texture_location, diffuse_texture_index); glCheckError();

				//color
                glUniform4fv(diffuse_color_location, 1, glm::value_ptr(diffuse.color)); glCheckError();
			}

			//normal
			{
				auto& normal = material->normal;

				//texture
                gpu.textures.bind(normal_texture_index, normal.texture);

                glUniform1i(normal_texture_location, normal_texture_index); glCheckError();
			}

			//specular
			{
				auto& specular = material->specular;
				
				//texture
                gpu.textures.bind(specular_texture_index, specular.texture);

                glUniform1i(specular_texture_location, specular_texture_index); glCheckError();

				//color
                glUniform4fv(specular_color_location, 1, glm::value_ptr(specular.color)); glCheckError();

				//intensity
                glUniform1f(specular_intensity_location, specular.intensity); glCheckError();
			}

			//emissive
			{
				auto& emissive = material->emissive;

				//texture
                gpu.textures.bind(emissive_texture_index, emissive.texture);

                glUniform1i(emissive_texture_location, emissive_texture_index); glCheckError();

				//color
                glUniform4fv(emissive_color_location, 1, glm::value_ptr(emissive.color)); glCheckError();

				//intensity
                glUniform1f(emissive_intensity_location, emissive.intensity); glCheckError();
			}
		}

        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT, 0); glCheckError();

        glDisableVertexAttribArray(position_location); glCheckError();

        if (normal_location != -1)
        {
            glDisableVertexAttribArray(normal_location); glCheckError();
        }

        if (tangent_location != -1)
        {
            glDisableVertexAttribArray(tangent_location); glCheckError();
        }

        glDisableVertexAttribArray(texcoord_location); glCheckError();
        glDisableVertexAttribArray(bone_indices_0_location); glCheckError();
        glDisableVertexAttribArray(bone_indices_1_location); glCheckError();
        glDisableVertexAttribArray(bone_weights_0_location); glCheckError();
        glDisableVertexAttribArray(bone_weights_1_location); glCheckError();

		//unbind textures
        gpu.textures.unbind(emissive_texture_index);
        gpu.textures.unbind(specular_texture_index);
        gpu.textures.unbind(normal_texture_index);
        gpu.textures.unbind(diffuse_texture_index);

		//unbind buffers
        gpu.buffers.pop(gpu_t::buffer_target_e::array);
        gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
	}
};
