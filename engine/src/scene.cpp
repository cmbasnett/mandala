//mandala
#include "scene.hpp"
#include "camera.hpp"
#include "actor.hpp"
#include "gpu.hpp"
#include "frame_buffer.hpp"
#include "camera_params.hpp"

namespace mandala
{
    void scene::render(const boost::shared_ptr<frame_buffer>& frame_buffer, const boost::shared_ptr<mandala::camera>& camera) const
    {
        gpu_viewport_type viewport;
        viewport.width = frame_buffer->get_size().x;
        viewport.height = frame_buffer->get_size().y;

        gpu.viewports.push(viewport);
        gpu.frame_buffers.push(frame_buffer);

        gpu.clear(gpu_t::CLEAR_FLAG_COLOR | gpu_t::CLEAR_FLAG_DEPTH);

        const auto camera_params = camera->get_params(viewport);

        for (const auto& actor : actors)
        {
            actor->render(camera_params);
        }

        gpu.frame_buffers.pop();
        gpu.viewports.pop();
    }

    void scene::tick(f32 dt)
    {
        for (auto& actor : actors)
        {
            actor->on_tick(dt);
        }
    }

    void scene::on_input_event(input_event_t& input_event)
    {
        for (auto& actor : actors)
        {
            actor->on_input_event(input_event);
        }
    }
}