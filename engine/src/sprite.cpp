//mandala
#include "resource_mgr.hpp"
#include "sprite.hpp"
#include "sprite_set.hpp"
#include "texture.hpp"

namespace mandala
{
    sprite_ref_t::sprite_ref_t(const hash_t& sprite_set_hash, const hash_t& region_hash) :
        sprite_set_hash(sprite_set_hash),
        region_hash(region_hash)
    {
    }

    sprite_ref_t::sprite_ref_t(sprite_ref_t&& copy) :
        sprite_set_hash(std::move(copy.sprite_set_hash)),
        region_hash(std::move(copy.region_hash))
    {
    }

    sprite_t::sprite_t(const hash_t& sprite_set_hash, const hash_t& region_hash) :
        sprite_t(resources.get<sprite_set_t>(sprite_set_hash), region_hash)
    {
    }

    sprite_t::sprite_t(const sprite_ref_t& sprite_ref) :
        sprite_t(sprite_ref.sprite_set_hash, sprite_ref.region_hash)
    {
    }

    sprite_t::sprite_t(const sprite_set_type& sprite_set, const hash_t& region_hash) :
        sprite_set(sprite_set)
    {
        if (sprite_set == nullptr)
        {
            throw std::invalid_argument("sprite set cannot be null");
        }

        const auto region_optional = sprite_set->get_region(region_hash);

        if (!region_optional)
        {
            throw std::invalid_argument("region does not exist in sprite set");
        }

        region = region_optional.get();
    }

        sprite_t& sprite_t::operator=(const sprite_t& rhs)
    {
        region = rhs.region;
        sprite_set = rhs.sprite_set;

        return *this;
    }

    sprite_t& sprite_t::operator=(const sprite_ref_t& rhs)
    {
        sprite_set = resources.get<sprite_set_t>(rhs.sprite_set_hash);

        const auto region_optional = sprite_set->get_region(rhs.region_hash);

        if (!region_optional)
        {
            throw std::exception("region does not exist in sprite set");
        }

        region = region_optional.get();

        return *this;
    }

    bool sprite_t::operator==(const sprite_ref_t& sprite_ref) const
    {
        return region.hash == sprite_ref.region_hash && sprite_set->hash == sprite_ref.sprite_set_hash;
    }

    bool sprite_t::operator!=(const sprite_ref_t& sprite_ref) const
    {
        return region.hash != sprite_ref.region_hash || sprite_set->hash != sprite_ref.sprite_set_hash;
    }
}
