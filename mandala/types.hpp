#pragma once

//glm
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>

//std
#include <functional>

namespace mandala
{
	typedef char				int8_t;
	typedef unsigned char		uint8_t;
	typedef short				int16_t;
	typedef unsigned short		uint16_t;
	typedef int					int32_t;
	typedef unsigned int		uint32_t;
	typedef long long			int64_t;
	typedef unsigned long long	uint64_t;
	typedef float				float32_t;
	typedef double				float64_t;

	typedef glm::vec2			vec2_t;
	typedef glm::lowp_ivec2		vec2_i16_t;
    typedef glm::lowp_uvec2     vec2_u16_t;
	typedef glm::ivec2			vec2_i32_t;
    typedef glm::uvec2          vec2_u32_t;
	typedef glm::dvec2			vec2_f64_t;

	typedef glm::vec3			vec3_t;
    typedef glm::lowp_ivec3		vec3_i16_t;
    typedef glm::lowp_uvec3		vec3_u16_t;
    typedef glm::ivec3			vec3_i32_t;
    typedef glm::uvec3			vec3_u32_t;
	typedef glm::dvec3			vec3_f64_t;

	typedef glm::vec4			vec4_t;
    typedef glm::lowp_ivec4		vec4_i16_t;
    typedef glm::lowp_uvec4		vec4_u16_t;
    typedef glm::ivec4			vec4_i32_t;
    typedef glm::uvec4			vec4_u32_t;
	typedef glm::dvec4			vec4_f64_t;

	typedef glm::fquat			quat_t;
	typedef glm::mat3			mat3_t;
	typedef glm::mat4			mat4_t;

    typedef vec3_t              rgb_type;
    typedef vec4_t              rgba_type;

    template<typename T, typename U>
    T unsafe_cast(U u)
    {
        return (T)u;
    }
};
