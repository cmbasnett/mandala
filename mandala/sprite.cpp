//mandala
#include "app.h"
#include "sprite.h"
#include "sprite_set.h"
#include "texture.h"

namespace mandala
{
	sprite_ref_t::sprite_ref_t()
	{
	}

	sprite_ref_t::sprite_ref_t(const hash_t& sprite_set_hash, const hash_t& region_hash) :
		sprite_set_hash(sprite_set_hash),
		region_hash(region_hash)
	{
	}

	sprite_ref_t::sprite_ref_t(sprite_ref_t&& copy) :
		sprite_set_hash(std::move(copy.sprite_set_hash)),
		region_hash(std::move(copy.region_hash))
	{
	}

	sprite_t::sprite_t()
	{
	}

	sprite_t::sprite_t(const hash_t& sprite_set_hash, const hash_t& region_hash)
	{
		sprite_set = app.resources.get<sprite_set_t>(sprite_set_hash);

		auto regions_itr = sprite_set->regions.find(region_hash);

		if (regions_itr == sprite_set->regions.end())
		{
			throw std::exception();
		}

		region = regions_itr->second;
	}

	sprite_t::sprite_t(const sprite_ref_t& sprite_ref) :
		sprite_t(sprite_ref.sprite_set_hash, sprite_ref.region_hash)
	{
	}

	sprite_t& sprite_t::operator=(const sprite_ref_t& sprite_ref)
	{
		sprite_set = app.resources.get<sprite_set_t>(sprite_ref.sprite_set_hash);

		auto regions_itr = sprite_set->regions.find(sprite_ref.region_hash);

		if (regions_itr == sprite_set->regions.end())
		{
			throw std::exception();
		}

		region = regions_itr->second;

		return *this;
	}
};
