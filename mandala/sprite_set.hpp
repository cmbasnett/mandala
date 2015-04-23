#pragma once

//std
#include <vector>
#include <map>
#include <memory>

//mandala
#include "resource.hpp"
#include "rectangle.hpp"
#include "aabb.hpp"

//boost
#include <boost/optional.hpp>

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

        typedef std::map<const hash_t, const region_t> regions_type;

		sprite_set_t(std::istream& istream);
		sprite_set_t(const boost::shared_ptr<texture_t>& texture);	//TODO: ugly, feels like this needs to be elsewhere in a helper class

        const boost::shared_ptr<texture_t>& get_texture() const { return texture; }
        const regions_type& get_regions() const { return regions; }

        boost::optional<region_t> get_region(const hash_t& region_hash) const;

	private:
        boost::shared_ptr<texture_t> texture;
        regions_type regions;

		sprite_set_t(const sprite_set_t&) = delete;
		sprite_set_t& operator=(const sprite_set_t&) = delete;
	};
}
