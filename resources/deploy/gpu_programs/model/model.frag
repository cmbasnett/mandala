{
	"type":"fragment",
	"source":"#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

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

uniform diffuse_t diffuse;
uniform normal_t normal;
uniform specular_t specular;
uniform emissive_t emissive;

in vec3 out_position;
in vec2 out_texcoord;
in vec3 out_light_vector;
in vec3 out_normal;
in vec3 out_view_direction;

void main()
{
	float rim_power = 6.0;
	vec3 rim_color = vec3(0.0);

	vec3 ambient_color = vec3(0.125);
	vec3 normal_ = 2.0 * texture(normal.texture, out_texcoord).rgb - 1.0;
	
	vec3 albedo_color = texture2D(diffuse.texture, out_texcoord).rgb * diffuse.color.rgb;
	
	vec3 light_direction = normalize(-out_light_vector);
	
	float nxDir = max(0.0, dot(normal_, light_direction));
	
	vec3 diffuse_color = vec3(nxDir);
	
	float specular_power = 0.0;
	
	if(nxDir != 0.0)
	{
		vec3 half_vector = normalize(out_light_vector + out_view_direction);
		float nxHalf = max(0.0, dot(normal_, half_vector));
		
		specular_power = pow(nxHalf, specular.intensity);
	}
	
	float rim_term = 1.0 - clamp(dot(out_view_direction, out_normal), 0.0, 1.0);
	
	vec3 specular_color = texture(specular.texture, out_texcoord).rgb * specular.color.rgb * specular_power;
	vec3 emissive_color = texture(emissive.texture, out_texcoord).rgb * emissive.color.rgb * emissive.intensity;
	rim_color = rim_color * pow(rim_term, rim_power);
	
	gl_FragColor = vec4((ambient_color + diffuse_color) * albedo_color + emissive_color + rim_color, 1.0);
}"
}