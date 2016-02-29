#version 400

uniform sampler2D diffuse_texture;
uniform vec4 color;

in vec4 out_color;
in vec2 out_texcoord;

out vec4 fragment;

void main()
{
    fragment = texture2D(diffuse_texture, out_texcoord) * out_color;
}
