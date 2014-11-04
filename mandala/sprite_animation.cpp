#include "sprite_animation.hpp"

namespace mandala
{
	sprite_animation_t::sprite_animation_t(std::istream& istream)
	{
	}

	const sprite_ref_t& sprite_animation_t::get_sprite_at_time(time_type time) const
	{
        if (time.count() <= 0)
        {
            return frames.begin()->sprite;
        }
        else if (time >= _duration)
        {
            return frames.rbegin()->sprite;
		}
        else
        {
            for (const auto& frame : frames)
            {
                if (time < frame.duration)
                {
                    return frame.sprite;
                }

                time -= frame.duration;
            }
        }

        return frames.begin()->sprite;
	}
}
