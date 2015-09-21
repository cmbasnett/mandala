#version 400

uniform vec4 color;

in vec4 out_color;

out vec4 fragment;

void main() 
{
    fragment = out_color * color;
}  