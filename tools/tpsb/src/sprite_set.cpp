//std
#include <array>

//tpb
#include "sprite_set.hpp"

//boost
#include <boost\property_tree\json_parser.hpp>

namespace tpb
{
    sprite_set_t::sprite_set_t(std::istream& istream)
    {
        using namespace boost::property_tree;

        ptree ptree_;

        read_json(istream, ptree_);

        //meta
        auto meta_ptree = ptree_.get_child("meta");

        //texture
        texture = meta_ptree.get<std::string>("image");

        //size
        auto size_ptree = meta_ptree.get_child("size");

        glm::i16vec2 texture_size;

        texture_size.x = size_ptree.get<glm::i16vec2::value_type>("w");
        texture_size.y = size_ptree.get<glm::i16vec2::value_type>("h");

        //sprites
        auto regions_ptree = ptree_.get_child("frames");

        for (auto& region_ptree : regions_ptree)
        {
            region_t region;

            //name
            region.name = region_ptree.second.get<std::string>("filename");

            //frame_recangle
            auto frame_rectangle_ptree = region_ptree.second.get_child("frame");

            region.frame_rectangle.x = frame_rectangle_ptree.get<region_t::rectangle_type::scalar_type>("x");
            region.frame_rectangle.y = frame_rectangle_ptree.get<region_t::rectangle_type::scalar_type>("y");
            region.frame_rectangle.width = frame_rectangle_ptree.get<region_t::rectangle_type::scalar_type>("w");
            region.frame_rectangle.height = frame_rectangle_ptree.get<region_t::rectangle_type::scalar_type>("h");

            //is_rotated
            region.is_rotated = region_ptree.second.get<bool>("rotated");

            //is_trimmed
            region.is_trimmed = region_ptree.second.get<bool>("trimmed");

            //source_size
            auto source_size_ptree = region_ptree.second.get_child("sourceSize");

            region.source_size.x = source_size_ptree.get<region_t::size_type::value_type>("w");
            region.source_size.y = source_size_ptree.get<region_t::size_type::value_type>("h");

            //rectangle
            auto rectangle_ptree = region_ptree.second.get_child("spriteSourceSize");

            region.rectangle.x = rectangle_ptree.get<region_t::rectangle_type::scalar_type>("x");
            region.rectangle.y = region.source_size.y - rectangle_ptree.get<region_t::rectangle_type::scalar_type>("y") - region.frame_rectangle.height;
            region.rectangle.width = rectangle_ptree.get<region_t::rectangle_type::scalar_type>("w");
            region.rectangle.height = rectangle_ptree.get<region_t::rectangle_type::scalar_type>("h");

            if (region.is_rotated)
            {
                std::swap(region.frame_rectangle.width, region.frame_rectangle.height);
            }

            //uv
            region.uv.min.x = static_cast<glm::f32>(region.frame_rectangle.x) / texture_size.x;
            region.uv.min.y = static_cast<glm::f32>(texture_size.y - region.frame_rectangle.y - region.frame_rectangle.height) / texture_size.y;
            region.uv.max.x = static_cast<glm::f32>(region.frame_rectangle.x + region.frame_rectangle.width) / texture_size.x;
            region.uv.max.y = static_cast<glm::f32>(texture_size.y - region.frame_rectangle.y) / texture_size.y;

            regions.push_back(region);
        }
    }

    std::ostream& operator<<(std::ostream& ostream, const sprite_set_t& sprite_set)
    {
        static const std::array<char, 4> magic = { 'T', 'P', 'S', 'B' };
        static const int version = 1;

        //magic
        ostream.write(magic.data(), magic.size());

        //version
        ostream.write(reinterpret_cast<const char*>(&version), sizeof(version));

        //texture
        ostream.write(sprite_set.texture.c_str(), sprite_set.texture.size() + 1);

        //region count
        glm::u16 region_count = static_cast<glm::u16>(sprite_set.regions.size());

        ostream.write(reinterpret_cast<const char*>(&region_count), sizeof(region_count));

        //regions
        for (const auto& region : sprite_set.regions)
        {
            //name
            ostream.write(region.name.c_str(), region.name.size() + 1);

            //frame rectangle
            ostream.write(reinterpret_cast<const char*>(&region.frame_rectangle.x), sizeof(region.frame_rectangle.x));
            ostream.write(reinterpret_cast<const char*>(&region.frame_rectangle.y), sizeof(region.frame_rectangle.y));
            ostream.write(reinterpret_cast<const char*>(&region.frame_rectangle.width), sizeof(region.frame_rectangle.width));
            ostream.write(reinterpret_cast<const char*>(&region.frame_rectangle.height), sizeof(region.frame_rectangle.height));

            //rectangle
            ostream.write(reinterpret_cast<const char*>(&region.rectangle.x), sizeof(region.rectangle.x));
            ostream.write(reinterpret_cast<const char*>(&region.rectangle.y), sizeof(region.rectangle.y));
            ostream.write(reinterpret_cast<const char*>(&region.rectangle.width), sizeof(region.rectangle.width));
            ostream.write(reinterpret_cast<const char*>(&region.rectangle.height), sizeof(region.rectangle.height));
            
            //source size
            ostream.write(reinterpret_cast<const char*>(&region.source_size.x), sizeof(region.source_size.x));
            ostream.write(reinterpret_cast<const char*>(&region.source_size.y), sizeof(region.source_size.y));

            //uv
            ostream.write(reinterpret_cast<const char*>(&region.uv.min.x), sizeof(region.uv.min.x));
            ostream.write(reinterpret_cast<const char*>(&region.uv.min.y), sizeof(region.uv.min.y));
            ostream.write(reinterpret_cast<const char*>(&region.uv.max.x), sizeof(region.uv.max.x));
            ostream.write(reinterpret_cast<const char*>(&region.uv.max.y), sizeof(region.uv.max.y));

            //flags
            typedef glm::u8 flags_type;

            enum : flags_type
            {
                flag_none = 0,
                flag_rotated = (1 << 0),
                flag_trimmed = (1 << 1)
            };

            flags_type flags = flag_none;

            if (region.is_rotated)
            {
                flags |= flag_rotated;
            }

            if (region.is_trimmed)
            {
                flags |= flag_trimmed;
            }

            ostream.write(reinterpret_cast<const char*>(&flags), sizeof(flags));
        }

        return ostream;
    }
}
