#pragma once

//std
#include <memory>
#include <deque>

//mandala
#include "hash.hpp"
#include "model_skeleton.hpp"

namespace mandala
{
	struct model_animation_t;

	struct model_animation_controller_t
	{
		struct task_t
		{
			enum class type_e
			{
				play,
				oscillate,
				loop
			};

			type_e type;
		};

		void play(const hash_t& animation_hash);
		void pause();
		void tick(float32_t dt);

	private:
		std::shared_ptr<model_animation_t> animation;
		model_skeleton_t skeleton;
		std::deque<task_t> tasks;
	};
}
