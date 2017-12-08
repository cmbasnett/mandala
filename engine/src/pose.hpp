#pragma once

//naga
#include "types.hpp"

namespace naga
{
    namespace details
    {
        template<typename T>
        struct Pose
        {
            typedef glm::detail::tquat<T> RotationType;
            typedef glm::detail::tmat4x4<T> MatrixType;
        };

        template<typename T>
		struct Pose2 : Pose<T>
        {
            typedef glm::detail::tvec2<T> LocationType;

            LocationType location;
            RotationType rotation;

            inline MatrixType to_matrix() const
            {
                return glm::translate(location.x location.y, T(0)) * glm::toMat4(rotation);
            }
        };

        template<typename T>
        struct Pose3 : Pose<T>
        {
            typedef glm::detail::tvec3<T> LocationType;
			typedef Pose3<T> Type;

			Pose3() = default;
			Pose3(const MatrixType& matrix) :
				location(glm::swizzle<glm::X, glm::Y, glm::Z>(glm::column(matrix, 3))),
				rotation(matrix)
			{
				//TODO: verify correctness
			}

            LocationType location;
            RotationType rotation;

            inline MatrixType to_matrix() const
            {
                return glm::translate(location) * glm::toMat4(rotation);
			}

			Type& operator *=(const Type& rhs)
			{
				*this = *this * rhs;
				return *this;
			}
        };
	}

	template<typename T>
	details::Pose3<T> operator *(const details::Pose3<T>& lhs, const details::Pose3<T>& rhs)
	{
		return details::Pose3<T>(lhs.to_matrix() * rhs.to_matrix());
	}

	typedef details::Pose2<f32> Pose2;
    typedef details::Pose3<f32> Pose3;
}
