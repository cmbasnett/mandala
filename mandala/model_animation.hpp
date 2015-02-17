#pragma once

//mandala
#include "md5b.hpp"
#include "resource.hpp"
#include "model_skeleton.hpp"

namespace mandala
{
	struct model_animation_t : resource_t
	{
		struct bone_t
		{
			enum flags_t : uint8_t
			{
				location_x	= (1 << 0),
				location_y	= (1 << 1),
				location_z	= (1 << 2),
				rotation_x	= (1 << 3),
				rotation_y	= (1 << 4),
				rotation_z	= (1 << 5),
			};
			
			hash_t hash;
			uint8_t parent_index = 0;
			uint8_t flags = 0;
			int32_t data_start_index = 0;
		};
		
		model_animation_t(std::istream& istream);

		std::string name;
		uint8_t frames_per_second = 0;
		uint16_t frame_count = 0;
		uint8_t bone_count = 0;
		std::vector<model_skeleton_t> frame_skeletons;

	private:
		model_animation_t(const model_animation_t&) = delete;
		model_animation_t& operator=(const model_animation_t&) = delete;
	};
}
