#version 450

struct Diffuse
{
    sampler2D texture;
    vec4 color;
};

struct Normal
{
    sampler2D texture;
};

struct Specular
{
    sampler2D texture;
    vec4 color;
    float intensity;
};

struct Emissive
{
    sampler2D texture;
    vec4 color;
    float intensity;
};

struct Light
{
    vec3 ambient_color;
};

uniform Diffuse diffuse;
uniform Normal normal;
uniform Specular specular;
uniform Emissive emissive;
uniform Light light;

in vec3 out_location;
in vec2 out_texcoord;
in vec3 out_light_vector;
in vec3 out_normal;
in vec3 out_view_direction;
in vec4 out_bone_weights;

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
    
    frag_color = vec4((light.ambient_color + diffuse_color) * albedo_color * out_bone_weights[0] + emissive_color, 1.0);
}