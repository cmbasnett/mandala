#pragma once

//std
#include <vector>

//boost
#include <boost/shared_ptr.hpp>

//mandala
#include "octtree.hpp"

namespace mandala
{
    struct actor;
    struct camera;
    struct frame_buffer;
    struct input_event_t;

    struct scene
    {
        const std::vector<boost::shared_ptr<actor>>& get_actors() const { return actors; }

        void tick(f32 dt);
        void render(const boost::shared_ptr<frame_buffer>& frame_buffer, const boost::shared_ptr<camera>& camera) const;
        void on_input_event(input_event_t& input_event);

        //TODO: the same actor can be added twice (double tick etc.)
        void add_actor(const boost::shared_ptr<actor>& actor) { actors.push_back(actor); }

    private:
        std::vector<boost::shared_ptr<actor>> actors;
        boost::shared_ptr<camera> camera;
        octree octree;
    };
}