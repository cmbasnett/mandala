#pragma once

//std
#include <array>

//naga
#include "types.hpp"
#include "plane.hpp"
#include "aabb.hpp"
#include "sphere.hpp"

//glm
#include <glm\gtc\matrix_access.hpp>

namespace naga
{
    enum
    {
        FRUSTUM_CORNER_INDEX_LEFT_TOP_NEAR,
        FRUSTUM_CORNER_INDEX_LEFT_TOP_FAR,
        FRUSTUM_CORNER_INDEX_LEFT_BOTTOM_NEAR,
        FRUSTUM_CORNER_INDEX_LEFT_BOTTOM_FAR,
        FRUSTUM_CORNER_INDEX_RIGHT_TOP_NEAR,
        FRUSTUM_CORNER_INDEX_RIGHT_TOP_FAR,
        FRUSTUM_CORNER_INDEX_RIGHT_BOTTOM_NEAR,
        FRUSTUM_CORNER_INDEX_RIGHT_BOTTOM_FAR
    };

    enum
    {
        FRUSTUM_PLANE_INDEX_LEFT,
        FRUSTUM_PLANE_INDEX_RIGHT,
        FRUSTUM_PLANE_INDEX_TOP,
        FRUSTUM_PLANE_INDEX_BOTTOM,
        FRUSTUM_PLANE_INDEX_NEAR,
        FRUSTUM_PLANE_INDEX_FAR
    };

    static const size_t FRUSTUM_PLANE_COUNT = 6;
    static const size_t FRUSTUM_CORNER_COUNT = 8;

    namespace details
    {
		template<typename ScalarType, typename Enable = void>
        struct Frustum;

        template<typename ScalarType>
		struct Frustum<ScalarType, typename std::enable_if<std::is_floating_point<ScalarType>::value>::type>
        {
			typedef glm::tvec3<ScalarType> VectorType;
			typedef Frustum<ScalarType> Type;
			typedef details::Plane3<ScalarType> PlaneType;
			typedef glm::tmat4x4<ScalarType> MatrixType;
            typedef std::array<PlaneType, FRUSTUM_PLANE_COUNT> PlanesType;
            typedef std::array<VectorType, FRUSTUM_CORNER_COUNT> CornersType;
			typedef details::AABB3<ScalarType> AABBType;
			typedef details::Sphere<ScalarType> SphereType;

            Frustum() = default;

			void set(const VectorType& origin, const VectorType& left, const VectorType& up, const VectorType& forward, ScalarType fov, ScalarType near, ScalarType far, ScalarType aspect)
            {
                const auto tangent = glm::tan(glm::radians(fov) / 2);
                const auto near_height = near * tangent;
                const auto near_width = near_height * aspect;

                const auto far_plane_half_height = tangent * far;
                const auto near_plane_half_height = tangent * near;
                const auto far_plane_half_width = far_plane_half_height * aspect;
                const auto near_plane_half_width = near_plane_half_height * aspect;

                const auto far_plane_center = origin + (forward * far);
                const auto far_plane_right = far_plane_center + (left * far_plane_half_width);
                const auto far_plane_left = far_plane_center - (left * far_plane_half_width);
                const auto far_plane_top = far_plane_center + (up * far_plane_half_height);
                const auto far_plane_bottom = far_plane_center - (up * far_plane_half_height);

                const auto near_plane_center = origin + (forward * near);

                //corners
                corners[FRUSTUM_CORNER_INDEX_LEFT_TOP_NEAR] = near_plane_center + (left * near_plane_half_width) + (up * near_plane_half_height);
                corners[FRUSTUM_CORNER_INDEX_LEFT_TOP_FAR] = far_plane_center + (left * far_plane_half_width) + (up * far_plane_half_height);
                corners[FRUSTUM_CORNER_INDEX_LEFT_BOTTOM_NEAR] = near_plane_center + (left * near_plane_half_width) - (up * near_plane_half_height);
                corners[FRUSTUM_CORNER_INDEX_LEFT_BOTTOM_FAR] = far_plane_center + (left * far_plane_half_width) - (up * far_plane_half_height);
                corners[FRUSTUM_CORNER_INDEX_RIGHT_TOP_NEAR] = near_plane_center - (left * near_plane_half_width) + (up * near_plane_half_height);
                corners[FRUSTUM_CORNER_INDEX_RIGHT_TOP_FAR] = far_plane_center - (left * far_plane_half_width) + (up * far_plane_half_height);
                corners[FRUSTUM_CORNER_INDEX_RIGHT_BOTTOM_NEAR] = near_plane_center - (left * near_plane_half_width) - (up * near_plane_half_height);
                corners[FRUSTUM_CORNER_INDEX_RIGHT_BOTTOM_FAR] = far_plane_center - (left * far_plane_half_width) - (up * far_plane_half_height);

                //aabb
                aabb = corners;

                //planes
                const auto a = glm::normalize(far_plane_right - origin);
                const auto b = glm::normalize(far_plane_left - origin);
                const auto c = glm::normalize(far_plane_top - origin);
                const auto d = glm::normalize(far_plane_bottom - origin);

                const auto right_plane_normal = -glm::cross(up, a);
                const auto left_plane_normal = -glm::cross(b, up);
                const auto top_plane_normal = -glm::cross(c, left);
                const auto bottom_plane_normal = -glm::cross(left, d);

                planes[FRUSTUM_PLANE_INDEX_LEFT] = PlaneType(origin, left_plane_normal);
                planes[FRUSTUM_PLANE_INDEX_RIGHT] = PlaneType(origin, right_plane_normal);
                planes[FRUSTUM_PLANE_INDEX_TOP] = PlaneType(origin, top_plane_normal);
                planes[FRUSTUM_PLANE_INDEX_BOTTOM] = PlaneType(origin, bottom_plane_normal);
                planes[FRUSTUM_PLANE_INDEX_NEAR] = PlaneType(near_plane_center, forward);
                planes[FRUSTUM_PLANE_INDEX_FAR] = PlaneType(far_plane_center, -forward);

                //TODO: verify correctness
                //sphere
                auto view_length = far - near;
                auto far_height = view_length * glm::tan(fov / 2);
                auto far_width = far_height;

                //TODO: simplify
				auto p = VectorType(ScalarType(0), ScalarType(0), near + (view_length / 2));
                auto q = VectorType(far_width, far_height, view_length);
                auto r = q - p;

                sphere.radius = glm::length(r);
                sphere.origin = origin + (forward * (view_length / 2) + near);
            }

            const PlanesType& get_planes() const { return planes; }
            const CornersType& get_corners() const { return corners; }
            const AABBType& get_aabb() const { return aabb; }
            const SphereType& get_sphere() const { return sphere; }

        private:
            PlanesType planes;
            CornersType corners;
            AABBType aabb;
			SphereType sphere;
        };
    }

    typedef details::Frustum<f32> Frustum_f32;
    typedef details::Frustum<f64> Frustum_f64;
    typedef Frustum_f32 Frustum;
}
