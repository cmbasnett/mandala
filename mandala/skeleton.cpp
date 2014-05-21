//mandala
#include "skeleton.h"

//glm
#include <glm\gtx\transform.hpp>

namespace mandala
{
	void skeleton_t::interpolate(skeleton_t& skeleton, const skeleton_t& a, const skeleton_t& b, float32_t t)
	{
#ifdef _DEBUG
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
#ifdef _DEBUG
			assert(bone_0.parent_index == bone_1.parent_index);
#endif
			skeleton_bone.parent_index = bone_0.parent_index;
			skeleton_bone.position = glm::mix(bone_0.position, bone_1.position, t);
			skeleton_bone.orientation = glm::slerp(bone_0.orientation, bone_1.orientation, t);

			skeleton.bone_matrices[i] = glm::translate(skeleton_bone.position) * glm::toMat4(skeleton_bone.orientation);
		}

		skeleton.aabb = aabb3_t::join(a.aabb, b.aabb);
	}
};
