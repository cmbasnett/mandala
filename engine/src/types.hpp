#pragma once

//glm
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>

namespace naga
{
    typedef char                i8;
    typedef unsigned char       u8;
    typedef short               i16;
    typedef unsigned short      u16;
    typedef int                 i32;
    typedef unsigned int        u32;
    typedef long long           i64;
    typedef unsigned long long  u64;
    typedef float               f32;
    typedef double              f64;

    typedef glm::vec2           vec2;
    typedef glm::ivec2          vec2_i32;
    typedef glm::uvec2          vec2_u32;
    typedef glm::dvec2          vec2_f64;

    typedef glm::vec3           vec3;
    typedef glm::ivec3          vec3_i32;
    typedef glm::uvec3          vec3_u32;
    typedef glm::dvec3          vec3_f64;

    typedef glm::vec4           vec4;
    typedef glm::ivec4          vec4_i32;
    typedef glm::uvec4          vec4_u32;
    typedef glm::dvec4          vec4_f64;

    typedef glm::fquat          quat;
    typedef glm::mat3           mat3;
    typedef glm::mat4           mat4;
};

template<typename T, typename U>
inline T unsafe_cast(U u)
{
    return (T)u;
}