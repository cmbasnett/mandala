//mandala
#include "model_gpu_program.hpp"

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
		position_location = gpu.get_attribute_location(id(), "position");
		normal_location = gpu.get_attribute_location(id(), "normal");
		tangent_location = gpu.get_attribute_location(id(), "tangent");
		texcoord_location = gpu.get_attribute_location(id(), "texcoord");
		bone_indices_0_location = gpu.get_attribute_location(id(), "bone_indices_0");
		bone_indices_1_location = gpu.get_attribute_location(id(), "bone_indices_1");
		bone_weights_0_location = gpu.get_attribute_location(id(), "bone_weights_0");
		bone_weights_1_location = gpu.get_attribute_location(id(), "bone_weights_1");

		world_matrix_location = gpu.get_uniform_location(id(), "world_matrix");
		normal_matrix_location = gpu.get_uniform_location(id(), "normal_matrix");
		view_projection_matrix_location = gpu.get_uniform_location(id(), "view_projection_matrix");
		bone_matrices_location = gpu.get_uniform_location(id(), "bone_matrices");
		light_position_location = gpu.get_uniform_location(id(), "light_position");
		camera_position_location = gpu.get_uniform_location(id(), "camera_position");
		diffuse_texture_index_location = gpu.get_uniform_location(id(), "diffuse.texture");
		diffuse_color_location = gpu.get_uniform_location(id(), "diffuse.color");
		normal_texture_index_location = gpu.get_uniform_location(id(), "normal.texture");
		specular_texture_index_location = gpu.get_uniform_location(id(), "specular.texture");
		specular_color_location = gpu.get_uniform_location(id(), "specular.color");
		specular_intensity_location = gpu.get_uniform_location(id(), "specular.intensity");
		emissive_texture_index_location = gpu.get_uniform_location(id(), "emissive.texture");
		emissive_color_location = gpu.get_uniform_location(id(), "emissive.color");
		emissive_intensity_location = gpu.get_uniform_location(id(), "emissive.intensity");
        calculate_lighting_function_location = gpu.get_subroutine_uniform_location(id(), gpu_t::shader_type_e::fragment, "calculate_lighting");
        calculate_lighting_lit_subroutine_index = gpu.get_subroutine_index(id(), gpu_t::shader_type_e::fragment, "calculate_lighting_lit");
        calculate_lighting_unlit_subroutine_index = gpu.get_subroutine_index(id(), gpu_t::shader_type_e::fragment, "calculate_lighting_unlit");
	}

	void model_gpu_program_t::on_bind()
	{
		gpu.enable_vertex_attribute_array(position_location);
		gpu.enable_vertex_attribute_array(normal_location);
		gpu.enable_vertex_attribute_array(tangent_location);
		gpu.enable_vertex_attribute_array(texcoord_location);
		gpu.enable_vertex_attribute_array(bone_indices_0_location);
		gpu.enable_vertex_attribute_array(bone_indices_1_location);
		gpu.enable_vertex_attribute_array(bone_weights_0_location);
		gpu.enable_vertex_attribute_array(bone_weights_1_location);

		static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_type, position));
		static const auto normal_offset = reinterpret_cast<void*>(offsetof(vertex_type, normal));
		static const auto tangent_offset = reinterpret_cast<void*>(offsetof(vertex_type, tangent));
		static const auto texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_type, texcoord));
		static const auto bone_indices_0_offset = reinterpret_cast<void*>(offsetof(vertex_type, bone_indices_0));
		static const auto bone_indices_1_offset = reinterpret_cast<void*>(offsetof(vertex_type, bone_indices_1));
		static const auto bone_weights_0_offset = reinterpret_cast<void*>(offsetof(vertex_type, bone_weights_0));
		static const auto bone_weights_1_offset = reinterpret_cast<void*>(offsetof(vertex_type, bone_weights_1));

		gpu.set_vertex_attrib_pointer(position_location, 3, gpu_t::data_type_e::float_, false, sizeof(vertex_type), position_offset);
		gpu.set_vertex_attrib_pointer(normal_location, 3, gpu_t::data_type_e::float_, false, sizeof(vertex_type), normal_offset);
		gpu.set_vertex_attrib_pointer(tangent_location, 3, gpu_t::data_type_e::float_, false, sizeof(vertex_type), tangent_offset);
		gpu.set_vertex_attrib_pointer(texcoord_location, 2, gpu_t::data_type_e::float_, false, sizeof(vertex_type), texcoord_offset);
		gpu.set_vertex_attrib_pointer(bone_indices_0_location, 4, gpu_t::data_type_e::int_, sizeof(vertex_type), bone_indices_0_offset);
		gpu.set_vertex_attrib_pointer(bone_indices_1_location, 4, gpu_t::data_type_e::int_, sizeof(vertex_type), bone_indices_1_offset);
		gpu.set_vertex_attrib_pointer(bone_weights_0_location, 4, gpu_t::data_type_e::float_, false, sizeof(vertex_type), bone_weights_0_offset);
		gpu.set_vertex_attrib_pointer(bone_weights_1_location, 4, gpu_t::data_type_e::float_, false, sizeof(vertex_type), bone_weights_1_offset);
	}

	void model_gpu_program_t::on_unbind()
	{
		gpu.disable_vertex_attribute_array(position_location);
		gpu.disable_vertex_attribute_array(normal_location);
		gpu.disable_vertex_attribute_array(tangent_location);
		gpu.disable_vertex_attribute_array(texcoord_location);
		gpu.disable_vertex_attribute_array(bone_indices_0_location);
		gpu.disable_vertex_attribute_array(bone_indices_1_location);
		gpu.disable_vertex_attribute_array(bone_weights_0_location);
		gpu.disable_vertex_attribute_array(bone_weights_1_location);
	}

	void model_gpu_program_t::world_matrix(const mat4_t& world_matrix) const
	{
		gpu.set_uniform(world_matrix_location, world_matrix, false);
	}

	void model_gpu_program_t::normal_matrix(const mat3_t& normal_matrix) const
	{
		gpu.set_uniform(normal_matrix_location, normal_matrix, false);
	}

	void model_gpu_program_t::view_projection_matrix(const mat4_t& view_projection_matrix) const
	{
		gpu.set_uniform(view_projection_matrix_location, view_projection_matrix, false);
	}

	void model_gpu_program_t::bone_matrices(const std::vector<mat4_t>& bone_matrices) const
	{
		gpu.set_uniform(bone_matrices_location, bone_matrices, false);
	}

	void model_gpu_program_t::light_position(const vec3_t& light_position) const
	{
		gpu.set_uniform(light_position_location, light_position);
	}

	void model_gpu_program_t::camera_position(const vec3_t& camera_position) const
	{
		gpu.set_uniform(camera_position_location, camera_position);
	}

	void model_gpu_program_t::diffuse_texture_index(uint32_t diffuse_texture_index) const
	{
		gpu.set_uniform(diffuse_texture_index_location, diffuse_texture_index);
	}

	void model_gpu_program_t::diffuse_color(const vec4_t& diffuse_color) const
	{
		gpu.set_uniform(diffuse_color_location, diffuse_color);
	}

	void model_gpu_program_t::normal_texture_index(uint32_t normal_texture_index) const
	{
		gpu.set_uniform(normal_texture_index_location, normal_texture_index);
	}

	void model_gpu_program_t::specular_texture_index(uint32_t specular_texture_index) const
	{
		gpu.set_uniform(specular_texture_index_location, specular_texture_index);
	}

	void model_gpu_program_t::specular_color(const vec4_t& specular_color) const
	{
		gpu.set_uniform(specular_color_location, specular_color);
	}

	void model_gpu_program_t::specular_intensity(float32_t specular_intensity) const
	{
		gpu.set_uniform(specular_intensity_location, specular_intensity);
	}

	void model_gpu_program_t::emissive_texture_index(uint32_t emissive_texture_index) const
	{
		gpu.set_uniform(emissive_texture_index_location, emissive_texture_index);
	}

	void model_gpu_program_t::emissive_color(const vec4_t& emissive_color) const
	{
		gpu.set_uniform(emissive_color_location, emissive_color);
	}

	void model_gpu_program_t::emissive_intensity(float32_t emissive_intensity) const
	{
		gpu.set_uniform(emissive_intensity_location, emissive_intensity);
	}

    void model_gpu_program_t::is_lit(bool is_lit) const
    {
        if (is_lit)
        {
            gpu.set_uniform_subroutine(gpu_t::shader_type_e::fragment, calculate_lighting_lit_subroutine_index);
        }
        else
        {
			gpu.set_uniform_subroutine(gpu_t::shader_type_e::fragment, calculate_lighting_unlit_subroutine_index);
        }
    }
}
