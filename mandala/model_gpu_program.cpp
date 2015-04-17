//mandala
#include "model_gpu_program.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
    std::string model_gpu_program_t::vertex_shader_source = R"(
#version 400

uniform mat4 world_matrix;
uniform mat3 normal_matrix;
uniform mat4 view_projection_matrix;
uniform mat4 bone_matrices[128];
uniform vec3 light_location;
uniform vec3 camera_location;

in vec3 location;
in vec3 normal;
in vec3 tangent;
in vec2 texcoord;
in ivec4 bone_indices_0;
in ivec4 bone_indices_1;
in vec4 bone_weights_0;
in vec4 bone_weights_1;

out vec2 out_texcoord;
out vec3 out_location;
out vec3 out_light_vector;
out vec3 out_normal;
out vec3 out_view_direction;

mat4 create_bone_transform()
{
	mat4 bone_transform = mat4(0.0);
	
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
	
	vec4 world_location = world_matrix * bone_transform * vec4(location, 1.0);
	
	out_location = world_location.xyz;
	
	out_normal = normal;
	
	vec3 world_normal = normal_matrix * mat3(bone_transform) * normal;
	vec3 world_tangent = normal_matrix * mat3(bone_transform) * tangent;
	vec3 world_binormal = cross(world_normal, world_tangent);
	
	mat3 tbn_matrix = mat3(world_tangent, world_binormal, world_normal);
	
	out_light_vector = (world_location.xyz - light_location) * tbn_matrix;
	
	vec4 final_location = view_projection_matrix * world_location;
	
	out_view_direction = normalize(camera_location - out_location);
	
	gl_Position = final_location;	
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

in vec3 out_location;
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
		location_location = gpu.get_attribute_location(get_id(), "location");
		normal_location = gpu.get_attribute_location(get_id(), "normal");
		tangent_location = gpu.get_attribute_location(get_id(), "tangent");
		texcoord_location = gpu.get_attribute_location(get_id(), "texcoord");
		bone_indices_0_location = gpu.get_attribute_location(get_id(), "bone_indices_0");
		bone_indices_1_location = gpu.get_attribute_location(get_id(), "bone_indices_1");
		bone_weights_0_location = gpu.get_attribute_location(get_id(), "bone_weights_0");
		bone_weights_1_location = gpu.get_attribute_location(get_id(), "bone_weights_1");

        calculate_lighting_function_location = gpu.get_subroutine_uniform_location(get_id(), gpu_t::shader_type_e::fragment, "calculate_lighting");
        calculate_lighting_lit_subroutine_index = gpu.get_subroutine_index(get_id(), gpu_t::shader_type_e::fragment, "calculate_lighting_lit");
        calculate_lighting_unlit_subroutine_index = gpu.get_subroutine_index(get_id(), gpu_t::shader_type_e::fragment, "calculate_lighting_unlit");
	}

	void model_gpu_program_t::on_bind()
	{
		gpu.enable_vertex_attribute_array(location_location);
		gpu.enable_vertex_attribute_array(normal_location);
		gpu.enable_vertex_attribute_array(tangent_location);
		gpu.enable_vertex_attribute_array(texcoord_location);
		gpu.enable_vertex_attribute_array(bone_indices_0_location);
		gpu.enable_vertex_attribute_array(bone_indices_1_location);
		gpu.enable_vertex_attribute_array(bone_weights_0_location);
		gpu.enable_vertex_attribute_array(bone_weights_1_location);

		gpu.set_vertex_attrib_pointer(location_location, 3, gpu_data_type_e::float_, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, location)));
		gpu.set_vertex_attrib_pointer(normal_location, 3, gpu_data_type_e::float_, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, normal)));
		gpu.set_vertex_attrib_pointer(tangent_location, 3, gpu_data_type_e::float_, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, tangent)));
		gpu.set_vertex_attrib_pointer(texcoord_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, texcoord)));
		gpu.set_vertex_attrib_pointer(bone_indices_0_location, 4, gpu_data_type_e::int_, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, bone_indices_0)));
		gpu.set_vertex_attrib_pointer(bone_indices_1_location, 4, gpu_data_type_e::int_, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, bone_indices_1)));
		gpu.set_vertex_attrib_pointer(bone_weights_0_location, 4, gpu_data_type_e::float_, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, bone_weights_0)));
		gpu.set_vertex_attrib_pointer(bone_weights_1_location, 4, gpu_data_type_e::float_, false, sizeof(vertex_type), reinterpret_cast<void*>(offsetof(vertex_type, bone_weights_1)));
	}

	void model_gpu_program_t::on_unbind()
	{
		gpu.disable_vertex_attribute_array(location_location);
		gpu.disable_vertex_attribute_array(normal_location);
		gpu.disable_vertex_attribute_array(tangent_location);
		gpu.disable_vertex_attribute_array(texcoord_location);
		gpu.disable_vertex_attribute_array(bone_indices_0_location);
		gpu.disable_vertex_attribute_array(bone_indices_1_location);
		gpu.disable_vertex_attribute_array(bone_weights_0_location);
		gpu.disable_vertex_attribute_array(bone_weights_1_location);
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
