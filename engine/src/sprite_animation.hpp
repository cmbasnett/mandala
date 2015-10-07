#pragma once

//std
#include <vector>
#include <chrono>

//mandala
#include "resource.hpp"
#include "sprite.hpp"

namespace mandala
{
    struct sprite_animation : resource
    {
        typedef f32 time_type;

        struct frame
        {
            sprite_ref sprite;
            time_type time;

            //TODO: move to custom std::less somehow
            bool operator>(const time_type& rhs) const
            {
                return time > rhs;
            }

            bool operator<(const time_type& rhs) const
            {
                return time < rhs;
            }
        };

        sprite_animation(std::istream& istream);
        
        inline const time_type& get_duration() const { return duration; }

        const sprite_ref& get_sprite_at_time(const time_type& time) const;

    private:
        std::vector<const frame> frames;
        time_type duration;

        sprite_animation(const sprite_animation&) = delete;
        sprite_animation& operator=(const sprite_animation&) = delete;
    };
}