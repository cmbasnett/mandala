#pragma once

#undef MANDALA_CPP11
#if __cplusplus == (199711L)
#define MANDALA_CPP11
#endif

#define VEC3_ZERO		(vec3_t(0))
#define VEC3_X			(vec3_t(1, 0, 0))
#define VEC3_XY			(vec3_t(1, 1, 0))
#define VEC3_XZ			(vec3_t(1, 0, 1))
#define VEC3_XYZ		(vec3_t(1, 1, 1))
#define VEC3_Y			(vec3_t(0, 1, 0))
#define VEC3_Z			(vec3_t(0, 0, 1))
#define VEC3_YZ			(vec3_t(0, 1, 1))

#define VEC3I_ZERO		(vec3i_t(0))
#define VEC3I_X			(vec3i_t(1, 0, 0))
#define VEC3I_XY		(vec3i_t(1, 1, 0))
#define VEC3I_XZ		(vec3i_t(1, 0, 1))
#define VEC3I_XYZ		(vec3i_t(1, 1, 1))
#define VEC3I_Y			(vec3i_t(0, 1, 0))
#define VEC3I_Z			(vec3i_t(0, 0, 1))
#define VEC3I_YZ		(vec3i_t(0, 1, 1))

#define VEC3_FORWARD	(VEC3_Z)
#define VEC3_BACKWARD   (-VEC3_FORWARD)
#define VEC3_LEFT		(VEC3_X)
#define VEC3_RIGHT		(-VEC3_LEFT)
#define VEC3_UP			(VEC3_Y)
#define VEC3_DOWN		(-VEC3_UP)
