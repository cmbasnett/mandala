#pragma once

//std
#include <type_traits>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//naga
#include "types.hpp"
#include "aabb.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "circle.hpp"
#include "frustum.hpp"
#include "sphere.hpp"

namespace naga
{
    enum class IntersectType
    {
        DISJOINT,
        INTERSECT,
        CONTAIN,
        PARALLEL
    };

    template<typename Scalar>
    glm::tvec3<Scalar> intersect(const details::Plane3<Scalar>& p0, const details::Plane3<Scalar>& p1, const details::Plane3<Scalar>& p2)
    {
        const auto d0 = p0.distance;
        const auto d1 = p1.distance;
        const auto d2 = p2.distance;

        const auto& n0 = p0.normal;
        const auto& n1 = p1.normal;
        const auto& n2 = p2.normal;

        const auto e = (d0 * (glm::cross(n1, n2))) + (d1 * (glm::cross(n2, n0))) + (d2 * (glm::cross(n0, n1)));
        const auto d = glm::dot(n1, glm::cross(n1, n2));

        return e / d;
    }

    template<typename Scalar>
    IntersectType intersects(const details::Circle<Scalar>& circle, const details::AABB2<Scalar>& aabb)
    {
        const auto aabb_center = aabb.center();
        const auto distance = vec2(glm::abs(circle.origin.x - aabb_center.x), glm::abs(circle.origin.y - aabb_center.y));

        if (distance.x > ((aabb.width() / 2) + circle.radius) ||
            distance.y > ((aabb.height() / 2) + circle.radius))
        {
            return IntersectType::DISJOINT;
        }

        if (distance.x <= (aabb.width() / 2) ||
            distance.y <= (aabb.height() / 2))
        {
            return IntersectType::INTERSECT;
        }

        const auto distance_2 = glm::pow2(distance.x - aabb.width() / 2) + glm::pow2(distance.y - aabb.height() / 2);

        if (distance_2 <= glm::pow2(circle.radius))
        {
            return IntersectType::intersect;
        }

        return IntersectType::disjoint;
    }

    template<typename LHSScalar, typename RHSScalar>
	IntersectType intersects(const details::AABB2<LHSScalar>& lhs, const details::AABB2<RHSScalar>& rhs)
    {
        if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x ||
            lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y)
        {
            return IntersectType::DISJOINT;
        }

        return IntersectType::INTERSECT;
    }

    template<typename AABBScalar, typename PointScalar>
	bool contains(const details::AABB2<AABBScalar>& aabb, const glm::tvec2<PointScalar>& point)
    {
        return aabb.min == glm::min(aabb.min, static_cast<glm::tvec2<AABBScalar>>(point)) &&
               aabb.max == glm::max(aabb.max, static_cast<glm::tvec2<AABBScalar>>(point));
    }

    template<typename AABBScalar, typename PointScalar>
    bool contains(const details::AABB3<AABBScalar>& aabb, const glm::tvec3<PointScalar>& point)
    {
        return aabb.min == glm::min(aabb.min, static_cast<glm::tvec3<AABBScalar>>(point)) &&
               aabb.max == glm::max(aabb.max, static_cast<glm::tvec3<AABBScalar>>(point));
    }

    template<typename LHSScalar, typename RHSScalar>
	bool contains(const details::AABB3<LHSScalar>& lhs, const details::AABB3<RHSScalar>& rhs)
    {
        return contains(lhs, rhs.min) && contains(lhs, rhs.max);
    }

    template<typename LHSScalar, typename RHSScalar>
	IntersectType intersects(const details::AABB3<LHSScalar>& lhs, const details::AABB3<RHSScalar>& rhs)
    {
        if (contains(lhs, rhs))
        {
            return IntersectType::CONTAIN;
        }

        if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x ||
            lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y ||
            lhs.max.z < rhs.min.z || lhs.min.z > rhs.max.z)
        {
            return IntersectType::DISJOINT;
        }

        return IntersectType::INTERSECT;
    }

    template<typename Scalar>
	IntersectType intersects(const details::Line3<Scalar>& line, const details::AABB3<Scalar>& aabb, Scalar* t = nullptr, glm::tvec3<Scalar>* location = nullptr, glm::tvec3<Scalar>* normal = nullptr)
    {
		// r.dir is unit direction vector of ray
		vec3 dirfrac;
		dirfrac.x = 1.0f / line.direction().x;
		dirfrac.y = 1.0f / line.direction().y;
		dirfrac.z = 1.0f / line.direction().z;
		// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
		// r.org is origin of ray
		f32 t1 = (aabb.min.x - line.start.x) * dirfrac.x;
		f32 t2 = (aabb.max.x - line.start.x) * dirfrac.x;
		f32 t3 = (aabb.min.y - line.start.y) * dirfrac.y;
		f32 t4 = (aabb.max.y - line.start.y) * dirfrac.y;
		f32 t5 = (aabb.min.z - line.start.z) * dirfrac.z;
		f32 t6 = (aabb.max.z - line.start.z) * dirfrac.z;
		f32 tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
		f32 tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

		// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
		if (tmax < 0) {
			if (t != nullptr) {
				*t = tmax;
			}
			return IntersectType::DISJOINT;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax) {
			if (t != nullptr) {
				*t = tmax;
			}
			return IntersectType::DISJOINT;
		}
		if (t != nullptr) {
			*t = tmin;
		}
		return IntersectType::INTERSECT;
    }

    template<typename Scalar = std::enable_if<std::is_floating_point<Scalar>::value>::type>
    IntersectType intersects(const details::Line3<Scalar>& line, const details::Plane3<Scalar>& plane, Scalar& t)
    {
        const auto u = line.end - line.start;
        const auto w = line.start - plane.normal;
        const auto d = glm::dot(plane.normal, u);
        const auto n = -glm::dot(plane.normal, w);

        if (glm::abs(d) < glm::epsilon<Scalar>())
        {
            return n == 0 ? IntersectType::PARALLEL : IntersectType::DISJOINT;
        }

        t = n / d;

        if (t < 0 || t > 1)
        {
            return IntersectType::DISJOINT;
        }

        return IntersectType::INTERSECT;
    }

    template<typename Scalar>
    Scalar distance_to_plane(const details::Plane3<Scalar>& plane, const glm::tvec3<Scalar>& point)
    {
        return glm::dot(plane.normal, point - plane.origin());
    }

    template<typename PlaneScalar, typename PointScalar>
    PlaneScalar distance_to_plane(const details::Plane3<PlaneScalar>& plane, const glm::tvec3<PointScalar>& point)
    {
        return glm::dot(plane.normal, static_cast<glm::tvec3<PlaneScalar>>(point)-plane.origin());
    }

    template<typename FrustumScalar, typename PointScalar>
    IntersectType intersects(const details::Frustum<FrustumScalar>& frustum, const glm::tvec3<PointScalar>& point)
    {
        for (const auto& plane : frustum.planes())
        {
            if (distance_to_plane(plane, point) < 0)
            {
                return IntersectType::disjoint;
            }
        }

        return IntersectType::contain;
    }

    template<typename FrustumScalar, typename SphereScalar>
	IntersectType intersects(const details::Frustum<FrustumScalar>& frustum, const details::Sphere<SphereScalar>& sphere)
    {
        for (const auto& plane : frustum.get_planes())
        {
            if (distance_to_plane(plane, sphere.origin) + sphere.radius < 0)
            {
                return IntersectType::DISJOINT;
            }
        }

        return IntersectType::CONTAIN;
    }

    template<typename FrustumScalar, typename AABBScalar>
	IntersectType intersects(const details::Frustum<FrustumScalar>& frustum, const details::AABB3<AABBScalar>& aabb)
    {
		typedef details::Frustum<FrustumScalar> FrustumType;
		typedef details::AABB3<AABBScalar> AABBType;

        size_t total_in = 0;

        for (const auto& plane : frustum.get_planes())
        {
			auto in_count = AABBType::CORNER_COUNT;
            bool is_point_inside = false;

            for (const auto& corner : aabb.get_corners())
            {
                if (distance_to_plane(plane, corner) < 0)
                {
                    is_point_inside = true;

                    --in_count;
                }
            }

            if (in_count == 0)
            {
                return IntersectType::DISJOINT;
            }
            else if (is_point_inside)
            {
                ++total_in;
            }
        }

        if (total_in == FRUSTUM_PLANE_COUNT)
        {
            return IntersectType::CONTAIN;
        }

        return IntersectType::INTERSECT;
    }

    template<typename Scalar = std::enable_if<std::is_floating_point<Scalar>::value>::type>
	IntersectType intersects(const details::AABB3<Scalar> aabb0, const glm::tvec3<Scalar>& d0, const details::AABB3<Scalar>& aabb1, const glm::tvec3<Scalar>& d1, Scalar& u0, Scalar& u1)
    {
        typedef glm::tvec3<Scalar> vector_type;

        const auto v = glm::value_ptr((d1 - d0));
        const auto amin = glm::value_ptr(aabb0.min);
        const auto amax = glm::value_ptr(aabb0.max);
        const auto bmin = glm::value_ptr(aabb1.min);
        const auto bmax = glm::value_ptr(aabb1.max);
        vector_type u_0(std::numeric_limits<Scalar>::max());
        vector_type u_1(std::numeric_limits<Scalar>::min());

        if (intersects(aabb0, aabb1) != IntersectType::DISJOINT)
        {
            u0 = u1 = 0;

            return IntersectType::INTERSECT;
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

        auto result = (u0 >= 0 && u1 <= 1 && u0 <= u1) ? IntersectType::INTERSECT : IntersectType::DISJOINT;

        return result;
    }
}
