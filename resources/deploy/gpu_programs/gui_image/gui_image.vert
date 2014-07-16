{
	"type":"vertex",
	"source":"#version 150

precision lowp float;

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec2 position;
in vec2 texcoord;

out vec2 out_texcoord;
out vec4 out_color;

void main() 
{
	out_texcoord = texcoord;
	
	gl_Position = (view_projection_matrix) * (world_matrix * vec4(position, 0, 1));
}"
}