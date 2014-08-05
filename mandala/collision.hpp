#pragma once

//mandala
#include "types.hpp"
#include "aabb.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "circle.hpp"
#include "frustum.hpp"

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//std
#include <type_traits>

namespace mandala
{
	enum class intersect_type_e
	{
		disjoint,
		intersect,
		contain,
		parallel
	};

	template<typename T>
	glm::detail::tvec3<T> intersect(const details::plane3_t<T>& p0, const details::plane3_t<T>& p1, const details::plane3_t<T>& p2)
	{
		auto d0 = p0.distance;
		auto d1 = p1.distance;
		auto d2 = p2.distance;

		const auto& n0 = p0.normal;
		const auto& n1 = p1.normal;
		const auto& n2 = p2.normal;

		auto e = (d0 * (glm::cross(n1, n2))) + (d1 * (glm::cross(n2, n0))) + (d2 * (glm::cross(n0, n1)));
		auto d = glm::dot(n1, glm::cross(n1, n2));

		return e / d;
	}

	template<typename T>
	intersect_type_e intersects(const details::circle_t<T>& circle, const details::aabb2_t<T>& aabb)
	{
		auto aabb_center = aabb.center();
		auto distance = vec2_t(glm::abs(circle.origin.x - aabb_center.x), glm::abs(circle.origin.y - aabb_center.y));

		if (distance.x > ((aabb.width() / 2) + circle.radius) ||
			distance.y > ((aabb.height() / 2) + circle.radius))
		{
			return intersect_type_e::disjoint;
		}

		if (distance.x <= (aabb.width() / 2) ||
			distance.y <= (aabb.height() / 2))
		{
			return intersect_type_e::intersect;
		}

		auto distance_2 = glm::pow2(distance.x - aabb.width() / 2) + glm::pow2(distance.y - aabb.height() / 2);

		if (distance_2 <= glm::pow2(circle.radius))
		{
			return intersect_type_e::intersect;
		}

		return intersect_type_e::disjoint;
	}

	template<typename T>
    intersect_type_e intersects(const details::aabb2_t<T>& lhs, const details::aabb2_t<T>& rhs)
	{
		if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x ||
			lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y)
		{
			return intersect_type_e::disjoint;
		}

		return intersect_type_e::intersect;
	}

	template<typename T>
    intersect_type_e intersects(const details::aabb3_t<T>& lhs, const details::aabb3_t<T>& rhs)
	{
		if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x ||
			lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y ||
			lhs.max.z < rhs.min.z || lhs.min.z > rhs.max.z)
		{
			return intersect_type_e::disjoint;
		}

		return intersect_type_e::intersect;
	}

	template<typename T>
	intersect_type_e intersects(const details::line3_t<T>& line, const details::aabb3_t<T>& aabb, T* t = nullptr, glm::detail::tvec3<T>* location = nullptr, glm::detail::tvec3<T>* normal = nullptr)
	{
		//http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/
		if (line.start == line.end)
		{
			return intersect_type_e::disjoint;
		}

		auto d = line.direction();

		auto tmin = (aabb.min.x - line.start.x) / d.x;
		auto tmax = (aabb.max.x - line.start.x) / d.x;

		if (tmin > tmax)
		{
			std::swap(tmin, tmax);
		}

		auto tymin = (aabb.min.y - line.start.y) / d.y;
		auto tymax = (aabb.max.y - line.start.y) / d.y;

		if (tymin > tymax)
		{
			std::swap(tymin, tymax);
		}

		if ((tmin > tymax) || (tymin > tmax))
		{
			return intersect_type_e::disjoint;
		}

		if (tymin > tmin)
		{
			tmin = tymin;
		}

		if (tymax < tmax)
		{
			tmax = tymax;
		}

		auto tzmin = (aabb.min.z - line.start.z) / d.z;
		auto tzmax = (aabb.max.z - line.start.z) / d.z;

		if (tzmin > tzmax)
		{
			std::swap(tzmin, tzmax);
		}

		if (tmin > tzmax || tzmin > tmax)
		{
			return intersect_type_e::disjoint;
		}

		if (tzmin > tmin)
		{
			tmin = tzmin;
		}

		if (tzmax < tmax)
		{
			tmax = tzmax;
		}

		if (tmin < 0.0f)
		{
			return intersect_type_e::contain;
		}

		if (t != nullptr)
		{
			*t = tmin;
		}

		if (normal != nullptr)
		{
			*location = line.start + (d * tmin);

			if (glm::abs(location->x - aabb.min.x) < 0.01f)
			{
				*normal = glm::detail::tvec3<T>(-1, 0, 0);
			}
			else if (glm::abs(location->x - aabb.max.x) < 0.01f)
			{
				*normal = glm::detail::tvec3<T>(1, 0, 0);
			}
			else if (glm::abs(location->y - aabb.min.y) < 0.01f)
			{
				*normal = glm::detail::tvec3<T>(0, -1, 0);
			}
			else if (glm::abs(location->y - aabb.max.y) < 0.01f)
			{
				*normal = glm::detail::tvec3<T>(0, 1, 0);
			}
			else if (glm::abs(location->z - aabb.min.z) < 0.01f)
			{
				*normal = glm::detail::tvec3<T>(0, 0, -1);
			}
			else if (glm::abs(location->z - aabb.max.z) < 0.01f)
			{
				*normal = glm::detail::tvec3<T>(0, 0, 1);
			}
		}

		if (normal == nullptr && location != nullptr)
		{
			*location = line.start + (d * tmin);
		}

		return intersect_type_e::intersect;
	}

	template<typename T = std::enable_if<std::is_floating_point<T>::value>::type>
	intersect_type_e intersects(const details::line3_t<T>& line, const details::plane3_t<T>& plane, T& t)
	{
		const auto u = line.end - line.start;
		const auto w = line.start - plane.normal;
		const auto d = glm::dot(plane.normal, u);
		const auto n = -glm::dot(plane.normal, w);

		if (glm::abs(d) < glm::epsilon<T>())
		{
			return n == 0 ? intersect_type_e::parallel : intersect_type_e::disjoint;
		}

		t = n / d;

		if (t < 0 || t > 1)
		{
			return intersect_type_e::disjoint;
		}

		return intersect_type_e::intersect;
	}

	template<typename T>
	intersect_type_e intersects(const details::aabb3_t<T>& aabb, const details::frustum_t<T>& frustum)
	{
		size_t total_in = 0;
		const auto corners = aabb.get_corners();

		for (const auto& plane : frustum.planes)
		{
			auto in_count = details::aabb3_t<T>::corner_count;
			bool is_point_in = false;

			for (size_t corner_index = 0; corner_index < details::aabb3_t<T>::corner_count; corner_index++)
			{
				const auto& corner = corners[corner_index];

				if (glm::dot(corner - plane.origin(), plane.normal) > 0)
				{
					is_point_in = true;

					--in_count;
				}
			}

			if (in_count == 0)
			{
				return intersect_type_e::disjoint;
			}
			else if (is_point_in)
			{
				++total_in;
			}
		}

		if (total_in == details::frustum_t<T>::plane_count)
		{
			return intersect_type_e::contain;
		}

		return intersect_type_e::intersect;
	}

	template<typename T = std::enable_if<std::is_floating_point<T>::value>::type>
	intersect_type_e intersects(const details::aabb3_t<T> aabb0, const glm::detail::tvec3<T>& d0, const details::aabb3_t<T>& aabb1, const glm::detail::tvec3<T>& d1, T& u0, T& u1)
	{
		const auto v = glm::value_ptr((d1 - d0));
		const auto amin = glm::value_ptr(aabb0.min);
		const auto amax = glm::value_ptr(aabb0.max);
		const auto bmin = glm::value_ptr(aabb1.min);
		const auto bmax = glm::value_ptr(aabb1.max);
		glm::detail::tvec3<T> u_0(std::numeric_limits<T>::max());
		glm::detail::tvec3<T> u_1(std::numeric_limits<T>::min());

		if (intersects(aabb0, aabb1) != intersect_type_e::disjoint)
		{
			u0 = u1 = 0;

			return intersect_type_e::intersect;
		}

		for (size_t i = 0; i < 3; ++i)
		{
			if (v[i] == 0)
			{
				u_0[i] = 0;
				u_1[i] = 1;

				continue;
			}

			if (amax[i] < bmin[i] && v[i] < 0)
			{
				u_0[i] = (amax[i] - bmin[i]) / v[i];
			}

			else if (bmax[i] < amin[i] && v[i] > 0)
			{
				u_0[i] = (amin[i] - bmax[i]) / v[i];
			}

			if (bmax[i] > amin[i] && v[i] < 0)
			{
				u_1[i] = (amin[i] - bmax[i]) / v[i];
			}

			else if (amax[i] > bmin[i] && v[i] > 0)
			{
				u_1[i] = (amax[i] - bmin[i]) / v[i];
			}
		}

		u0 = glm::compMax(u_0);
		u1 = glm::compMin(u_1);

		auto result = (u0 >= 0 && u1 <= 1 && u0 <= u1) ? intersect_type_e::intersect : intersect_type_e::disjoint;

		return result;
	}
}
