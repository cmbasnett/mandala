#pragma once

#include "types.hpp"

#include <vector>
#include <array>

namespace mandala
{
    template<typename Scalar = std::enable_if<std::is_floating_point<Scalar>::value>::type>
    inline glm::detail::tvec2<Scalar> bezier(const glm::detail::tvec2<Scalar>& point0, const glm::detail::tvec2<Scalar>& point1, const glm::detail::tvec2<Scalar>& point2, Scalar t)
    {
        return point0 + ((point2 - point1) * t - (point1 - point0) * t) * t;
    }

    template<typename Scalar = std::enable_if<std::is_floating_point<Scalar>::value>::type>
    inline glm::detail::tvec3<Scalar> bezier(const glm::detail::tvec3<Scalar>& point0, const glm::detail::tvec3<Scalar>& point1, const glm::detail::tvec3<Scalar>& point2, Scalar t)
    {
        return point0 + ((point2 - point1) * t - (point1 - point0) * t) * t;
    }
}
