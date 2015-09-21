//mandala
#include "scene.hpp"
#include "camera.hpp"
#include "actor.hpp"
#include "collision.hpp"
#include "gpu.hpp"

namespace mandala
{
    void scene_t::render(const boost::shared_ptr<camera_t>& camera) const
    {
        for (const auto& actor : actors)
        {
            actor->render(camera);
        }
    }
}