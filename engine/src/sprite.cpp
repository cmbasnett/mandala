//naga
#include "resource_mgr.hpp"
#include "sprite.hpp"
#include "sprite_set.hpp"
#include "texture.hpp"

namespace naga
{
	sprite_ref::sprite_ref(const std::string& sprite_set_name, const hash& region_hash) :
		sprite_set_name(sprite_set_name),
        region_hash(region_hash)
    {
    }

    sprite_ref::sprite_ref(sprite_ref&& copy) :
		sprite_set_name(std::move(copy.sprite_set_name)),
        region_hash(std::move(copy.region_hash))
    {
    }

    sprite::sprite(const std::string& sprite_set_name, const hash& region_hash) :
		sprite(resources.get<naga::sprite_set>(sprite_set_name), region_hash)
    {
    }

    sprite::sprite(const sprite_ref& sprite_ref) :
		sprite(sprite_ref.sprite_set_name, sprite_ref.region_hash)
    {
    }

    sprite::sprite(const sprite_set_type& sprite_set, const hash& region_hash) :
        sprite_set(sprite_set)
    {
        if (sprite_set == nullptr)
        {
            throw std::invalid_argument("sprite set cannot be null");
        }

        region = sprite_set->get_region(region_hash);

        if (!region)
        {
            throw std::invalid_argument("region does not exist in sprite set");
        }
    }

        sprite& sprite::operator=(const sprite& rhs)
    {
        region = rhs.region;
        sprite_set = rhs.sprite_set;

        return *this;
    }

    sprite& sprite::operator=(const sprite_ref& rhs)
    {
        sprite_set = resources.get<naga::sprite_set>(rhs.sprite_set_name);

        auto region = sprite_set->get_region(rhs.region_hash);

        if (!region)
        {
            throw std::exception("region does not exist in sprite set");
        }

        return *this;
    }

    bool sprite::operator==(const sprite_ref& sprite_ref) const
    {
		return region->hash == sprite_ref.region_hash&& sprite_set->hash == hash(sprite_ref.sprite_set_name);
    }

    bool sprite::operator!=(const sprite_ref& sprite_ref) const
    {
		return region->hash != sprite_ref.region_hash || sprite_set->hash != hash(sprite_ref.sprite_set_name);
    }
}
