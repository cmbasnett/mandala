#pragma once

#include "gpu_program.hpp"
#include "glm.hpp"

using namespace glm;

namespace naga
{
    struct blur_horizontal_gpu_program : GpuProgram
    {
        struct Vertex
        {
            Vertex() = default;
            Vertex(vec3 location, vec2 texcoord)
            {
                this->location = location;
                this->texcoord = texcoord;
            }

            vec3 location;
            vec2 texcoord;
        };

        typedef Vertex VertexType;

        blur_horizontal_gpu_program() :
            GpuProgram(R"(#version 150

uniform mat4 world_matrix;
uniform mat4 view_projection_matrix;

in vec3 location;
in vec2 texcoord;

out vec2 out_texcoord;

void main(void)
{
    out_texcoord = texcoord;

    gl_Position = (view_projection_matrix) * (world_matrix * vec4(location, 1));
})", R"(/*
Film Grain post-process shader v1.1    
Martins Upitis (martinsh) devlog-martinsh.blogspot.com
2013

--------------------------
This work is licensed under a Creative Commons Attribution 3.0 Unported License.
So you are free to share, modify and adapt it for your needs, and even use it for commercial use.
I would also love to hear about a project you are using it.

Have fun,
Martins
--------------------------

Perlin noise shader by toneburst:
http://machinesdontcare.wordpress.com/2009/06/25/3d-perlin-noise-sphere-vertex-shader-sourcecode/
*/

uniform sampler2D diffuse_texture; //rendered scene sampler
uniform float t;

const float permTexUnit = 1.0 / 256.0;        // Perm texture texel-size
const float permTexUnitHalf = 0.5 / 256.0;    // Half perm texture texel-size

float width = 1280.0;
float height = 720.0;

const float grainamount = 0.02; //grain amount
bool colored = false; //colored noise?
float coloramount = 0.6;
float grainsize = 2.5; //grain particle size (1.5 - 2.5)
float lumamount = 1.0; //

in vec2 out_texcoord;
    
//a random texture generator, but you can also use a pre-computed perturbation texture
vec4 rnm(in vec2 tc) 
{
    float noise =  sin(dot(tc + vec2(t,t),vec2(12.9898,78.233))) * 43758.5453;

            float noiseR =  fract(noise)*2.0-1.0;
    float noiseG =  fract(noise*1.2154)*2.0-1.0; 
    float noiseB =  fract(noise*1.3453)*2.0-1.0;
    float noiseA =  fract(noise*1.3647)*2.0-1.0;
    
    return vec4(noiseR,noiseG,noiseB,noiseA);
}

float fade(in float t) {
    return t*t*t*(t*(t*6.0-15.0)+10.0);
}

float pnoise3D(in vec3 p)
{
    vec3 pi = permTexUnit*floor(p)+permTexUnitHalf; // Integer part, scaled so +1 moves permTexUnit texel
    // and offset 1/2 texel to sample texel centers
    vec3 pf = fract(p);     // Fractional part for interpolation

            // Noise contributions from (x=0, y=0), z=0 and z=1
    float perm00 = rnm(pi.xy).a ;
    vec3  grad000 = rnm(vec2(perm00, pi.z)).rgb * 4.0 - 1.0;
    float n000 = dot(grad000, pf);
    vec3  grad001 = rnm(vec2(perm00, pi.z + permTexUnit)).rgb * 4.0 - 1.0;
    float n001 = dot(grad001, pf - vec3(0.0, 0.0, 1.0));

            // Noise contributions from (x=0, y=1), z=0 and z=1
    float perm01 = rnm(pi.xy + vec2(0.0, permTexUnit)).a ;
    vec3  grad010 = rnm(vec2(perm01, pi.z)).rgb * 4.0 - 1.0;
    float n010 = dot(grad010, pf - vec3(0.0, 1.0, 0.0));
    vec3  grad011 = rnm(vec2(perm01, pi.z + permTexUnit)).rgb * 4.0 - 1.0;
    float n011 = dot(grad011, pf - vec3(0.0, 1.0, 1.0));

            // Noise contributions from (x=1, y=0), z=0 and z=1
    float perm10 = rnm(pi.xy + vec2(permTexUnit, 0.0)).a ;
    vec3  grad100 = rnm(vec2(perm10, pi.z)).rgb * 4.0 - 1.0;
    float n100 = dot(grad100, pf - vec3(1.0, 0.0, 0.0));
    vec3  grad101 = rnm(vec2(perm10, pi.z + permTexUnit)).rgb * 4.0 - 1.0;
    float n101 = dot(grad101, pf - vec3(1.0, 0.0, 1.0));

            // Noise contributions from (x=1, y=1), z=0 and z=1
    float perm11 = rnm(pi.xy + vec2(permTexUnit, permTexUnit)).a ;
    vec3  grad110 = rnm(vec2(perm11, pi.z)).rgb * 4.0 - 1.0;
    float n110 = dot(grad110, pf - vec3(1.0, 1.0, 0.0));
    vec3  grad111 = rnm(vec2(perm11, pi.z + permTexUnit)).rgb * 4.0 - 1.0;
    float n111 = dot(grad111, pf - vec3(1.0, 1.0, 1.0));

            // Blend contributions along x
    vec4 n_x = mix(vec4(n000, n001, n010, n011), vec4(n100, n101, n110, n111), fade(pf.x));

            // Blend contributions along y
    vec2 n_xy = mix(n_x.xy, n_x.zw, fade(pf.y));

            // Blend contributions along z
    float n_xyz = mix(n_xy.x, n_xy.y, fade(pf.z));

            // We're done, return the final noise value.
    return n_xyz;
}

//2d coordinate orientation thing
vec2 coordRot(in vec2 tc, in float angle)
{
    float aspect = width/height;
    float rotX = ((tc.x*2.0-1.0)*aspect*cos(angle)) - ((tc.y*2.0-1.0)*sin(angle));
    float rotY = ((tc.y*2.0-1.0)*cos(angle)) + ((tc.x*2.0-1.0)*aspect*sin(angle));
    rotX = ((rotX/aspect)*0.5+0.5);
    rotY = rotY*0.5+0.5;
    return vec2(rotX,rotY);
}

void main() 
{
    vec3 rotOffset = vec3(1.425,3.892,5.835); //rotation offset values    
    vec2 rotCoordsR = coordRot(out_texcoord, t + rotOffset.x);
    vec3 noise = vec3(pnoise3D(vec3(rotCoordsR*vec2(width/grainsize,height/grainsize),0.0)));
  
    if (colored)
    {
        vec2 rotCoordsG = coordRot(out_texcoord, t + rotOffset.y);
        vec2 rotCoordsB = coordRot(out_texcoord, t + rotOffset.z);
        noise.g = mix(noise.r,pnoise3D(vec3(rotCoordsG*vec2(width/grainsize,height/grainsize),1.0)),coloramount);
        noise.b = mix(noise.r,pnoise3D(vec3(rotCoordsB*vec2(width/grainsize,height/grainsize),2.0)),coloramount);
    }

        vec3 col = texture2D(diffuse_texture, out_texcoord).rgb;

            //noisiness response curve based on scene luminance
    vec3 lumcoeff = vec3(0.299,0.587,0.114);
    float luminance = mix(0.0,dot(col, lumcoeff),lumamount);
    float lum = smoothstep(0.2,0.0,luminance);
    lum += luminance;
    
    noise = mix(noise,vec3(0.0),pow(lum,4.0));
    col = col + noise * grainamount;
   
    gl_FragColor =  vec4(col,1.0);
})")
        {
            location_location = gpu.get_attribute_location(get_id(), "location");
            texcoord_location = gpu.get_attribute_location(get_id(), "texcoord");
        }

        void on_bind() override
        {
            gpu.enable_vertex_attribute_array(location_location);
            gpu.enable_vertex_attribute_array(texcoord_location);
            gpu.set_vertex_attrib_pointer(location_location, sizeof(vec3) / sizeof(vec3::value_type), GpuDataType<vec3::value_type>::VALUE, false, sizeof(VertexType), reinterpret_cast<void*>(offsetof(VertexType, location)));
            gpu.set_vertex_attrib_pointer(texcoord_location, sizeof(vec2) / sizeof(vec2::value_type), GpuDataType<vec2::value_type>::VALUE, false, sizeof(VertexType), reinterpret_cast<void*>(offsetof(VertexType, texcoord)));
        }

        void on_unbind() override
        {
            gpu.disable_vertex_attribute_array(location_location);
            gpu.disable_vertex_attribute_array(texcoord_location);
        }


    private:
       GpuLocation location_location;
       GpuLocation texcoord_location;
    };
}
