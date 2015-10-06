//mandala
#include "model_skeleton.hpp"

//glm
#include <glm\gtx\transform.hpp>

namespace mandala
{
    void model_skeleton_t::interpolate(model_skeleton_t& skeleton, const model_skeleton_t& a, const model_skeleton_t& b, f32 t)
    {
#if defined(DEBUG)
        assert(skeleton.bone_matrices.size() == a.bone_matrices.size());
        assert(skeleton.bone_matrices.size() == b.bone_matrices.size());
        assert(skeleton.bones.size() == a.bones.size());
        assert(skeleton.bones.size() == b.bones.size());
        assert(t >= 0.0f && t <= 1.0f);
#endif

        for(size_t i = 0; i < skeleton.bones.size(); ++i)
        {
            auto& skeleton_bone = skeleton.bones[i];
            const auto& bone_0 = a.bones[i];
            const auto& bone_1 = b.bones[i];

#if defined(DEBUG)
            assert(bone_0.parent_index == bone_1.parent_index);
#endif

            skeleton_bone.parent_index = bone_0.parent_index;
            skeleton_bone.pose.location = glm::mix(bone_0.pose.location, bone_1.pose.location, t);
            skeleton_bone.pose.rotation = glm::slerp(bone_0.pose.rotation, bone_1.pose.rotation, t);

            skeleton.bone_matrices[i] = skeleton_bone.pose.to_matrix();
        }

        skeleton.aabb = aabb3::join(a.aabb, b.aabb);
    }
}
