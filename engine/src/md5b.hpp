#pragma once

//naga
#include "types.hpp"

namespace naga
{
    namespace md5b
    {
        inline void compute_quaternion_w(quat& quaternion)
        {
            auto t = 1.0f - ((quaternion.x * quaternion.x) + (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z));

            quaternion.w = t < 0.0f ? 0.0f : -glm::sqrt(t);
        }
    }
}
