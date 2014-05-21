//std
#include <fstream>

//glm
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtx\norm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>	

//mandala
#include "model.h"
#include "app.h"
#include "md5b.h"
#include "material.h"
#include "texture.h"
#include "gpu_program.h"

namespace mandala
{
	model_t::model_t(std::istream& istream)
	{
		//magic
        char magic[md5b::magic_length + 1];
        memset(magic, '\0', md5b::magic_length + 1);
        istream.read(magic, md5b::magic_length);

		if(strcmp(md5b::magic, magic) != 0)
		{
			throw std::exception();
		}

        istream.read(magic, md5b::magic_length);

		if(strcmp(md5b::model_magic, magic) != 0)
		{
			throw std::exception();
		}

		//version
		int32_t version = 0;
		istream.read((char*)&version, sizeof(version));

		if(version != md5b::model_version)
		{
			throw std::exception();
		}

		//bone count
		uint8_t bone_count;
		istream.read((char*)&bone_count, sizeof(bone_count));

		//bones
		struct bone_info_t
		{
			std::string name;
			uint8_t parent_index;
			vec3_t position;
			quat_t orientation;
		};

		std::vector<bone_info_t> bone_infos;

		bone_infos.resize(bone_count);

		for(uint8_t i = 0; i < bone_infos.size(); ++i)
		{
			auto& bone_info = bone_infos[i];

			std::getline(istream, bone_info.name, '\0');

			bone_indices.insert(std::make_pair(hash_t(bone_info.name), i));

			istream.read((char*)&bone_info.parent_index, sizeof(bone_info.parent_index));
			istream.read((char*)&bone_info.position, sizeof(bone_info.position));
			istream.read((char*)&bone_info.orientation.x, sizeof(bone_info.orientation.x));
			istream.read((char*)&bone_info.orientation.y, sizeof(bone_info.orientation.y));
			istream.read((char*)&bone_info.orientation.z, sizeof(bone_info.orientation.z));

			md5b::compute_quaternion_w(bone_info.orientation);
		}

		//mesh count
		uint8_t mesh_count = 0;
		istream.read((char*)&mesh_count, sizeof(mesh_count));

		//meshes
		struct mesh_info_t
		{
			struct vertex_t
			{
				vec2_t texcoord;
				uint16_t weight_index_start;
				uint8_t weight_count;
			};

			struct weight_t
			{
				uint8_t bone_index;
				float32_t bias;
				vec3_t position;
			};

			std::string shader;
			std::vector<vertex_t> vertices;
			std::vector<uint16_t> indices;
			std::vector<weight_t> weights;
		};

		std::vector<mesh_info_t> mesh_infos;
		mesh_infos.resize(mesh_count);

		for(mesh_info_t& mesh : mesh_infos)
		{
			//shader
			std::getline(istream, mesh.shader, '\0');

			//vertex count
			uint16_t vertex_count;
			istream.read((char*)&vertex_count, sizeof(vertex_count));

			//vertices
			mesh.vertices.resize(vertex_count);

			for(mesh_info_t::vertex_t& vertex : mesh.vertices)
			{
				istream.read((char*)&vertex.texcoord, sizeof(vertex.texcoord));
				istream.read((char*)&vertex.weight_index_start, sizeof(vertex.weight_index_start));
				istream.read((char*)&vertex.weight_count, sizeof(vertex.weight_count));
			}

			//index count
			uint16_t index_count;
			istream.read((char*)&index_count, sizeof(index_count));

			//indices
			mesh.indices.resize(index_count);
			istream.read((char*)&mesh.indices[0], sizeof(mesh.indices[0]) * mesh.indices.size());

			//weight count
			uint16_t weight_count;
			istream.read((char*)&weight_count, sizeof(weight_count));

			//weights
			mesh.weights.resize(weight_count);

			for(mesh_info_t::weight_t& weight : mesh.weights)
			{
				istream.read((char*)&weight.bone_index, sizeof(weight.bone_index));
				istream.read((char*)&weight.bias, sizeof(weight.bias));
				istream.read((char*)&weight.position, sizeof(weight.position));
			}
		}

		//meshes
		meshes.reserve(mesh_infos.size());

		for(mesh_info_t& mesh_info : mesh_infos)
		{
			auto mesh = std::make_shared<mesh_t>();

			//index count
			mesh->index_count = mesh_info.indices.size();

			//material
			mesh->material = app.resources.get<material_t>(hash_t(mesh_info.shader + ".mat"));	//TODO: don't resort to string concat

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

					vec3_t rotated_position = bone.orientation * weight.position;
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

				vec3_t v0 = vertex_1.position - vertex_0.position;
				vec3_t v1 = vertex_2.position - vertex_0.position;
				
				//normal
				vec3_t normal = glm::cross(v0, v1);
				normal = glm::normalize(normal);

				vertex_0.normal += normal;
				vertex_1.normal += normal;
				vertex_2.normal += normal;

				//tangent
				vec2_t t0 = vertex_2.texcoord - vertex_0.texcoord;
				vec2_t t1 = vertex_1.texcoord - vertex_0.texcoord;

				float32_t denominator = (t0.x * t1.y) - (t1.x * t0.y);

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

			glGenBuffers(1, &mesh->vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(mesh_t::vertex_t) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &mesh->index_buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t)* mesh->index_count, mesh_info.indices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

				const GLint WORLD_MATRIX_LOCATION = glGetUniformLocation(gpu_program->program, "world_matrix");
				const GLint NORMAL_MATRIX_LOCATION = glGetUniformLocation(gpu_program->program, "normal_matrix");
				const GLint VIEW_PROJECTION_MATRIX_LOCATION = glGetUniformLocation(gpu_program->program, "view_projection_matrix");
				const GLint BONE_MATRICES_LOCATION = glGetUniformLocation(gpu_program->program, "bone_matrices");
				const GLint LIGHT_POSITION_LOCATION = glGetUniformLocation(gpu_program->program, "light_position");
				const GLint CAMERA_POSITION_LOCATION = glGetUniformLocation(gpu_program->program, "camera_position");

				glUseProgram(gpu_program->program);

				//world matrix
				if (WORLD_MATRIX_LOCATION != -1)
				{
					glUniformMatrix4fv(WORLD_MATRIX_LOCATION, 1, GL_FALSE, glm::value_ptr(world_matrix));
				}

				//view projection matrix
				if (VIEW_PROJECTION_MATRIX_LOCATION != -1)
				{
					glUniformMatrix4fv(VIEW_PROJECTION_MATRIX_LOCATION, 1, GL_FALSE, glm::value_ptr(view_projection_matrix));
				}
		
				//normal matrix
				if (NORMAL_MATRIX_LOCATION != -1)
				{
					auto normal_matrix = glm::inverseTranspose(glm::mat3(world_matrix));
					glUniformMatrix3fv(NORMAL_MATRIX_LOCATION, 1, false, glm::value_ptr(normal_matrix));
				}

				//bone matrices
				if (BONE_MATRICES_LOCATION != -1)
				{
					glUniformMatrix4fv(BONE_MATRICES_LOCATION, bone_matrices.size(), GL_FALSE, (float*)(bone_matrices.data()));
				}

				//light position
				if (LIGHT_POSITION_LOCATION != -1)
				{
					glUniform3fv(LIGHT_POSITION_LOCATION, 1, glm::value_ptr(light_position));
				}

				//camera position
				if (CAMERA_POSITION_LOCATION != -1)
				{
					glUniform3fv(CAMERA_POSITION_LOCATION, 1, glm::value_ptr(camera_position));
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
	}

	void model_t::mesh_t::render(const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const std::vector<mat4_t>& bone_matrices) const
	{
		static const GLsizei VERTEX_SIZE = sizeof(mesh_t::vertex_t);
		static const GLvoid* POSITION_OFFSET = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, position));
		static const GLvoid* NORMAL_OFFSET = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, normal));
		static const GLvoid* TANGENT_OFFSET = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, tangent));
		static const GLvoid* TEXCOORD_OFFSET = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, texcoord));
		static const GLvoid* BONE_INDICES_0_OFFSET = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, bone_indices_0));
		static const GLvoid* BONE_INDICES_1_OFFSET = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, bone_indices_1));
		static const GLvoid* BONE_WEIGHTS_0_OFFSET = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, bone_weights_0));
		static const GLvoid* BONE_WEIGHTS_1_OFFSET = reinterpret_cast<GLvoid*>(offsetof(mesh_t::vertex_t, bone_weights_1));

		auto program = material->gpu_program->program;

		//attribute locations
		const GLint POSITION_LOCATION = glGetAttribLocation(program, "position");
		const GLint NORMAL_LOCATION = glGetAttribLocation(program, "normal");
		const GLint TANGENT_LOCATION = glGetAttribLocation(program, "tangent");
		const GLint TEXCOORD_LOCATION = glGetAttribLocation(program, "texcoord");
		const GLint BONE_INDICES_0_LOCATION = glGetAttribLocation(program, "bone_indices_0");
		const GLint BONE_INDICES_1_LOCATION = glGetAttribLocation(program, "bone_indices_1");
		const GLint BONE_WEIGHTS_0_LOCATION = glGetAttribLocation(program, "bone_weights_0");
		const GLint BONE_WEIGHTS_1_LOCATION = glGetAttribLocation(program, "bone_weights_1");
		const GLint DIFFUSE_MAP_LOCATION = glGetUniformLocation(program, "diffuse.texture");
		const GLint DIFFUSE_COLOR_LOCATION = glGetUniformLocation(program, "diffuse.color");
		const GLint NORMAL_MAP_LOCATION = glGetUniformLocation(program, "normal.texture");
		const GLint SPECULAR_MAP_LOCATION = glGetUniformLocation(program, "specular.texture");
		const GLint SPECULAR_COLOR_LOCATION = glGetUniformLocation(program, "specular.color");
		const GLint SPECULAR_INTENSITY_LOCATION = glGetUniformLocation(program, "specular.intensity");
		const GLint EMISSIVE_MAP_LOCATION = glGetUniformLocation(program, "emissive.texture");
		const GLint EMISSIVE_COLOR_LOCATION = glGetUniformLocation(program, "emissive.color");
		const GLint EMISSIVE_INTENSITY_LOCATION = glGetUniformLocation(program, "emissive.intensity");

		//bind buffers
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

		//position
		if (POSITION_LOCATION != -1)
		{
			glEnableVertexAttribArray(POSITION_LOCATION);
			glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, false, VERTEX_SIZE, POSITION_OFFSET);
		}

		//normal
		if (NORMAL_LOCATION != -1)
		{
			glEnableVertexAttribArray(NORMAL_LOCATION);
			glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, false, VERTEX_SIZE, NORMAL_OFFSET);
		}

		//tangent
		if (TANGENT_LOCATION != -1)
		{
			glEnableVertexAttribArray(TANGENT_LOCATION);
			glVertexAttribPointer(TANGENT_LOCATION, 3, GL_FLOAT, false, VERTEX_SIZE, TANGENT_OFFSET);
		}

		//texcoord
		if (TEXCOORD_LOCATION != -1)
		{
			glEnableVertexAttribArray(TEXCOORD_LOCATION);
			glVertexAttribPointer(TEXCOORD_LOCATION, 2, GL_FLOAT, false, VERTEX_SIZE, TEXCOORD_OFFSET);
		}

		//bone indices
		if (BONE_INDICES_0_LOCATION != -1)
		{
			glEnableVertexAttribArray(BONE_INDICES_0_LOCATION);
			glVertexAttribIPointer(BONE_INDICES_0_LOCATION, 4, GL_INT, VERTEX_SIZE, BONE_INDICES_0_OFFSET);
		}

		if (BONE_INDICES_1_LOCATION != -1)
		{
			glEnableVertexAttribArray(BONE_INDICES_1_LOCATION);
			glVertexAttribIPointer(BONE_INDICES_1_LOCATION, 4, GL_INT, VERTEX_SIZE, BONE_INDICES_1_OFFSET);
		}

		//bone weights
		if (BONE_WEIGHTS_0_LOCATION != -1)
		{
			glEnableVertexAttribArray(BONE_WEIGHTS_0_LOCATION);
			glVertexAttribPointer(BONE_WEIGHTS_0_LOCATION, 4, GL_FLOAT, false, VERTEX_SIZE, BONE_WEIGHTS_0_OFFSET);
		}

		if (BONE_WEIGHTS_1_LOCATION != -1)
		{
			glEnableVertexAttribArray(BONE_WEIGHTS_1_LOCATION);
			glVertexAttribPointer(BONE_WEIGHTS_1_LOCATION, 4, GL_FLOAT, false, VERTEX_SIZE, BONE_WEIGHTS_1_OFFSET);
		}
		
		//material
		if(material.get() != nullptr)
		{
			//diffuse
			{
				auto& diffuse = material->diffuse;

				//texture
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, diffuse.texture != nullptr ? diffuse.texture->handle : 0);
				glUniform1i(DIFFUSE_MAP_LOCATION, 0);

				//color
				glUniform4fv(DIFFUSE_COLOR_LOCATION, 1, glm::value_ptr(diffuse.color));
			}

			//normal
			{
				auto& normal = material->normal;

				//texture
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, normal.texture != nullptr ? normal.texture->handle : 0);
				glUniform1i(NORMAL_MAP_LOCATION, 1);
			}

			//specular
			{
				auto& specular = material->specular;
				
				//texture
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, specular.texture != nullptr ? specular.texture->handle : 0);
				glUniform1i(SPECULAR_MAP_LOCATION, 2);

				//color
				glUniform4fv(SPECULAR_COLOR_LOCATION, 1, glm::value_ptr(specular.color));

				//intensity
				glUniform1f(SPECULAR_INTENSITY_LOCATION, specular.intensity);
			}

			//emissive
			{
				auto& emissive = material->emissive;

				//texture
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, emissive.texture != nullptr ? emissive.texture->handle : 0);
				glUniform1i(EMISSIVE_MAP_LOCATION, 3);

				//color
				glUniform4fv(EMISSIVE_COLOR_LOCATION, 1, glm::value_ptr(emissive.color));

				//intensity
				glUniform1f(EMISSIVE_INTENSITY_LOCATION, emissive.intensity);
			}
		}

		glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_SHORT, 0);

		//disable vertex attributes
		if (POSITION_LOCATION >= 0)
		{
			glDisableVertexAttribArray(POSITION_LOCATION);
		}

		if (NORMAL_LOCATION >= 0)
		{
			glDisableVertexAttribArray(NORMAL_LOCATION);
		}

		if (TEXCOORD_LOCATION >= 0)
		{
			glDisableVertexAttribArray(TEXCOORD_LOCATION);
		}

		if (BONE_INDICES_0_LOCATION >= 0)
		{
			glDisableVertexAttribArray(BONE_INDICES_0_LOCATION);
		}

		if (BONE_INDICES_1_LOCATION >= 0)
		{
			glDisableVertexAttribArray(BONE_INDICES_1_LOCATION);
		}

		if (BONE_WEIGHTS_0_LOCATION >= 0)
		{
			glDisableVertexAttribArray(BONE_WEIGHTS_0_LOCATION);
		}

		if (BONE_WEIGHTS_1_LOCATION >= 0)
		{
			glDisableVertexAttribArray(BONE_WEIGHTS_1_LOCATION);
		}
		
		//unbind textures
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		//unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
};
