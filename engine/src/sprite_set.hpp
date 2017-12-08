#pragma once

//std
#include <vector>
#include <map>
#include <memory>

//naga
#include "resource.hpp"
#include "rectangle.hpp"
#include "aabb.hpp"

//boost
#include <boost/optional.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace naga
{
	struct Texture;

	struct SpriteSet : Resource, boost::enable_shared_from_this<SpriteSet>
    {
        struct Region
        {
            std::string name;
			Rectangle frame_rectangle;
			Rectangle rectangle;
			vec2 source_size;
            bool is_rotated = false;
            bool is_trimmed = false;
			AABB2 frame_uv;
			AABB2 uv;
        };

        typedef std::map<const std::string, Region> RegionsType;

		SpriteSet(std::istream& istream);
		SpriteSet(const boost::shared_ptr<Texture>& texture);
		SpriteSet(const std::string& texture_name);

		const boost::shared_ptr<Texture>& get_texture() const { return texture; }
		const RegionsType& get_regions() const { return regions; }

		bool has_region(const std::string& region_name) const;
		const SpriteSet::Region& get_region(const std::string& region_name) const;

    private:
		boost::shared_ptr<Texture> texture;
		RegionsType regions;

		SpriteSet(const SpriteSet&) = delete;
		SpriteSet& operator=(const SpriteSet&) = delete;
    };
}
