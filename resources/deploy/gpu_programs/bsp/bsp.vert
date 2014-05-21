{
	"type":"vertex",
	"source":"#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

precision lowp float;

uniform mat4 world;
uniform mat4 view_projection;
uniform sampler2D diffuse_texture;
uniform sampler2D lightmap_texture;

in vec3 position;
//in vec3 normal;
in vec2 diffuse_texcoord;
in vec2 lightmap_texcoord;

//out vec3 out_normal;
out vec2 out_diffuse_texcoord;
out vec2 out_lightmap_texcoord;

void main() 
{
	gl_Position = (view_projection) * (world * vec4(position, 1));
	
	//out_normal = normal;
	out_diffuse_texcoord = diffuse_texcoord;
	out_lightmap_texcoord = lightmap_texcoord;
}"
}