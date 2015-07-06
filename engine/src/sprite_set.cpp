//std
#include <fstream>

//mandala
#include "io.hpp"
#include "resource_mgr.hpp"
#include "sprite_set.hpp"
#include "texture.hpp"

#define TPSB_MAGIC_LENGTH   (4)
#define TPSB_MAGIC          (std::array<char, 4> { 'T', 'P', 'S', 'B' })
#define TPSB_VERSION        (1)

namespace mandala
{
	sprite_set_t::sprite_set_t(std::istream& istream)
    {
        //magic
        std::array<char, TPSB_MAGIC_LENGTH> magic;
        read(istream, magic);

        if (magic != TPSB_MAGIC)
        {
            throw std::exception();
        }

        //version
        int32_t version = 0;
		read(istream, version);

        if (version != TPSB_VERSION)
        {
            throw std::exception();
        }

        //texture
        std::string texture_name;
        std::getline(istream, texture_name, '\0');

        texture = resources.get<texture_t>(hash_t(texture_name));

        //region count
        uint16_t region_count = 0;

		read(istream, region_count);

        for (uint16_t i = 0; i < region_count; ++i)
        {
            region_t region;

            //hash
            std::string region_name;
            std::getline(istream, region_name, '\0');
            
            region.hash = hash_t(std::move(region_name));

			//frame rectangle
			read(istream, region.frame_rectangle.x);
			read(istream, region.frame_rectangle.y);
			read(istream, region.frame_rectangle.width);
			read(istream, region.frame_rectangle.height);

            //rectangle
			read(istream, region.rectangle.x);
			read(istream, region.rectangle.y);
			read(istream, region.rectangle.width);
			read(istream, region.rectangle.height);

            //source size
			read(istream, region.source_size.x);
			read(istream, region.source_size.y);

            //uv
			read(istream, region.uv.min.x);
			read(istream, region.uv.min.y);
			read(istream, region.uv.max.x);
			read(istream, region.uv.max.y);

            //flags
            typedef uint8_t flags_type;

            enum : flags_type
            {
                flag_none = 0,
                flag_rotated = (1 << 0),
                flag_trimmed = (1 << 1)
            };

            flags_type flags = flag_none;

			read(istream, flags);

            region.is_rotated = ((flags & flag_rotated) == flag_rotated);
            region.is_trimmed = ((flags & flag_trimmed) == flag_trimmed);

            regions.insert(std::make_pair(region.hash, region));
        }
	}

	sprite_set_t::sprite_set_t(const boost::shared_ptr<texture_t>& texture) :
		texture(texture)
	{
        if (texture == nullptr)
        {
            throw std::invalid_argument("");
        }

		region_t region;
		region.frame_rectangle.x = 0;
		region.frame_rectangle.y = 0;
        region.frame_rectangle.width = texture->get_size().x;
		region.frame_rectangle.height = texture->get_size().y;
		region.hash = texture->hash;
		region.is_rotated = false;
		region.is_trimmed = false;
		region.rectangle.x = 0;
		region.rectangle.y = 0;
		region.rectangle.width = texture->get_size().x;
		region.rectangle.height = texture->get_size().y;
        region.source_size = texture->get_size();
		region.uv.min.x = 0.0f;
		region.uv.min.y = 0.0f;
		region.uv.max.x = 1.0f;
		region.uv.max.y = 1.0f;

		regions.emplace(hash_t(), region);
	}

    boost::optional<sprite_set_t::region_t> sprite_set_t::get_region(const hash_t & region_hash) const
    {
        boost::optional<region_t> region;

        auto regions_itr = regions.find(region_hash);

        if (regions_itr != regions.end())
        {
            region = regions_itr->second;
        }

        return region;
    }
}