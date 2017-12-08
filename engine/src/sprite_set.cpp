//std
#include <fstream>

//naga
#include "io.hpp"
#include "resource_manager.hpp"
#include "sprite_set.hpp"
#include "texture.hpp"

#define TPSB_MAGIC_LENGTH   (4)
#define TPSB_MAGIC          (std::array<char, 4> { { 'T', 'P', 'S', 'B' } })
#define TPSB_VERSION        (1)

namespace naga
{
	SpriteSet::SpriteSet(std::istream& istream)
    {
        //magic
        std::array<char, TPSB_MAGIC_LENGTH> magic;
        read(istream, magic);

        if (magic != TPSB_MAGIC)
        {
            throw std::exception();
        }

        //version
        i32 version = 0;
        read(istream, version);

        if (version != TPSB_VERSION)
        {
            throw std::exception();
        }

        //texture
        std::string texture_name;
        std::getline(istream, texture_name, '\0');

        texture = resources.get<Texture>(texture_name);

        //region count
        u16 region_count = 0;

        read(istream, region_count);

        for (u16 i = 0; i < region_count; ++i)
        {
			Region region;

            //name
            std::getline(istream, region.name, '\0');

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

            //frame uv
            read(istream, region.frame_uv.min.x);
            read(istream, region.frame_uv.min.y);
            read(istream, region.frame_uv.max.x);
            read(istream, region.frame_uv.max.y);

            //uv
            read(istream, region.uv.min.x);
            read(istream, region.uv.min.y);
            read(istream, region.uv.max.x);
            read(istream, region.uv.max.y);

            //flags
            typedef u8 FlagsType;

			enum : FlagsType
            {
                FLAG_NONE = 0,
                FLAG_ROTATED = (1 << 0),
                FLAG_TRIMMED = (1 << 1)
            };

			FlagsType flags = FLAG_NONE;

            read(istream, flags);

            region.is_rotated = ((flags & FLAG_ROTATED) == FLAG_ROTATED);
            region.is_trimmed = ((flags & FLAG_TRIMMED) == FLAG_TRIMMED);

            regions.insert(std::make_pair(region.name, region));
        }
    }

	SpriteSet::SpriteSet(const std::string& texture_name) :
		SpriteSet(resources.get<Texture>(texture_name))
	{
	}

	SpriteSet::SpriteSet(const boost::shared_ptr<Texture>& texture) :
		texture(texture)
	{
		if (texture == nullptr)
		{
			throw std::invalid_argument("no texture");
		}

		Region region;
		region.frame_rectangle.x = 0;
		region.frame_rectangle.y = 0;
		region.frame_rectangle.width = texture->get_size().x;
		region.frame_rectangle.height = texture->get_size().y;
		region.name = "A";
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

		regions.emplace(region.name, region);
	}

	bool SpriteSet::has_region(const std::string& region_name) const
	{
		return regions.find(region_name) != regions.end();
	}

	const SpriteSet::Region& SpriteSet::get_region(const std::string& region_name) const
    {
		auto regions_itr = regions.find(region_name);

        if (regions_itr != regions.end())
        {
			return regions_itr->second;
        }

		throw std::invalid_argument("region does not exist");
    }
}
