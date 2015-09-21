#pragma once

//std
#include <vector>

//boost
#include <boost/shared_ptr.hpp>

//mandala
#include "octtree.hpp"

namespace mandala
{
    struct gui_scene_t : gui_node_t
    {
        const std::vector<boost::shared_ptr<actor_t>>& get_actors() const { return actors; }

        void render() const;

    private:
        std::vector<boost::shared_ptr<actor_t>> actors;
        boost::shared_ptr<camera_t> camera;
        octree_t octree;
    };
}