#pragma once

//mandala
#include "sprite_set.hpp"

namespace mandala
{
	struct texture_t;

	struct sprite_ref_t
	{
		hash_t sprite_set_hash;
		hash_t region_hash;

		sprite_ref_t() = default;
		sprite_ref_t(const hash_t& sprite_set_hash, const hash_t& region_hash);
		sprite_ref_t(sprite_ref_t&& copy);
	};

	struct sprite_t
	{
		typedef sprite_set_t::region_t region_type;
		typedef std::shared_ptr<sprite_set_t> sprite_set_type;

		sprite_set_type sprite_set;
		region_type region;

		sprite_t() = default;
		sprite_t(const hash_t& sprite_set_hash, const hash_t& region_hash);
		sprite_t(const sprite_ref_t& sprite_def);

		sprite_t& operator=(const sprite_t& rhs);
		sprite_t& operator=(const sprite_ref_t& rhs);
		bool operator==(const sprite_ref_t&) const;
		bool operator!=(const sprite_ref_t&) const;
	};
}
