#pragma once

//std
#include <array>

//mandala
#include "types.hpp"
#include "plane.hpp"
#include "aabb.hpp"
#include "sphere.hpp"

//glm
#include <glm\gtc\matrix_access.hpp>

namespace mandala
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
		template<typename Scalar, typename Enable = void>
		struct frustum_t;

		template<typename Scalar>
		struct frustum_t<Scalar, typename std::enable_if<std::is_floating_point<Scalar>::value>::type>
		{
			typedef Scalar scalar_type;
			typedef glm::detail::tvec3<scalar_type> vector_type;
			typedef frustum_t<scalar_type> type;
			typedef details::plane3_t<scalar_type> plane_type;
			typedef glm::detail::tmat4x4<scalar_type> matrix_type;
			typedef std::array<plane_type, FRUSTUM_PLANE_COUNT> planes_type;
			typedef std::array<vector_type, FRUSTUM_CORNER_COUNT> corners_type;
			typedef details::aabb3_t<scalar_type> aabb_type;
			typedef details::sphere_t<scalar_type> sphere_type;

			frustum_t() = default;

			void set(const vector_type& origin, const vector_type& left, const vector_type& up, const vector_type& forward, scalar_type fov, scalar_type near, scalar_type far, scalar_type aspect)
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

                planes[FRUSTUM_PLANE_INDEX_LEFT] = plane_type(origin, left_plane_normal);
                planes[FRUSTUM_PLANE_INDEX_RIGHT] = plane_type(origin, right_plane_normal);
                planes[FRUSTUM_PLANE_INDEX_TOP] = plane_type(origin, top_plane_normal);
                planes[FRUSTUM_PLANE_INDEX_BOTTOM] = plane_type(origin, bottom_plane_normal);
                planes[FRUSTUM_PLANE_INDEX_NEAR] = plane_type(near_plane_center, forward);
                planes[FRUSTUM_PLANE_INDEX_FAR] = plane_type(far_plane_center, -forward);

				//TODO: verify correctness
				//sphere
				auto view_length = far - near;
				auto far_height = view_length * glm::tan(fov / 2);
				auto far_width = far_height;

				//TODO: simplify
				auto p = vector_type(scalar_type(0), scalar_type(0), near + (view_length / 2));
				auto q = vector_type(far_width, far_height, view_length);
				auto r = q - p;

				sphere.radius = glm::length(r);
				sphere.origin = origin + (forward * (view_length / 2) + near);
			}

			const planes_type& get_planes() const { return planes; }
			const corners_type& get_corners() const { return corners; }
			const aabb_type& get_aabb() const { return aabb; }
			const sphere_type& get_sphere() const { return sphere; }

		private:
			planes_type planes;
			corners_type corners;
			aabb_type aabb;
			sphere_type sphere;
		};
	}

	typedef details::frustum_t<float32_t> frustum_f32_t;
	typedef details::frustum_t<float64_t> frustum_f64_t;
	typedef frustum_f32_t frustum_t;
}
