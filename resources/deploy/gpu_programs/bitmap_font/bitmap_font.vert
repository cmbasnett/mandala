{
	"type":"vertex",
	"source":"#version 150
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable

precision lowp float;

uniform mat4 world;
uniform mat4 view_projection;
uniform float line_height;
uniform float base;
uniform vec4 color_top;
uniform vec4 color_bottom;

in vec2 position;
in vec2 texcoord;

out vec2 out_texcoord;
out vec4 out_color;

void main() 
{
	gl_Position = (view_projection) * (world * vec4(position, 0, 1));
	
	out_texcoord = texcoord;
	
	float t = (position.y + (line_height - base)) / line_height;
	out_color = mix(color_bottom, color_top, t);
}"
}