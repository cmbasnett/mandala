//naga
#include "resource_manager.hpp"
#include "sprite.hpp"
#include "sprite_set.hpp"
#include "texture.hpp"

namespace naga
{
	SpriteReference::SpriteReference(const std::string& sprite_set_name, const std::string& region_name) :
		sprite_set_name(sprite_set_name),
        region_name(region_name)
    {
    }

	SpriteReference::SpriteReference(SpriteReference&& copy) :
		sprite_set_name(std::move(copy.sprite_set_name)),
        region_name(std::move(copy.region_name))
    {
    }

	Sprite::Sprite(const std::string& sprite_set_name, const std::string& region_name) :
		Sprite(resources.get<SpriteSet>(sprite_set_name), region_name)
    {
    }

	Sprite::Sprite(const SpriteReference& sprite_reference) :
		Sprite(sprite_reference.sprite_set_name, sprite_reference.region_name)
    {
    }

	Sprite::Sprite(const SpriteSetType& sprite_set, const std::string& region_name)
    {
        if (sprite_set == nullptr)
        {
            throw std::invalid_argument("sprite set cannot be null");
        }

		texture = sprite_set->get_texture();
		region = sprite_set->get_region(region_name);
    }


	Sprite::Sprite(const boost::shared_ptr<Texture>& texture) :
		texture(texture)
	{
		if (texture == nullptr)
		{
			throw std::invalid_argument("sprite set cannot be null");
		}

		auto width = static_cast<f32>(texture->get_width());
		auto height = static_cast<f32>(texture->get_height());

		region.rectangle.width = width;
		region.rectangle.height = height;
		region.frame_uv.max.x = 1;
		region.frame_uv.max.y = 1;
		region.frame_rectangle.width = width;
		region.frame_rectangle.height = height;
		region.source_size.x = width;
		region.source_size.y = height;
		region.uv.max.x = 1;
		region.uv.max.y = 1;
	}

	Sprite& Sprite::operator=(const Sprite& rhs)
	{
		texture = rhs.texture;
        region = rhs.region;

        return *this;
    }

	Sprite& Sprite::operator=(const SpriteReference& ref)
    {
		auto sprite_set = resources.get<SpriteSet>(ref.sprite_set_name);

		texture = sprite_set->get_texture();

		try
		{
			region = sprite_set->get_region(ref.region_name);
		}
		catch (std::invalid_argument&)
		{
			throw std::exception("region does not exist in sprite set");
		}

        return *this;
    }

	/*
	bool Sprite::operator==(const SpriteReference& sprite_ref) const
    {
		return region.name == sprite_ref.region_name && sprite_set->name == sprite_ref.sprite_set_name;
    }

	bool Sprite::operator!=(const SpriteReference& sprite_ref) const
    {
		return region.name != sprite_ref.region_name || sprite_set->name != sprite_ref.sprite_set_name;
    }
	*/
}
