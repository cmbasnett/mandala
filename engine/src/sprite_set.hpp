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

namespace naga
{
    struct texture;

    struct sprite_set : resource
    {
        struct region
        {
            typedef rectangle_i16 rectangle_type;
            typedef vec2_i16 size_type;
            typedef aabb2 uv_type;

            naga::hash hash;
            rectangle_type frame_rectangle;
            rectangle_type rectangle;
            size_type source_size;
            bool is_rotated = false;
            bool is_trimmed = false;
            uv_type frame_uv;
            uv_type uv;
        };

        typedef std::map<const naga::hash, boost::shared_ptr<region>> regions_type;

        sprite_set(std::istream& istream);
        sprite_set(const boost::shared_ptr<texture>& texture);    //TODO: ugly, feels like this needs to be elsewhere in a helper class

        const boost::shared_ptr<texture>& get_texture() const { return texture; }
        const regions_type& get_regions() const { return regions; }

        boost::shared_ptr<region> get_region(const naga::hash& region_hash) const;

    private:
        boost::shared_ptr<texture> texture;
        regions_type regions;

        sprite_set(const sprite_set&) = delete;
        sprite_set& operator=(const sprite_set&) = delete;
    };
}
