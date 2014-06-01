//mandala
#include "collision.hpp"
#include "frustum.hpp"

//std
#include <utility>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

namespace mandala
{
	vec3_t intersect(const plane3_t& p0, const plane3_t& p1, const plane3_t& p2)
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

	intersect_type_t intersects(circle_f32_t circle, const aabb2_t& aabb)
	{
		auto aabb_center = aabb.center();
		auto distance = vec2_t(glm::abs(circle.origin.x - aabb_center.x), glm::abs(circle.origin.y - aabb_center.y));

		if (distance.x > ((aabb.width() / 2) + circle.radius) ||
			distance.y > ((aabb.height() / 2) + circle.radius))
		{
			return intersect_type_t::disjoint;
		}

		if (distance.x <= (aabb.width() / 2) ||
			distance.y <= (aabb.height() / 2))
		{
			return intersect_type_t::intersect;
		}

		auto distance_2 = glm::pow2(distance.x - aabb.width() / 2) + glm::pow2(distance.y - aabb.height() / 2);

		if (distance_2 <= glm::pow2(circle.radius))
		{
			return intersect_type_t::intersect;
		}

		return intersect_type_t::disjoint;
	}

	intersect_type_t intersects(const aabb3_t& aabb, const frustum_t& frustum)
	{
		size_t total_in = 0;
		auto corners = aabb.get_corners();

		for (const auto& plane : frustum.planes)
		{
            size_t in = aabb3_t::corner_count;
			bool point_in = false;

            for (size_t corner_index = 0; corner_index < aabb3_t::corner_count; corner_index++)
			{
				const auto& corner = corners[corner_index];

				if (glm::dot(corner - plane.origin(), plane.normal) > 0)
				{
					point_in = true;

					--in;
				}
			}

			if (in == 0)
			{
				return intersect_type_t::disjoint;
			}

			if (point_in)
			{
				++total_in;
			}
		}

		if (total_in == frustum_t::plane_count)
		{
			return intersect_type_t::contain;
		}

		return intersect_type_t::intersect;
	}

	intersect_type_t intersects(const line3_t& line, const aabb3_t& aabb, float32_t* t, vec3_t* location, vec3_t* normal)
	{
		//http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/
		if (line.start == line.end)
		{
			return intersect_type_t::disjoint;
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
			return intersect_type_t::disjoint;
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
			return intersect_type_t::disjoint;
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
			return intersect_type_t::contain;
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
				*normal = vec3_t(-1, 0, 0);
			}
			else if (glm::abs(location->x - aabb.max.x) < 0.01f)
			{
				*normal = vec3_t(1, 0, 0);
			}
			else if (glm::abs(location->y - aabb.min.y) < 0.01f)
			{
				*normal = vec3_t(0, -1, 0);
			}
			else if (glm::abs(location->y - aabb.max.y) < 0.01f)
			{
				*normal = vec3_t(0, 1, 0);
			}
			else if (glm::abs(location->z - aabb.min.z) < 0.01f)
			{
				*normal = vec3_t(0, 0, -1);
			}
			else if (glm::abs(location->z - aabb.max.z) < 0.01f)
			{
				*normal = vec3_t(0, 0, 1);
			}
		}

		if (normal == nullptr && location != nullptr)
		{
			*location = line.start + (d * tmin);
		}

		return intersect_type_t::intersect;
	}

	intersect_type_t intersects(const line3_t& line, const plane3_t& plane, float32_t& t)
	{
		auto u = line.end - line.start;
		auto w = line.start - plane.normal;
		auto d = glm::dot(plane.normal, u);
		auto n = -glm::dot(plane.normal, w);

		if (glm::abs(d) < glm::epsilon<float32_t>())
		{
			return n == 0 ? intersect_type_t::parallel : intersect_type_t::disjoint;
		}

		t = n / d;

		if (t < 0 || t > 1)
		{
			return intersect_type_t::disjoint;
		}

		return intersect_type_t::intersect;
	}

	intersect_type_t intersects(const aabb3_t aabb0, const vec3_t& d0, const aabb3_t& aabb1, const vec3_t& d1, float32_t& u0, float32_t& u1)
	{
		auto v = glm::value_ptr((d1 - d0));
		auto amin = glm::value_ptr(aabb0.min);
		auto amax = glm::value_ptr(aabb0.max);
		auto bmin = glm::value_ptr(aabb1.min);
		auto bmax = glm::value_ptr(aabb1.max);
		vec3_t u_0(std::numeric_limits<float32_t>::max());
		vec3_t u_1(std::numeric_limits<float32_t>::min());

		if (intersects(aabb0, aabb1) != intersect_type_t::disjoint)
		{
			u0 = u1 = 0;

			return intersect_type_t::intersect;
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

		auto result = (u0 >= 0 && u1 <= 1 && u0 <= u1) ? intersect_type_t::intersect : intersect_type_t::disjoint;

		return result;
	}
};
