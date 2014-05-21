#pragma once

//std
#include <vector>
#include <map>
#include <memory>

//mandala
#include "resource.h"
#include "rectangle.h"
#include "aabb.h"

namespace mandala
{
	struct texture_t;

	struct sprite_set_t : resource_t
	{
		struct region_t
		{
			typedef rectangle_i16_t rectangle_type_t;
			typedef vec2_i16_t size_type_t;
			typedef aabb2_t uv_type_t;

			hash_t hash;
			rectangle_type_t frame_rectangle;
			rectangle_type_t rectangle;
			size_type_t source_size;
			bool is_rotated = false;
			bool is_trimmed = false;
			uv_type_t uv;
		};

		std::shared_ptr<texture_t> texture;
		std::map<const hash_t, region_t> regions;
		std::vector<const hash_t> region_hashes;

		sprite_set_t(std::istream& istream);

	private:
		sprite_set_t(const sprite_set_t&) = delete;
		sprite_set_t& operator=(const sprite_set_t&) = delete;
	};
};
