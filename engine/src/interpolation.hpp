#pragma once

//std
#include <vector>
#include <array>

//mandala
#include "types.hpp"

namespace mandala
{
    template<typename Scalar = std::enable_if<std::is_floating_point<Scalar>::value>::type>
    inline glm::detail::tvec2<Scalar> bezier2(const glm::detail::tvec2<Scalar>& p0, const glm::detail::tvec2<Scalar>& p1, const glm::detail::tvec2<Scalar>& p2, Scalar t)
    {
        const auto q0 = (p1 - p0) * t;
        const auto q1 = (p2 - p1) * t;
        return p0 + q0 + ((q1 - q0) * t);
    }

    template<typename Scalar = std::enable_if<std::is_floating_point<Scalar>::value>::type>
    inline glm::detail::tvec3<Scalar> bezier3(const glm::detail::tvec3<Scalar>& p0, const glm::detail::tvec3<Scalar>& p1, const glm::detail::tvec3<Scalar>& p2, Scalar t)
    {
        const auto q0 = (p1 - p0) * t;
        const auto q1 = (p2 - p1) * t;
        return p0 + q0 + ((q1 - q0) * t);
    }
}
