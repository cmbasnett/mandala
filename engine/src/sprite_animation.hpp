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
        typedef float32_t time_type;

        struct frame_t
        {
            sprite_ref_t sprite;
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

        sprite_animation_t(std::istream& istream);
        
        inline const time_type& get_duration() const { return duration; }

        const sprite_ref_t& get_sprite_at_time(const time_type& time) const;

    private:
        std::vector<const frame_t> frames;
        time_type duration;

        sprite_animation_t(const sprite_animation_t&) = delete;
        sprite_animation_t& operator=(const sprite_animation_t&) = delete;
    };
}