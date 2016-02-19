//std
#include <array>

//naga
#include "heightmap.hpp"

namespace naga
{
	f32 heightmap::get_height(f32 x, f32 z) const
	{
		//TODO: indices must be less than the width/depth values (clamp to width - 1 etc.)
		auto x_index = glm::clamp(static_cast<u32>(glm::max(x, 0.0f)), 0U, width - 1);
		auto z_index = glm::clamp(static_cast<u32>(glm::max(z, 0.0f)), 0U, depth - 1);

        // z
        // ^ 3---4
        // | | 2 |
        // | 0---1
        // +---- > x
		const std::array<f32, 5> heights = {
			outer[x_index + 0][z_index + 0],
			outer[x_index + 1][z_index + 0],
			inner[x_index + 0][z_index + 0],
			outer[x_index + 0][z_index + 1],
			outer[x_index + 1][z_index + 1],
		};
	}
	
	f32 heightmap::get_height(const vec2& location) const
	{
		return get_height(location.x, location.y);
	}
}