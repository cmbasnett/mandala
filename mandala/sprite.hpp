#pragma once

#include "sprite_set.hpp"

namespace mandala
{
	struct texture_t;

	struct sprite_ref_t
	{
		hash_t sprite_set_hash;
		hash_t region_hash;

		sprite_ref_t();
		sprite_ref_t(const hash_t& sprite_set_hash, const hash_t& region_hash);
		sprite_ref_t(sprite_ref_t&& copy);
	};

	struct sprite_t
	{
		std::shared_ptr<sprite_set_t> sprite_set;
		sprite_set_t::region_t region;

		sprite_t();
		sprite_t(const hash_t& sprite_set_hash, const hash_t& region_hash);
		sprite_t(const sprite_ref_t& sprite_def);

		sprite_t& operator=(const sprite_ref_t& sprite_ref);
	};
};
