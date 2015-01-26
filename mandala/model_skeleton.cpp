//mandala
#include "model_skeleton.hpp"

//glm
#include <glm\gtx\transform.hpp>

namespace mandala
{
	void model_skeleton_t::interpolate(model_skeleton_t& skeleton, const model_skeleton_t& a, const model_skeleton_t& b, float32_t t)
	{
#if defined(DEBUG)
		assert(skeleton.bone_matrices.size() == a.bone_matrices.size());
		assert(skeleton.bone_matrices.size() == b.bone_matrices.size());
		assert(skeleton.bones.size() == a.bones.size());
		assert(skeleton.bones.size() == b.bones.size());
		assert(t >= 0.0f && t < 1.0f);
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
			skeleton_bone.location = glm::mix(bone_0.location, bone_1.location, t);
			skeleton_bone.rotation = glm::slerp(bone_0.rotation, bone_1.rotation, t);

			skeleton.bone_matrices[i] = glm::translate(skeleton_bone.location) * glm::toMat4(skeleton_bone.rotation);
		}

		skeleton.aabb = aabb3_t::join(a.aabb, b.aabb);
	}
}
