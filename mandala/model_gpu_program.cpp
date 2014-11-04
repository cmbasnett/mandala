//mandala
#include "model_gpu_program.hpp"
#include "opengl.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
	std::string model_gpu_program_t::vertex_shader_source = R"(
#version 400

precision highp float;

uniform mat4 world_matrix;
uniform mat3 normal_matrix;
uniform mat4 view_projection_matrix;
uniform mat4 bone_matrices[128];
uniform vec3 light_position;
uniform vec3 camera_position;

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 texcoord;
in ivec4 bone_indices_0;
in ivec4 bone_indices_1;
in vec4 bone_weights_0;
in vec4 bone_weights_1;

out vec2 out_texcoord;
out vec3 out_position;
out vec3 out_light_vector;
out vec3 out_normal;
out vec3 out_view_direction;

mat4 create_bone_transform()
{
	mat4 bone_transform;
	
	bone_transform += bone_matrices[bone_indices_0[0]] * bone_weights_0[0];
	bone_transform += bone_matrices[bone_indices_0[1]] * bone_weights_0[1];
	bone_transform += bone_matrices[bone_indices_0[2]] * bone_weights_0[2];
	bone_transform += bone_matrices[bone_indices_0[3]] * bone_weights_0[3];
	bone_transform += bone_matrices[bone_indices_1[0]] * bone_weights_1[0];
	bone_transform += bone_matrices[bone_indices_1[1]] * bone_weights_1[1];
	bone_transform += bone_matrices[bone_indices_1[2]] * bone_weights_1[2];
	bone_transform += bone_matrices[bone_indices_1[3]] * bone_weights_1[3];
	
	return bone_transform;
}

void main() 
{
	out_texcoord = texcoord;
	
	mat4 bone_transform = create_bone_transform();
	
	vec4 world_position = world_matrix * bone_transform * vec4(position, 1.0);
	
	out_position = world_position.xyz;
	
	out_normal = normal;
	
	vec3 world_normal = normal_matrix * mat3(bone_transform) * normal;
	vec3 world_tangent = normal_matrix * mat3(bone_transform) * tangent;
	vec3 world_binormal = cross(world_normal, world_tangent);
	
	mat3 tbn_matrix = mat3(world_tangent, world_binormal, world_normal);
	
	out_light_vector = (world_position.xyz - light_position) * tbn_matrix;
	
	vec4 final_position = view_projection_matrix * world_position;
	
	out_view_direction = normalize(camera_position - out_position);
	
	gl_Position = final_position;	
}
)";

std::string model_gpu_program_t::fragment_shader_source = R"(
#version 400

struct diffuse_t
{
	sampler2D texture;
	vec4 color;
};

struct normal_t
{
	sampler2D texture;
};

struct specular_t
{
	sampler2D texture;
	vec4 color;
	float intensity;
};

struct emissive_t
{
	sampler2D texture;
	vec4 color;
	float intensity;
};

struct light_t
{
    vec3 ambient_color;
};

uniform diffuse_t diffuse;
uniform normal_t normal;
uniform specular_t specular;
uniform emissive_t emissive;
uniform light_t light;

in vec3 out_position;
in vec2 out_texcoord;
in vec3 out_light_vector;
in vec3 out_normal;
in vec3 out_view_direction;

out vec4 frag_color;

subroutine vec3 calculate_lighting();

subroutine (calculate_lighting) vec3 calculate_lighting_lit()
{
	vec3 normal_ = 2.0 * texture(normal.texture, out_texcoord).rgb - 1.0;
	vec3 light_direction = normalize(-out_light_vector);
	float normal_dot_light_direction = max(0.0, dot(normal_, light_direction));
    float specular_power = 0.0;

    if(normal_dot_light_direction != 0.0)
	{
		vec3 half_vector = normalize(out_light_vector + out_view_direction);
		float normal_dot_half_vector = max(0.0, dot(normal_, half_vector));
		
		specular_power = pow(normal_dot_half_vector, specular.intensity);
	}
	
	vec3 specular_color = texture(specular.texture, out_texcoord).rgb * specular.color.rgb * specular_power;

    return vec3(normal_dot_light_direction) * specular_color;
}

subroutine (calculate_lighting) vec3 calculate_lighting_unlit()
{
    return vec3(1.0);
}

subroutine uniform calculate_lighting calculate_lighting_function;

void main()
{
	vec3 albedo_color = texture2D(diffuse.texture, out_texcoord).rgb * diffuse.color.rgb;
	vec3 diffuse_color = calculate_lighting_function();
	vec3 emissive_color = texture(emissive.texture, out_texcoord).rgb * emissive.color.rgb * emissive.intensity;
	
	frag_color = vec4((light.ambient_color + diffuse_color) * albedo_color + emissive_color, 1.0);
}
)";

	model_gpu_program_t::model_gpu_program_t() :
		gpu_program_t(vertex_shader_source, fragment_shader_source)
	{
		position_location = glGetAttribLocation(id, "position"); glCheckError();
		normal_location = glGetAttribLocation(id, "normal"); glCheckError();
		tangent_location = glGetAttribLocation(id, "tangent"); glCheckError();
		texcoord_location = glGetAttribLocation(id, "texcoord"); glCheckError();
		bone_indices_0_location = glGetAttribLocation(id, "bone_indices_0"); glCheckError();
		bone_indices_1_location = glGetAttribLocation(id, "bone_indices_1"); glCheckError();
		bone_weights_0_location = glGetAttribLocation(id, "bone_weights_0"); glCheckError();
		bone_weights_1_location = glGetAttribLocation(id, "bone_weights_1"); glCheckError();

		world_matrix_location = glGetUniformLocation(id, "world_matrix"); glCheckError();
		normal_matrix_location = glGetUniformLocation(id, "normal_matrix"); glCheckError();
		view_projection_matrix_location = glGetUniformLocation(id, "view_projection_matrix"); glCheckError();
		bone_matrices_location = glGetUniformLocation(id, "bone_matrices"); glCheckError();
		light_position_location = glGetUniformLocation(id, "light_position"); glCheckError();
		camera_position_location = glGetUniformLocation(id, "camera_position"); glCheckError();
		diffuse_texture_index_location = glGetUniformLocation(id, "diffuse.texture"); glCheckError();
		diffuse_color_location = glGetUniformLocation(id, "diffuse.color"); glCheckError();
		normal_texture_index_location = glGetUniformLocation(id, "normal.texture"); glCheckError();
		specular_texture_index_location = glGetUniformLocation(id, "specular.texture"); glCheckError();
		specular_color_location = glGetUniformLocation(id, "specular.color"); glCheckError();
		specular_intensity_location = glGetUniformLocation(id, "specular.intensity"); glCheckError();
		emissive_texture_index_location = glGetUniformLocation(id, "emissive.texture"); glCheckError();
		emissive_color_location = glGetUniformLocation(id, "emissive.color"); glCheckError();
		emissive_intensity_location = glGetUniformLocation(id, "emissive.intensity"); glCheckError();
        calculate_lighting_function_location = glGetSubroutineUniformLocation(id, GL_FRAGMENT_SHADER, "calculate_lighting");
        calculate_lighting_lit_subroutine_index = glGetSubroutineIndex(id, GL_FRAGMENT_SHADER, "calculate_lighting_lit"); glCheckError();
        calculate_lighting_unlit_subroutine_index = glGetSubroutineIndex(id, GL_FRAGMENT_SHADER, "calculate_lighting_unlit"); glCheckError();
	}

	void model_gpu_program_t::on_bind()
	{
		glEnableVertexAttribArray(position_location); glCheckError();
		glEnableVertexAttribArray(normal_location); glCheckError();
		glEnableVertexAttribArray(tangent_location); glCheckError();
		glEnableVertexAttribArray(texcoord_location); glCheckError();
		glEnableVertexAttribArray(bone_indices_0_location); glCheckError();
		glEnableVertexAttribArray(bone_indices_1_location); glCheckError();
		glEnableVertexAttribArray(bone_weights_0_location); glCheckError();
		glEnableVertexAttribArray(bone_weights_1_location); glCheckError();

		static const auto position_offset = reinterpret_cast<GLvoid*>(offsetof(vertex_type, position));
		static const auto normal_offset = reinterpret_cast<GLvoid*>(offsetof(vertex_type, normal));
		static const auto tangent_offset = reinterpret_cast<GLvoid*>(offsetof(vertex_type, tangent));
		static const auto texcoord_offset = reinterpret_cast<GLvoid*>(offsetof(vertex_type, texcoord));
		static const auto bone_indices_0_offset = reinterpret_cast<GLvoid*>(offsetof(vertex_type, bone_indices_0));
		static const auto bone_indices_1_offset = reinterpret_cast<GLvoid*>(offsetof(vertex_type, bone_indices_1));
		static const auto bone_weights_0_offset = reinterpret_cast<GLvoid*>(offsetof(vertex_type, bone_weights_0));
		static const auto bone_weights_1_offset = reinterpret_cast<GLvoid*>(offsetof(vertex_type, bone_weights_1));

		glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_type), position_offset); glCheckError();
		glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_type), normal_offset); glCheckError();
		glVertexAttribPointer(tangent_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_type), tangent_offset); glCheckError();
		glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_type), texcoord_offset); glCheckError();
		glVertexAttribIPointer(bone_indices_0_location, 4, GL_INT, sizeof(vertex_type), bone_indices_0_offset); glCheckError();
		glVertexAttribIPointer(bone_indices_1_location, 4, GL_INT, sizeof(vertex_type), bone_indices_1_offset); glCheckError();
		glVertexAttribPointer(bone_weights_0_location, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_type), bone_weights_0_offset); glCheckError();
		glVertexAttribPointer(bone_weights_1_location, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_type), bone_weights_1_offset); glCheckError();
	}

	void model_gpu_program_t::on_unbind()
	{
		glDisableVertexAttribArray(position_location); glCheckError();
		glDisableVertexAttribArray(normal_location); glCheckError();
		glDisableVertexAttribArray(tangent_location); glCheckError();
		glDisableVertexAttribArray(texcoord_location); glCheckError();
		glDisableVertexAttribArray(bone_indices_0_location); glCheckError();
		glDisableVertexAttribArray(bone_indices_1_location); glCheckError();
		glDisableVertexAttribArray(bone_weights_0_location); glCheckError();
		glDisableVertexAttribArray(bone_weights_1_location); glCheckError();
	}

	void model_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
	{
		glUniformMatrix4fv(world_matrix_location, 1, GL_FALSE, glm::value_ptr(world_matrix)); glCheckError();
	}

	void model_gpu_program_t::normal_matrix(const mat3_t& normal_matrix) const
	{
		glUniformMatrix3fv(normal_matrix_location, 1, GL_FALSE, glm::value_ptr(normal_matrix)); glCheckError();
	}

	void model_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
	{
		glUniformMatrix4fv(view_projection_matrix_location, 1, GL_FALSE, glm::value_ptr(view_projection_matrix)); glCheckError();
	}

	void model_gpu_program_t::bone_matrices(const std::vector<mat4_t>& bone_matrices) const
	{
		glUniformMatrix4fv(bone_matrices_location, static_cast<GLsizei>(bone_matrices.size()), GL_FALSE, (GLfloat*)(bone_matrices.data())); glCheckError();
	}

	void model_gpu_program_t::light_position(const vec3_t& light_position) const
	{
		glUniform3fv(light_position_location, 1, glm::value_ptr(light_position)); glCheckError();
	}

	void model_gpu_program_t::camera_position(const vec3_t& camera_position) const
	{
		glUniform3fv(camera_position_location, 1, glm::value_ptr(camera_position)); glCheckError();
	}

	void model_gpu_program_t::diffuse_texture_index(uint32_t diffuse_texture_index) const
	{
		glUniform1i(diffuse_texture_index_location, diffuse_texture_index); glCheckError();
	}

	void model_gpu_program_t::diffuse_color(const vec4_t& diffuse_color) const
	{
		glUniform4fv(diffuse_color_location, 1, glm::value_ptr(diffuse_color)); glCheckError();
	}

	void model_gpu_program_t::normal_texture_index(uint32_t normal_texture_index) const
	{
		glUniform1i(normal_texture_index_location, normal_texture_index); glCheckError();
	}

	void model_gpu_program_t::specular_texture_index(uint32_t specular_texture_index) const
	{
		glUniform1i(specular_texture_index_location, specular_texture_index); glCheckError();
	}

	void model_gpu_program_t::specular_color(const vec4_t& specular_color) const
	{
		glUniform4fv(specular_color_location, 1, glm::value_ptr(specular_color)); glCheckError();
	}

	void model_gpu_program_t::specular_intensity(float32_t specular_intensity) const
	{
		glUniform1f(specular_intensity_location, specular_intensity); glCheckError();
	}

	void model_gpu_program_t::emissive_texture_index(uint32_t emissive_texture_index) const
	{
		glUniform1i(emissive_texture_index_location, emissive_texture_index); glCheckError();
	}

	void model_gpu_program_t::emissive_color(const vec4_t& emissive_color) const
	{
		glUniform4fv(emissive_color_location, 1, glm::value_ptr(emissive_color)); glCheckError();
	}

	void model_gpu_program_t::emissive_intensity(float32_t emissive_intensity) const
	{
		glUniform1f(emissive_intensity_location, emissive_intensity); glCheckError();
	}

    void model_gpu_program_t::is_lit(bool is_lit) const
    {
        if (is_lit)
        {
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &calculate_lighting_lit_subroutine_index);
        }
        else
        {
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &calculate_lighting_unlit_subroutine_index);
        }
    }
}
