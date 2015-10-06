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
    struct model_skeleton_t
    {
        typedef mat4 matrix_type;

        struct bone_t
        {
            u8 parent_index = 0;
            pose3_f32 pose;
        };

        static void interpolate(model_skeleton_t& skeleton, const model_skeleton_t& a, const model_skeleton_t& b, f32 t);

        std::vector<bone_t> bones;
        std::vector<matrix_type> bone_matrices;
        aabb3 aabb;
    };
}
