#pragma once

// naga
#include "game_component.hpp"
#include "pose.hpp"

namespace naga
{
	struct SceneComponent : GameComponent
	{
		Pose3 relative_pose;

		void set_world_pose(const Pose3& pose);

		Pose3 get_world_pose() const;
	};
}