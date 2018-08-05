#pragma once

// naga
#include "scene_component.hpp"

namespace naga
{
	struct VoxelMap;

	struct VoxelMapComponent : SceneComponent
	{
		// TODO: VBOs for each chunk! have a VBO pool mayhaps

		const std::shared_ptr<VoxelMap>& get_map() const { return map; }

		virtual void on_render(CameraParameters& camera_parameters) override;

	private:
		std::shared_ptr<VoxelMap> map;
	};
}