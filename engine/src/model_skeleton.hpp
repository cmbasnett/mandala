#pragma once

//naga
#include "md5b.hpp"
#include "aabb.hpp"
#include "pose.hpp"

//std
#include <vector>

namespace naga
{
    struct ModelSkeleton
    {
		struct Bone
        {
            u8 parent_index = 0;
            Pose3 pose;
        };

		void interpolate(const ModelSkeleton& a, const ModelSkeleton& b, f32 t);

        std::vector<Bone> bones;
		std::vector<mat4> bone_matrices;
		AABB3 aabb;
    };
}
