//mandala
#include "gpu_programs.hpp"

namespace mandala
{
    const char* basic_gpu_program_t::vertex_shader_source = R"(
#version 150

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 position;
in vec4 color;

out vec4 out_color;

void main() 
{
	gl_Position = view_projection_matrix * (world_matrix * vec4(position, 1));

    	out_color = color;
})";

    const char* basic_gpu_program_t::fragment_shader_source = R"(
#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

in vec4 out_color;

out vec4 fragment;

void main() 
{
	fragment = out_color;
})";

    basic_gpu_program_t::basic_gpu_program_t() :
        gpu_program_t(vertex_shader_source, fragment_shader_source)
    {
        position_location = gpu.get_attribute_location(get_id(), "position");
        color_location = gpu.get_attribute_location(get_id(), "color");

        world_matrix_location = gpu.get_uniform_location(get_id(), "world_matrix");
        view_projection_matrix_location = gpu.get_uniform_location(get_id(), "view_projection_matrix");
    }

    void basic_gpu_program_t::on_bind()
    {
        static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_type, position));
        static const auto color_offset = reinterpret_cast<void*>(offsetof(vertex_type, color));

        gpu.enable_vertex_attribute_array(position_location);
        gpu.enable_vertex_attribute_array(color_location);

        gpu.set_vertex_attrib_pointer(position_location, 3, gpu_data_type_e::float_, false, sizeof(vertex_type), position_offset);
        gpu.set_vertex_attrib_pointer(color_location, 4, gpu_data_type_e::float_, false, sizeof(vertex_type), color_offset);
    }

    void basic_gpu_program_t::on_unbind()
    {
        gpu.disable_vertex_attribute_array(position_location);
        gpu.disable_vertex_attribute_array(color_location);
    }

    const char* bitmap_font_gpu_program_t::vertex_shader_source = R"(
#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

precision lowp float;

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;
uniform float line_height;
uniform float base;
uniform vec4 color_top;
uniform vec4 color_bottom;
uniform int should_invert_rgb;

in vec2 position;
in vec2 texcoord;

out vec2 out_texcoord;
out vec4 out_color;

void main() 
{
	gl_Position = (view_projection_matrix) * (world_matrix * vec4(position, 0, 1));
	
	out_texcoord = texcoord;
	
	float t = (position.y + (line_height - base)) / line_height;
	out_color = mix(color_bottom, color_top, t);

        //TODO: convert to color modifier subroutine
    if (should_invert_rgb == 1)
    {
        out_color.rgb = vec3(1) - out_color.rgb;
    }
}
)";

    const char* bitmap_font_gpu_program_t::fragment_shader_source = R"(
#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

uniform sampler2D diffuse_texture;

in vec2 out_texcoord;
in vec4 out_color;

void main() 
{
	gl_FragColor = texture2D(diffuse_texture, out_texcoord) * out_color;
}
)";

    bitmap_font_gpu_program_t::bitmap_font_gpu_program_t() :
        gpu_program_t(vertex_shader_source, fragment_shader_source)
    {
        position_location = gpu.get_attribute_location(get_id(), "position");
        texcoord_location = gpu.get_attribute_location(get_id(), "texcoord");

        world_matrix_location = gpu.get_uniform_location(get_id(), "world_matrix");
        view_projection_matrix_location = gpu.get_uniform_location(get_id(), "view_projection_matrix");
        font_line_height_location = gpu.get_uniform_location(get_id(), "line_height");
        font_base_location = gpu.get_uniform_location(get_id(), "base");
        font_color_top_location = gpu.get_uniform_location(get_id(), "color_top");
        font_color_bottom_location = gpu.get_uniform_location(get_id(), "color_bottom");
        diffuse_texture_location = gpu.get_uniform_location(get_id(), "diffuse_texture");
        should_invert_rgb_location = gpu.get_uniform_location(get_id(), "should_invert_rgb");
    }

    void bitmap_font_gpu_program_t::on_bind()
    {
        static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_type, position));
        static const auto texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_type, texcoord));

        gpu.enable_vertex_attribute_array(position_location);
        gpu.enable_vertex_attribute_array(texcoord_location);

        gpu.set_vertex_attrib_pointer(position_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), position_offset);
        gpu.set_vertex_attrib_pointer(texcoord_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), texcoord_offset);
    }

    void bitmap_font_gpu_program_t::on_unbind()
    {
        gpu.disable_vertex_attribute_array(position_location);
        gpu.disable_vertex_attribute_array(texcoord_location);
    }

    const char* bsp_gpu_program_t::vertex_shader_source = R"(
#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

precision lowp float;

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;
uniform sampler2D diffuse_texture;
uniform sampler2D lightmap_texture;

in vec3 position;
in vec2 diffuse_texcoord;
in vec2 lightmap_texcoord;

out vec2 out_diffuse_texcoord;
out vec2 out_lightmap_texcoord;

void main() 
{
	gl_Position = view_projection_matrix * (world_matrix * vec4(position, 1));
	
	//out_normal = normal;
	out_diffuse_texcoord = diffuse_texcoord;
	out_lightmap_texcoord = lightmap_texcoord;
})";

    const char* bsp_gpu_program_t::fragment_shader_source = R"(
#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

uniform sampler2D diffuse_texture;
uniform sampler2D lightmap_texture;
uniform float lightmap_gamma;
uniform float alpha;
uniform bool should_test_alpha;

in vec3 out_normal;
in vec2 out_diffuse_texcoord;
in vec2 out_lightmap_texcoord;

out vec4 fragment;

void main() 
{
	vec4 lightmap_term = texture(lightmap_texture, out_lightmap_texcoord);
	lightmap_term.xyz *= lightmap_gamma;
	
	vec4 diffuse_term = texture(diffuse_texture, out_diffuse_texcoord);
	
	fragment = diffuse_term * lightmap_term;
	
	fragment.a *= alpha;
	
	if(should_test_alpha && fragment.a <= 0.25)
	{
	    discard;
	}
})";

    bsp_gpu_program_t::bsp_gpu_program_t() :
        gpu_program_t(vertex_shader_source, fragment_shader_source)
    {
        position_location = gpu.get_attribute_location(get_id(), "position");
        diffuse_texcoord_location = gpu.get_attribute_location(get_id(), "diffuse_texcoord");
        lightmap_texcoord_location = gpu.get_attribute_location(get_id(), "lightmap_texcoord");

        world_matrix_location = gpu.get_uniform_location(get_id(), "world_matrix");
        view_projection_matrix_location = gpu.get_uniform_location(get_id(), "view_projection_matrix");
        diffuse_texture_location = gpu.get_uniform_location(get_id(), "diffuse_texture");
        lightmap_texture_location = gpu.get_uniform_location(get_id(), "lightmap_texture");
        lightmap_gamma_location = gpu.get_uniform_location(get_id(), "lightmap_gamma");
        alpha_location = gpu.get_uniform_location(get_id(), "alpha");
        should_test_alpha_location = gpu.get_uniform_location(get_id(), "should_test_alpha");
    }

    void bsp_gpu_program_t::on_bind()
    {
        static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_type, position));
        static const auto diffuse_texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_type, diffuse_texcoord));
        static const auto lightmap_texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_type, lightmap_texcoord));

        gpu.enable_vertex_attribute_array(position_location);
        gpu.enable_vertex_attribute_array(diffuse_texcoord_location);
        gpu.enable_vertex_attribute_array(lightmap_texcoord_location);

        gpu.set_vertex_attrib_pointer(position_location, 3, gpu_data_type_e::float_, false, sizeof(vertex_type), position_offset);
        gpu.set_vertex_attrib_pointer(diffuse_texcoord_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), diffuse_texcoord_offset);
        gpu.set_vertex_attrib_pointer(lightmap_texcoord_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), lightmap_texcoord_offset);
    }

    void bsp_gpu_program_t::on_unbind()
    {
        gpu.disable_vertex_attribute_array(position_location);
        gpu.disable_vertex_attribute_array(diffuse_texcoord_location);
        gpu.disable_vertex_attribute_array(lightmap_texcoord_location);
    }

    const char* model_gpu_program_t::vertex_shader_source = R"(
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

    const char* model_gpu_program_t::fragment_shader_source = R"(
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
        position_location = gpu.get_attribute_location(get_id(), "position");
        normal_location = gpu.get_attribute_location(get_id(), "normal");
        tangent_location = gpu.get_attribute_location(get_id(), "tangent");
        texcoord_location = gpu.get_attribute_location(get_id(), "texcoord");
        bone_indices_0_location = gpu.get_attribute_location(get_id(), "bone_indices_0");
        bone_indices_1_location = gpu.get_attribute_location(get_id(), "bone_indices_1");
        bone_weights_0_location = gpu.get_attribute_location(get_id(), "bone_weights_0");
        bone_weights_1_location = gpu.get_attribute_location(get_id(), "bone_weights_1");

        world_matrix_location = gpu.get_uniform_location(get_id(), "world_matrix");
        normal_matrix_location = gpu.get_uniform_location(get_id(), "normal_matrix");
        view_projection_matrix_location = gpu.get_uniform_location(get_id(), "view_projection_matrix");
        bone_matrices_location = gpu.get_uniform_location(get_id(), "bone_matrices");
        light_position_location = gpu.get_uniform_location(get_id(), "light_position");
        camera_position_location = gpu.get_uniform_location(get_id(), "camera_position");
        diffuse_texture_location = gpu.get_uniform_location(get_id(), "diffuse.texture");
        diffuse_color_location = gpu.get_uniform_location(get_id(), "diffuse.color");
        normal_texture_location = gpu.get_uniform_location(get_id(), "normal.texture");
        specular_texture_location = gpu.get_uniform_location(get_id(), "specular.texture");
        specular_color_location = gpu.get_uniform_location(get_id(), "specular.color");
        specular_intensity_location = gpu.get_uniform_location(get_id(), "specular.intensity");
        emissive_texture_location = gpu.get_uniform_location(get_id(), "emissive.texture");
        emissive_color_location = gpu.get_uniform_location(get_id(), "emissive.color");
        emissive_intensity_location = gpu.get_uniform_location(get_id(), "emissive.intensity");
        calculate_lighting_subroutine_location = gpu.get_subroutine_uniform_location(get_id(), gpu_t::shader_type_e::fragment, "calculate_lighting");
        calculate_lighting_lit_subroutine_index = gpu.get_subroutine_index(get_id(), gpu_t::shader_type_e::fragment, "calculate_lighting_lit");
        calculate_lighting_unlit_subroutine_index = gpu.get_subroutine_index(get_id(), gpu_t::shader_type_e::fragment, "calculate_lighting_unlit");
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

        gpu.set_vertex_attrib_pointer(position_location, 3, gpu_data_type_e::float_, false, sizeof(vertex_type), position_offset);
        gpu.set_vertex_attrib_pointer(normal_location, 3, gpu_data_type_e::float_, false, sizeof(vertex_type), normal_offset);
        gpu.set_vertex_attrib_pointer(tangent_location, 3, gpu_data_type_e::float_, false, sizeof(vertex_type), tangent_offset);
        gpu.set_vertex_attrib_pointer(texcoord_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), texcoord_offset);
        gpu.set_vertex_attrib_pointer(bone_indices_0_location, 4, gpu_data_type_e::int_, sizeof(vertex_type), bone_indices_0_offset);
        gpu.set_vertex_attrib_pointer(bone_indices_1_location, 4, gpu_data_type_e::int_, sizeof(vertex_type), bone_indices_1_offset);
        gpu.set_vertex_attrib_pointer(bone_weights_0_location, 4, gpu_data_type_e::float_, false, sizeof(vertex_type), bone_weights_0_offset);
        gpu.set_vertex_attrib_pointer(bone_weights_1_location, 4, gpu_data_type_e::float_, false, sizeof(vertex_type), bone_weights_1_offset);
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

    const char* gui_gpu_program_t::vertex_shader_source = R"(
#version 150

precision lowp float;

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 position;
in vec2 texcoord;

out vec2 out_texcoord;

void main() 
{
	out_texcoord = texcoord;
	
	gl_Position = (view_projection_matrix) * (world_matrix * vec4(position, 1));
}
)";

    const char* gui_gpu_program_t::fragment_shader_source = R"(
#version 150

uniform sampler2D diffuse_texture;
uniform vec4 color;

in vec2 out_texcoord;

out vec4 fragment;

void main()
{
	fragment = texture2D(diffuse_texture, out_texcoord) * color;
}
)";

    gui_gpu_program_t::gui_gpu_program_t() :
        gpu_program_t(vertex_shader_source, fragment_shader_source)
    {
        position_location = gpu.get_attribute_location(get_id(), "position");
        texcoord_location = gpu.get_attribute_location(get_id(), "texcoord");

        world_matrix_location = gpu.get_uniform_location(get_id(), "world_matrix");
        view_projection_matrix_location = gpu.get_uniform_location(get_id(), "view_projection_matrix");
        diffuse_texture_location = gpu.get_uniform_location(get_id(), "diffuse_texture");
        color_location = gpu.get_uniform_location(get_id(), "color");
    }

    void gui_gpu_program_t::on_bind()
    {
        static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_type, position));
        static const auto texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_type, texcoord));

        gpu.enable_vertex_attribute_array(position_location);
        gpu.enable_vertex_attribute_array(texcoord_location);

        gpu.set_vertex_attrib_pointer(position_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), position_offset);
        gpu.set_vertex_attrib_pointer(texcoord_location, 2, gpu_data_type_e::float_, false, sizeof(vertex_type), texcoord_offset);

        //uniforms
    }

    void gui_gpu_program_t::on_unbind()
    {
        gpu.disable_vertex_attribute_array(position_location);
        gpu.disable_vertex_attribute_array(texcoord_location);
    }
}