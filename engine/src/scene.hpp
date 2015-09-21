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

    struct scene_t
    {
        const std::vector<boost::shared_ptr<actor_t>>& get_actors() const { return actors; }

        void render(boost::shared_ptr<frame_buffer_t> frame_buffer) const;

    private:
        std::vector<boost::shared_ptr<actor_t>> actors;
        boost::shared_ptr<camera_t> camera;
        octree_t octree;
    };
}