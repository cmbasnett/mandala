#version 400

uniform sampler2D diffuse_texture;
uniform vec4 color;
uniform vec2 region_min;
uniform vec2 region_max;
uniform vec2 frame_min;
uniform vec2 frame_max;
uniform vec2 texcoord_origin;
uniform vec2 texcoord_scale;
uniform float rotation;

in vec2 out_texcoord;

out vec4 fragment;

void main()
{
    // rotation
    mat2 rotation_matrix = mat2(cos(rotation), -sin(rotation), sin(rotation), cos(rotation));
    vec2 texcoord = out_texcoord * texcoord_scale;
    texcoord -= texcoord_origin;
    texcoord = rotation_matrix * texcoord;
    texcoord += texcoord_origin;

    // tiling
    texcoord.x = mod(texcoord.x, 1.0);
    texcoord.y = mod(texcoord.y, 1.0);

    // move texcoord from frame space to texture space
    texcoord = frame_min + (texcoord * (frame_max - frame_min));

    // if texcoord outside of region, discard fragment
    if (texcoord.x < region_min.x ||
        texcoord.x > region_max.x ||
        texcoord.y < region_min.y ||
        texcoord.y > region_max.y)
    {
        discard;
    }

    fragment = texture2D(diffuse_texture, texcoord) * color;
}