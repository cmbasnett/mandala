#pragma once

//std
#include <chrono>
#include <deque>

//mandala
#include "sprite.hpp"

namespace mandala
{
    struct sprite_animation_t;

    struct sprite_controller
    {
        typedef std::chrono::milliseconds time_type;

        struct operation
        {
            enum class type
            {
                ANIMATION,
                FRAME,
                WAIT
            };

            type type;
            boost::shared_ptr<sprite_animation_t> animation;
            float32_t duration;
        };

        void queue(const sprite_ref_t& sprite_ref);
        void queue(const boost::shared_ptr<sprite_animation_t>& animation);
        void wait(float32_t duration);
        void play();
        void pause();
        void clear();
        void tick(float32_t dt);

    private:
        std::deque<operation> operations;
        bool is_playing = true;
        float32_t elapsed_time;
    };
}