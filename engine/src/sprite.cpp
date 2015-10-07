//mandala
#include "resource_mgr.hpp"
#include "sprite.hpp"
#include "sprite_set.hpp"
#include "texture.hpp"

namespace mandala
{
    sprite_ref::sprite_ref(const hash& sprite_set_hash, const hash& region_hash) :
        sprite_set_hash(sprite_set_hash),
        region_hash(region_hash)
    {
    }

    sprite_ref::sprite_ref(sprite_ref&& copy) :
        sprite_set_hash(std::move(copy.sprite_set_hash)),
        region_hash(std::move(copy.region_hash))
    {
    }

    sprite::sprite(const hash& sprite_set_hash, const hash& region_hash) :
        sprite(resources.get<mandala::sprite_set>(sprite_set_hash), region_hash)
    {
    }

    sprite::sprite(const sprite_ref& sprite_ref) :
        sprite(sprite_ref.sprite_set_hash, sprite_ref.region_hash)
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
        sprite_set = resources.get<mandala::sprite_set>(rhs.sprite_set_hash);

        auto region = sprite_set->get_region(rhs.region_hash);

        if (!region)
        {
            throw std::exception("region does not exist in sprite set");
        }

        return *this;
    }

    bool sprite::operator==(const sprite_ref& sprite_ref) const
    {
        return region->hash == sprite_ref.region_hash&& sprite_set->hash == sprite_ref.sprite_set_hash;
    }

    bool sprite::operator!=(const sprite_ref& sprite_ref) const
    {
        return region->hash!= sprite_ref.region_hash|| sprite_set->hash!= sprite_ref.sprite_set_hash;
    }
}
