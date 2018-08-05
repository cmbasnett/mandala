// naga
#include "voxel_map_component.hpp"
#include "camera_params.hpp"
#include "voxel_map.hpp"

namespace naga
{
	VoxelMapComponent::VoxelMapComponent() :
		map(std::make_shared<VoxelMap>())
	{
	}

	void VoxelMapComponent::on_render(CameraParameters& camera_parameters)
	{
		if (map != nullptr)
		{
			map->render(camera_parameters);
		}
	}
}
