//std
#include <fstream>

//mandala
#include "app.hpp"
#include "sprite_set.hpp"
#include "texture.hpp"

namespace mandala
{
    namespace tpsb
    {
        static const auto magic_length = 4;
        static const auto magic = "TPSB";
        static const auto version = 1;
    }

	sprite_set_t::sprite_set_t(std::istream& istream)
    {
        //magic
        char magic[tpsb::magic_length + 1];
        memset(magic, '\0', tpsb::magic_length + 1);
        istream.read(magic, tpsb::magic_length);

        if (strcmp(tpsb::magic, magic) != 0)
        {
            throw std::exception();
        }

        //version
        int32_t version = 0;
        istream.read(reinterpret_cast<char*>(&version), sizeof(version));

        if (version != tpsb::version)
        {
            throw std::exception();
        }

        //texture
        std::string texture_name;
        std::getline(istream, texture_name, '\0');

        texture = app.resources.get<texture_t>(hash_t(texture_name));

        //region count
        uint16_t region_count = 0;

        istream.read(reinterpret_cast<char*>(&region_count), sizeof(region_count));

        for (uint16_t i = 0; i < region_count; ++i)
        {
            region_t region;

            //hash
            std::string region_name;
            std::getline(istream, region_name, '\0');
            
            region.hash = hash_t(std::move(region_name));

            //frame rectangle
            istream.read(reinterpret_cast<char*>(&region.frame_rectangle.x), sizeof(region.frame_rectangle.x));
            istream.read(reinterpret_cast<char*>(&region.frame_rectangle.y), sizeof(region.frame_rectangle.y));
            istream.read(reinterpret_cast<char*>(&region.frame_rectangle.width), sizeof(region.frame_rectangle.width));
            istream.read(reinterpret_cast<char*>(&region.frame_rectangle.width), sizeof(region.frame_rectangle.width));

            //rectangle
            istream.read(reinterpret_cast<char*>(&region.rectangle.x), sizeof(region.rectangle.x));
            istream.read(reinterpret_cast<char*>(&region.rectangle.y), sizeof(region.rectangle.y));
            istream.read(reinterpret_cast<char*>(&region.rectangle.width), sizeof(region.rectangle.width));
            istream.read(reinterpret_cast<char*>(&region.rectangle.height), sizeof(region.rectangle.height));

            //source size
            istream.read(reinterpret_cast<char*>(&region.source_size.x), sizeof(region.source_size.x));
            istream.read(reinterpret_cast<char*>(&region.source_size.y), sizeof(region.source_size.y));

            //uv
            istream.read(reinterpret_cast<char*>(&region.uv.min.x), sizeof(region.uv.min.x));
            istream.read(reinterpret_cast<char*>(&region.uv.min.y), sizeof(region.uv.min.y));
            istream.read(reinterpret_cast<char*>(&region.uv.max.x), sizeof(region.uv.max.x));
            istream.read(reinterpret_cast<char*>(&region.uv.max.y), sizeof(region.uv.max.y));

            //flags
            typedef uint8_t flags_type;

            enum : flags_type
            {
                flag_none = 0,
                flag_rotated = (1 << 0),
                flag_trimmed = (1 << 1)
            };

            flags_type flags = flag_none;

            istream.read(reinterpret_cast<char*>(&flags), sizeof(flags));

            region.is_rotated = ((flags & flag_rotated) == flag_rotated);
            region.is_trimmed = ((flags & flag_trimmed) == flag_trimmed);

            regions.insert(std::make_pair(region.hash, region));
        }
	}

	sprite_set_t::sprite_set_t(const std::shared_ptr<texture_t>& texture) :
		texture(texture)
	{
        if (texture == nullptr)
        {
            throw std::invalid_argument("");
        }

		region_t region;
		region.frame_rectangle.x = 0;
		region.frame_rectangle.y = 0;
		region.frame_rectangle.width = texture->width;
		region.frame_rectangle.height = texture->height;
		region.hash = texture->hash;
		region.is_rotated = false;
		region.is_trimmed = false;
		region.rectangle.x = 0;
		region.rectangle.y = 0;
		region.rectangle.width = texture->width;
		region.rectangle.height = texture->height;
		region.source_size.x = texture->width;
		region.source_size.y = texture->height;
		region.uv.min.x = 0.0f;
		region.uv.min.y = 0.0f;
		region.uv.max.x = 1.0f;
		region.uv.max.y = 1.0f;

		regions.emplace(hash_t(), region);
	}
}
