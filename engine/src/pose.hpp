#pragma once

//naga
#include "types.hpp"

namespace naga
{
    namespace details
    {
        template<typename T>
        struct pose
        {
            typedef glm::detail::tquat<T> rotation_type;
            typedef glm::detail::tmat4x4<T> matrix_type;
        };

        template<typename T>
        struct pose2 : pose<T>
        {
            typedef glm::detail::tvec2<T> location_type;

            location_type location;
            rotation_type rotation;

            inline matrix_type to_matrix() const
            {
                return glm::translate(location.x location.y, T(0)) * glm::toMat4(rotation);
            }
        };

        template<typename T>
        struct pose3 : pose<T>
        {
            typedef glm::detail::tvec3<T> location_type;
			typedef pose3<T> type;

			pose3() = default;
			pose3(const matrix_type& matrix) :
				location(glm::swizzle<glm::X, glm::Y, glm::Z>(glm::column(matrix, 3))),
				rotation(matrix)
			{
				//TODO: verify correctness
			}

            location_type location;
            rotation_type rotation;

            inline matrix_type to_matrix() const
            {
                return glm::translate(location) * glm::toMat4(rotation);
			}

			type& operator *=(const type& rhs)
			{
				*this = *this * rhs;
				return *this;
			}
        };
	}

	template<typename T>
	details::pose3<T> operator *(const details::pose3<T>& lhs, const details::pose3<T>& rhs)
	{
		return details::pose3<T>(lhs.to_matrix() * rhs.to_matrix());
	}

    typedef details::pose2<f32> pose2_f32;
    typedef details::pose2<f64> pose2_f64;
    typedef pose2_f32 pose2;

    typedef details::pose3<f32> pose3_f32;
    typedef details::pose3<f64> pose3_f64;
    typedef pose3_f32 pose3;
}
