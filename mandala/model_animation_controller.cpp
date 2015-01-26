//mandala
#include "resource_mgr.hpp"
#include "model_animation.hpp"
#include "model_animation_controller.hpp"

namespace mandala
{
	void model_animation_controller_t::play(const hash_t& animation_hash)
	{
		task_t task;
		task.type = task_t::type_e::play;

		try
		{
			auto animation = resources.get<model_animation_t>(animation_hash);
		}
		catch (...)
		{
			std::cerr << "cannot play animation " << animation_hash << ", resource does not exist";

			return;
		}
	}

	void model_animation_controller_t::tick(float32_t dt)
	{
		/*if (animation != nullptr)
		{
			auto frame_count = static_cast<float32_t>(animation->frame_count);
			auto seconds_per_frame = 1.0f / animation->frames_per_second;
			auto frame_0_index = static_cast<size_t>(glm::floor(glm::mod(t / seconds_per_frame, frame_count)));
			auto frame_1_index = (frame_0_index + 1) % animation->frame_count;
			auto interpolate_t = glm::mod(t, seconds_per_frame) / seconds_per_frame;
			auto& frame_skeleton_0 = animation->frame_skeletons[frame_0_index];
			auto& frame_skeleton_1 = animation->frame_skeletons[frame_1_index];

			model_skeleton_t::interpolate(skeleton, frame_skeleton_0, frame_skeleton_1, interpolate_t);
		}*/
	}
}
