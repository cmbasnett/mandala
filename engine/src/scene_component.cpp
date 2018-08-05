// naga
#include "scene_component.hpp"
#include "game_object.hpp"

namespace naga
{
	void SceneComponent::set_world_pose(const Pose3& world_pose)
	{
		relative_pose.location = world_pose.location - get_owner()->pose.location;
		relative_pose.rotation = world_pose.rotation;
		relative_pose.scale = world_pose.scale / get_owner()->pose.scale;
	}

	Pose3 SceneComponent::get_world_pose() const
	{
		return get_owner()->pose * relative_pose;
	}
}