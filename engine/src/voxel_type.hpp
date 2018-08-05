// std
#include <iostream>

// naga
#include "resource.hpp"
#include "material.hpp"
#include "sprite.hpp"

namespace naga
{
	struct VoxelType : Resource
	{
		VoxelType(std::istream& istream);

		std::string name;
		SpriteReference sprite;	// TODO: separate references for different sides? skinning?

	private:
		virtual ~VoxelType();
	};
}