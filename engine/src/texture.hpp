#pragma once

//mandala
#include "resource.hpp"
#include "color_types.hpp"

//boost
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>

namespace mandala
{
    struct image;

    struct texture : resource, boost::enable_shared_from_this<texture>
    {
        typedef u32 size_value_type;
        typedef glm::detail::tvec2<size_value_type> size_type;
        typedef u32 id_type;
        typedef i32 format_type;
        typedef i32 type_type;

        texture(color_type color_type, const size_type& size, const void* data);
        texture(const boost::shared_ptr<image>& image);
        texture(std::istream& istream);
        virtual ~texture();

        color_type get_color_type() const { return color_type; }
        const size_type& get_size() const { return size; }
        id_type get_id() const { return id; }

        void set_size(const size_type& size);

    private:
        color_type color_type;
        size_type size;
        id_type id = 0;

        texture(texture&) = delete;
        texture& operator=(texture&) = delete;
    };
}
