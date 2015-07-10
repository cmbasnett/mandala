#pragma once

//std
#include <type_traits>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//mandala
#include "types.hpp"
#include "aabb.hpp"
#include "line.hpp"
#include "plane.hpp"
#include "circle.hpp"
#include "frustum.hpp"
#include "sphere.hpp"

namespace mandala
{
    enum class intersect_type_e
    {
        DISJOINT,
        INTERSECT,
        CONTAIN,
        PARALLEL
    };

    template<typename Scalar>
    glm::detail::tvec3<Scalar> intersect(const details::plane3_t<Scalar>& p0, const details::plane3_t<Scalar>& p1, const details::plane3_t<Scalar>& p2)
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
    intersect_type_e intersects(const details::circle_t<Scalar>& circle, const details::aabb2_t<Scalar>& aabb)
    {
        const auto aabb_center = aabb.center();
        const auto distance = vec2_t(glm::abs(circle.origin.x - aabb_center.x), glm::abs(circle.origin.y - aabb_center.y));

        if (distance.x > ((aabb.width() / 2) + circle.radius) ||
            distance.y > ((aabb.height() / 2) + circle.radius))
        {
            return intersect_type_e::DISJOINT;
        }

        if (distance.x <= (aabb.width() / 2) ||
            distance.y <= (aabb.height() / 2))
        {
            return intersect_type_e::INTERSECT;
        }

        const auto distance_2 = glm::pow2(distance.x - aabb.width() / 2) + glm::pow2(distance.y - aabb.height() / 2);

        if (distance_2 <= glm::pow2(circle.radius))
        {
            return intersect_type_e::intersect;
        }

        return intersect_type_e::disjoint;
    }

    template<typename LHSScalar, typename RHSScalar>
    intersect_type_e intersects(const details::aabb2_t<LHSScalar>& lhs, const details::aabb2_t<RHSScalar>& rhs)
    {
        if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x ||
            lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y)
        {
            return intersect_type_e::DISJOINT;
        }

        return intersect_type_e::INTERSECT;
    }

    template<typename AABBScalar, typename PointScalar>
    bool contains(const details::aabb2_t<AABBScalar>& aabb, const glm::detail::tvec2<PointScalar>& point)
    {
        return aabb.min == glm::min(aabb.min, static_cast<glm::detail::tvec2<AABBScalar>>(point)) &&
               aabb.max == glm::max(aabb.max, static_cast<glm::detail::tvec2<AABBScalar>>(point));
    }

    template<typename AABBScalar, typename PointScalar>
    bool contains(const details::aabb3_t<AABBScalar>& aabb, const glm::detail::tvec3<PointScalar>& point)
    {
        return aabb.min == glm::min(aabb.min, static_cast<glm::detail::tvec3<AABBScalar>>(point)) &&
               aabb.max == glm::max(aabb.max, static_cast<glm::detail::tvec3<AABBScalar>>(point));
    }

    template<typename LHSScalar, typename RHSScalar>
    bool contains(const details::aabb3_t<LHSScalar>& lhs, const details::aabb3_t<RHSScalar>& rhs)
    {
        return contains(lhs, rhs.min) && contains(lhs, rhs.max);
    }

    template<typename LHSScalar, typename RHSScalar>
    intersect_type_e intersects(const details::aabb3_t<LHSScalar>& lhs, const details::aabb3_t<RHSScalar>& rhs)
    {
        if (contains(lhs, rhs))
        {
            return intersect_type_e::CONTAIN;
        }

        if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x ||
            lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y ||
            lhs.max.z < rhs.min.z || lhs.min.z > rhs.max.z)
        {
            return intersect_type_e::DISJOINT;
        }

        return intersect_type_e::INTERSECT;
    }

    template<typename Scalar>
    intersect_type_e intersects(const details::line3_t<Scalar>& line, const details::aabb3_t<Scalar>& aabb, Scalar* t = nullptr, glm::detail::tvec3<Scalar>* location = nullptr, glm::detail::tvec3<Scalar>* normal = nullptr)
    {
        //http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-box-intersection/
        if (line.start == line.end)
        {
            return intersect_type_e::DISJOINT;
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
            return intersect_type_e::DISJOINT;
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
            return intersect_type_e::DISJOINT;
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
            return intersect_type_e::CONTAIN;
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

        return intersect_type_e::INTERSECT;
    }

    template<typename Scalar = std::enable_if<std::is_floating_point<Scalar>::value>::type>
    intersect_type_e intersects(const details::line3_t<Scalar>& line, const details::plane3_t<Scalar>& plane, Scalar& t)
    {
        const auto u = line.end - line.start;
        const auto w = line.start - plane.normal;
        const auto d = glm::dot(plane.normal, u);
        const auto n = -glm::dot(plane.normal, w);

        if (glm::abs(d) < glm::epsilon<Scalar>())
        {
            return n == 0 ? intersect_type_e::PARALLEL : intersect_type_e::DISJOINT;
        }

        t = n / d;

        if (t < 0 || t > 1)
        {
            return intersect_type_e::DISJOINT;
        }

        return intersect_type_e::INTERSECT;
    }

    template<typename Scalar>
    Scalar distance_to_plane(const details::plane3_t<Scalar>& plane, const glm::detail::tvec3<Scalar>& point)
    {
        return glm::dot(plane.normal, point - plane.origin());
    }

    template<typename PlaneScalar, typename PointScalar>
    PlaneScalar distance_to_plane(const details::plane3_t<PlaneScalar>& plane, const glm::detail::tvec3<PointScalar>& point)
    {
        return glm::dot(plane.normal, static_cast<glm::detail::tvec3<PlaneScalar>>(point)-plane.origin());
    }

    template<typename FrustumScalar, typename PointScalar>
    intersect_type_e intersects(const details::frustum_t<FrustumScalar>& frustum, const glm::detail::tvec3<PointScalar>& point)
    {
        for (const auto& plane : frustum.planes())
        {
            if (distance_to_plane(plane, point) < 0)
            {
                return intersect_type_e::disjoint;
            }
        }

        return intersect_type_e::contain;
    }

    template<typename FrustumScalar, typename SphereScalar>
    intersect_type_e intersects(const details::frustum_t<FrustumScalar>& frustum, const details::sphere_t<SphereScalar>& sphere)
    {
        for (const auto& plane : frustum.get_planes())
        {
            if (distance_to_plane(plane, sphere.origin) + sphere.radius < 0)
            {
                return intersect_type_e::DISJOINT;
            }
        }

        return intersect_type_e::CONTAIN;
    }

    template<typename FrustumScalar, typename AABBScalar>
    intersect_type_e intersects(const details::frustum_t<FrustumScalar>& frustum, const details::aabb3_t<AABBScalar>& aabb)
    {
        typedef details::frustum_t<FrustumScalar> frustum_type;
        typedef details::aabb3_t<AABBScalar> aabb_type;

        size_t total_in = 0;

        for (const auto& plane : frustum.get_planes())
        {
            auto in_count = aabb_type::CORNER_COUNT;
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
                return intersect_type_e::DISJOINT;
            }
            else if (is_point_inside)
            {
                ++total_in;
            }
        }

        if (total_in == FRUSTUM_PLANE_COUNT)
        {
            return intersect_type_e::CONTAIN;
        }

        return intersect_type_e::INTERSECT;
    }

    template<typename Scalar = std::enable_if<std::is_floating_point<Scalar>::value>::type>
    intersect_type_e intersects(const details::aabb3_t<Scalar> aabb0, const glm::detail::tvec3<Scalar>& d0, const details::aabb3_t<Scalar>& aabb1, const glm::detail::tvec3<Scalar>& d1, Scalar& u0, Scalar& u1)
    {
        typedef glm::detail::tvec3<Scalar> vector_type;

        const auto v = glm::value_ptr((d1 - d0));
        const auto amin = glm::value_ptr(aabb0.min);
        const auto amax = glm::value_ptr(aabb0.max);
        const auto bmin = glm::value_ptr(aabb1.min);
        const auto bmax = glm::value_ptr(aabb1.max);
        vector_type u_0(std::numeric_limits<Scalar>::max());
        vector_type u_1(std::numeric_limits<Scalar>::min());

        if (intersects(aabb0, aabb1) != intersect_type_e::DISJOINT)
        {
            u0 = u1 = 0;

            return intersect_type_e::INTERSECT;
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

        auto result = (u0 >= 0 && u1 <= 1 && u0 <= u1) ? intersect_type_e::INTERSECT : intersect_type_e::DISJOINT;

        return result;
    }
}
