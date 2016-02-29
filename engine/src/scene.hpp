#pragma once

//std
#include <vector>
#include <set>

//boost
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

//naga
#include "octtree.hpp"
#include "game_object_collection.hpp"

namespace naga
{
    struct game_object;
    struct frame_buffer;
    struct input_event_t;
    struct physics_simulation;
    struct bsp;

    struct scene : boost::enable_shared_from_this<scene>
    {
        scene();

        const std::vector<boost::shared_ptr<game_object>>& get_game_objects() const { return game_objects; }

        void tick(f32 dt);
        void render(const boost::shared_ptr<frame_buffer>& frame_buffer, const boost::shared_ptr<game_object>& camera) const;
        void on_input_event(input_event_t& input_event);

        boost::shared_ptr<game_object> create_game_object();
        void remove_game_object(const boost::shared_ptr<game_object>& game_object);

        const boost::shared_ptr<physics_simulation>& get_physics() const { return physics; }

    private:
        friend struct game_object;

        std::vector<boost::shared_ptr<game_object>> game_objects;

        boost::shared_ptr<physics_simulation> physics;
        octree octree;
        boost::shared_ptr<bsp> bsp;
    };
}