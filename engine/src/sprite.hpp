#pragma once

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "sprite_set.hpp"

namespace naga
{
    struct texture;

    struct sprite_ref
    {
		sprite_ref(const std::string& sprite_set_name, const hash& region_hash);
        sprite_ref(sprite_ref&& copy);

        std::string sprite_set_name;
        hash region_hash;
    };

    struct sprite
    {
        typedef boost::shared_ptr<sprite_set::region> region_type;
        typedef boost::shared_ptr<sprite_set> sprite_set_type;

        sprite(const std::string& sprite_set_name, const hash& region_hash);
        sprite(const sprite_ref& sprite_def);
        sprite(const sprite_set_type& sprite_set, const hash& region_hash);

        const sprite_set_type& get_sprite_set() const { return sprite_set; }
        const region_type& get_region() const { return region; }
        
        sprite& operator=(const sprite& rhs);
        sprite& operator=(const sprite_ref& rhs);
        bool operator==(const sprite_ref&) const;
        bool operator!=(const sprite_ref&) const;

    private:
        sprite_set_type sprite_set;
        region_type region;
    };
}
