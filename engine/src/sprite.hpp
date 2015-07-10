#pragma once

//boost
#include <boost\shared_ptr.hpp>

//mandala
#include "sprite_set.hpp"

namespace mandala
{
    struct texture_t;

    struct sprite_ref_t
    {
        sprite_ref_t(const hash_t& sprite_set_hash, const hash_t& region_hash);
        sprite_ref_t(sprite_ref_t&& copy);

        hash_t sprite_set_hash;
        hash_t region_hash;
    };

    struct sprite_t
    {
        typedef sprite_set_t::region_t region_type;
        typedef boost::shared_ptr<sprite_set_t> sprite_set_type;

        sprite_t(const hash_t& sprite_set_hash, const hash_t& region_hash);
        sprite_t(const sprite_ref_t& sprite_def);
        sprite_t(const sprite_set_type& sprite_set, const hash_t& region_hash);

        const sprite_set_type& get_sprite_set() const { return sprite_set; }
        const region_type& get_region() const { return region; }
        
        sprite_t& operator=(const sprite_t& rhs);
        sprite_t& operator=(const sprite_ref_t& rhs);
        bool operator==(const sprite_ref_t&) const;
        bool operator!=(const sprite_ref_t&) const;

    private:
        sprite_set_type sprite_set;
        region_type region;
    };
}
