#pragma once

//std
#include <vector>

//boost
#include <boost/shared_ptr.hpp>

//mandala
#include "octtree.hpp"

namespace mandala
{
    struct actor_t;
    struct camera_t;
    struct frame_buffer_t;
    struct input_event_t;

    struct scene_t
    {
        const std::vector<boost::shared_ptr<actor_t>>& get_actors() const { return actors; }

        void tick(f32 dt);
        void render(const boost::shared_ptr<frame_buffer_t>& frame_buffer, const boost::shared_ptr<camera_t>& camera) const;
        void on_input_event(input_event_t& input_event);

        //TODO: the same actor can be added twice (double tick etc.)
        void add_actor(const boost::shared_ptr<actor_t>& actor) { actors.push_back(actor); }

    private:
        std::vector<boost::shared_ptr<actor_t>> actors;
        boost::shared_ptr<camera_t> camera;
        octree_t octree;
    };
}