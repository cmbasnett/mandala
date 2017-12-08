#pragma once

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "sprite_set.hpp"

namespace naga
{
    struct Texture;

    struct SpriteReference
    {
		SpriteReference(const std::string& sprite_set_name, const std::string& region_name);
		SpriteReference(SpriteReference&& copy);

        std::string sprite_set_name;
		std::string region_name;
    };

    struct Sprite
    {
		typedef SpriteSet::Region RegionType;
		typedef boost::shared_ptr<SpriteSet> SpriteSetType;

		Sprite(const std::string& sprite_set_name, const std::string& region_name);
		Sprite(const SpriteReference& sprite_reference);
		Sprite(const SpriteSetType& sprite_set, const std::string& region_name);
		Sprite(const boost::shared_ptr<Texture>& texture);

		const RegionType& get_region() const { return region; }
		const boost::shared_ptr<Texture>& get_texture() const { return texture; }
        
		Sprite& operator=(const Sprite& rhs);
		Sprite& operator=(const SpriteReference& rhs);
		//bool operator==(const SpriteReference&) const;
		//bool operator!=(const SpriteReference&) const;

    private:
		boost::shared_ptr<Texture> texture;
		RegionType region;
    };
}
