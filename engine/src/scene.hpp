#pragma once

//std
#include <vector>

//boost
#include <boost/shared_ptr.hpp>

//naga
#include "octtree.hpp"

namespace naga
{
    struct game_object;
    struct frame_buffer;
    struct input_event_t;

    struct scene
    {
        const std::vector<boost::shared_ptr<game_object>>& get_game_objects() const { return game_objects; }

        void tick(f32 dt);
        void render(const boost::shared_ptr<frame_buffer>& frame_buffer, const boost::shared_ptr<game_object>& camera) const;
        void on_input_event(input_event_t& input_event);

        //TODO: the same actor can be added twice (double tick etc.)
        void add_game_object(const boost::shared_ptr<game_object>& actor) { game_objects.push_back(actor); }

    private:
        std::vector<boost::shared_ptr<game_object>> game_objects;
        octree octree;
    };
}