#version 400

precision lowp float;

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;
uniform float line_height;
uniform float base;
uniform vec4 color_top;
uniform vec4 color_bottom;
uniform int should_invert_rgb;

in vec2 location;
in vec2 texcoord;

out vec2 out_texcoord;
out vec4 out_color;

void main() 
{
    gl_Position = (view_projection_matrix) * (world_matrix * vec4(location, 0, 1));
    
    out_texcoord = texcoord;
    
    float t = (location.y + (line_height - base)) / line_height;
    out_color = mix(color_bottom, color_top, t);

    //TODO: convert to color modifier subroutine
    if (should_invert_rgb == 1)
    {
        out_color.rgb = vec3(1) - out_color.rgb;
    }
}