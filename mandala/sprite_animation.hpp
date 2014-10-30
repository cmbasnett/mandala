#pragma once

//std
#include <vector>
#include <chrono>

//mandala
#include "resource.hpp"
#include "sprite.hpp"

namespace mandala
{
	struct sprite_animation_t : resource_t
	{
		typedef std::chrono::milliseconds duration_type;

		struct frame_t
		{
			sprite_ref_t sprite;
			duration_type duration;
		};

		sprite_animation_t(std::istream& istream);
		
		inline const duration_type& duration() const { return _duration; }

		const frame_t& const get_frame_at_time(const duration_type& time) const;

	private:
		std::vector<const frame_t> frames;
		duration_type _duration;

		sprite_animation_t(const sprite_animation_t&) = delete;
		sprite_animation_t& operator=(const sprite_animation_t&) = delete;
	};
}
