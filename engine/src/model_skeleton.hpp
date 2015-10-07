#pragma once

//mandala
#include "md5b.hpp"
#include "aabb.hpp"
#include "hash.hpp"
#include "pose.hpp"

//std
#include <vector>

namespace mandala
{
    struct model_skeleton
    {
        typedef mat4 matrix_type;

        struct bone
        {
            u8 parent_index = 0;
            pose3_f32 pose;
        };

        static void interpolate(model_skeleton& skeleton, const model_skeleton& a, const model_skeleton& b, f32 t);

        std::vector<bone> bones;
        std::vector<matrix_type> bone_matrices;
        aabb3 aabb;
    };
}
