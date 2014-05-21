#pragma once

//mandala
#include "types.h"
#include "aabb.h"
#include "line.h"
#include "plane.h"
#include "circle.h"

namespace mandala
{
	struct frustum_t;

	enum class intersect_type_t
	{
		disjoint,
		intersect,
		contain,
		parallel
	};

	vec3_t intersect(const plane3_t& p0, const plane3_t& p1, const plane3_t& p2);

	intersect_type_t intersects(circle_t lhs, const aabb2_t& rhs);

	template<typename T>
	intersect_type_t intersects(const details::aabb2_t<T>& lhs, const details::aabb2_t<T>& rhs)
	{
		if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x ||
			lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y)
		{
			return intersect_type_t::disjoint;
		}

		return intersect_type_t::intersect;
	}

	template<typename T>
	intersect_type_t intersects(const details::aabb3_t<T>& lhs, const details::aabb3_t<T>& rhs)
	{
		if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x ||
			lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y ||
			lhs.max.z < rhs.min.z || lhs.min.z > rhs.max.z)
		{
			return intersect_type_t::disjoint;
		}

		return intersect_type_t::intersect;
	}

	intersect_type_t intersects(const line3_t& line, const aabb3_t& aabb, float32_t* t = nullptr, vec3_t* location = nullptr, vec3_t* normal = nullptr);

	intersect_type_t intersects(const line3_t& line, const plane3_t& plane, float32_t& t);

	intersect_type_t intersects(const aabb3_t& aabb, const frustum_t& frustum);

	intersect_type_t intersects(const aabb3_t aabb0, const vec3_t& d0, const aabb3_t& aabb1, const vec3_t& d1, float32_t& u0, float32_t& u1);
};