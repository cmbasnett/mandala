#pragma once

//std
#include <map>
#include <vector>
#include <string>

//glm
#include <glm\ext.hpp>

namespace tpb
{
    struct sprite_set_t
    {
        struct region_t
        {
            struct rectangle_t
            {
                typedef glm::i16 scalar_type;

                scalar_type x = 0;
                scalar_type y = 0;
                scalar_type width = 0;
                scalar_type height = 0;
            };

            struct aabb_t
            {
                typedef glm::vec2 vector_type;

                vector_type min;
                vector_type max;
            };

            typedef glm::u8 flags_type;

            typedef rectangle_t rectangle_type;
            typedef glm::i16vec2 size_type;
            typedef aabb_t uv_type;

            std::string name;
            rectangle_type frame_rectangle;
            rectangle_type rectangle;
            size_type source_size;
            uv_type uv;
            bool is_rotated = false;
            bool is_trimmed = false;
        };

        std::string texture;
        std::vector<const region_t> regions;

        sprite_set_t(std::istream& istream);

    private:
        sprite_set_t(const sprite_set_t&) = delete;
        sprite_set_t& operator=(const sprite_set_t&) = delete;
    };

    std::ostream& operator<<(std::ostream&, const sprite_set_t&);
}
