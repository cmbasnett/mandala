{
	"type":"vertex",
	"source":"#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

precision lowp float;

struct camera_t
{
	mat4 world_matrix;
	mat4 projection_matrix;
};

uniform camera_t camera;

in vec3 position;

out vec4 out_color;

void main() 
{
	gl_Position = (camera.projection_matrix) * (camera.world_matrix * vec4(position, 1));
	
	out_color = vec4(vec3(position) / 8.0, 1);
}"
}