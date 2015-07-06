#pragma once

//mandala
#include "types.hpp"

namespace mandala
{
    namespace details
    {
        template<typename T>
        struct pose_t
        {
            typedef glm::detail::tquat<T> rotation_type;
            typedef glm::detail::tmat4x4<T> matrix_type;
        };

        template<typename T>
        struct pose2_t : pose_t<T>
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
        struct pose3_t : pose_t<T>
        {
            typedef glm::detail::tvec3<T> location_type;

            location_type location;
            rotation_type rotation;

            inline matrix_type to_matrix() const
            {
                return glm::translate(location) * glm::toMat4(rotation);
            }
        };
    }

    typedef details::pose2_t<float32_t> pose2_f32;
    typedef details::pose2_t<float64_t> pose2_f64;
    typedef pose2_f32 pose2;

    typedef details::pose3_t<float32_t> pose3_f32;
    typedef details::pose3_t<float64_t> pose3_f64;
    typedef pose3_f32 pose3;
}
