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
    struct physics_simulation;
    struct bsp;
    struct terrain;

    struct scene
    {
        scene();

        const std::vector<boost::shared_ptr<game_object>>& get_game_objects() const { return game_objects; }

        void tick(f32 dt);
        void render(const boost::shared_ptr<frame_buffer>& frame_buffer, const boost::shared_ptr<game_object>& camera) const;
        void on_input_event(input_event_t& input_event);

        void add_game_object(const boost::shared_ptr<game_object>& game_object);

    private:
        std::vector<boost::shared_ptr<game_object>> game_objects;
        boost::shared_ptr<physics_simulation> physics;
        octree octree;
        boost::shared_ptr<bsp> bsp;
        boost::shared_ptr<terrain> terrain;
    };
}