#pragma once

//std
#include <vector>
#include <chrono>

//naga
#include "resource.hpp"
#include "sprite.hpp"

namespace naga
{
    struct SpriteAnimation : Resource
    {
        typedef f32 TimeType;

        struct Frame
        {
            SpriteReference sprite;
			TimeType time;

            //TODO: move to custom std::less somehow
			bool operator>(const TimeType& rhs) const
            {
                return time > rhs;
            }

			bool operator<(const TimeType& rhs) const
            {
                return time < rhs;
            }
        };

		SpriteAnimation(std::istream& istream);
        
		inline const TimeType& get_duration() const { return duration; }

		const SpriteReference& get_sprite_at_time(const TimeType& time) const;

    private:
        std::vector<Frame> frames;
        TimeType duration;

		SpriteAnimation(const SpriteAnimation&) = delete;
		SpriteAnimation& operator=(const SpriteAnimation&) = delete;
    };
}