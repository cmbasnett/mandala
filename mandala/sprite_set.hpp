#pragma once

//std
#include <vector>
#include <map>
#include <memory>

//mandala
#include "resource.hpp"
#include "rectangle.hpp"
#include "aabb.hpp"

namespace mandala
{
	struct texture_t;

	struct sprite_set_t : resource_t
	{
		struct region_t
		{
			typedef rectangle_i16_t rectangle_type;
            typedef vec2_i16_t size_type;
            typedef aabb2_t uv_type;

			hash_t hash;
            rectangle_type frame_rectangle;
            rectangle_type rectangle;
            size_type source_size;
			bool is_rotated = false;
			bool is_trimmed = false;
            uv_type uv;
		};

		std::shared_ptr<texture_t> texture;
		std::map<const hash_t, region_t> regions;
		std::vector<const hash_t> region_hashes;

		sprite_set_t(std::istream& istream);

	private:
		sprite_set_t(const sprite_set_t&) = delete;
		sprite_set_t& operator=(const sprite_set_t&) = delete;
	};
}
