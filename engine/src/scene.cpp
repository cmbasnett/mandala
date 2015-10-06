//mandala
#include "scene.hpp"
#include "camera.hpp"
#include "actor.hpp"
#include "gpu.hpp"
#include "frame_buffer.hpp"
#include "camera_params.hpp"

namespace mandala
{
    void scene_t::render(const boost::shared_ptr<frame_buffer_t>& frame_buffer, const boost::shared_ptr<camera_t>& camera) const
    {
        gpu_viewport_type viewport;
        viewport.width = frame_buffer->get_size().x;
        viewport.height = frame_buffer->get_size().y;

        gpu.viewports.push(viewport);
        gpu.frame_buffers.push(frame_buffer);

        gpu.set_clear_color(rgba_type(1, 0, 0, 1));
        gpu.clear(gpu_t::CLEAR_FLAG_COLOR | gpu_t::CLEAR_FLAG_DEPTH);

        const auto camera_params = camera->get_params(viewport);

        for (const auto& actor : actors)
        {
            actor->render(camera_params);
        }

        gpu.frame_buffers.pop();
        gpu.viewports.pop();
    }

    void scene_t::tick(float32_t dt)
    {
        for (auto& actor : actors)
        {
            actor->on_tick(dt);
        }
    }

    void scene_t::on_input_event(input_event_t& input_event)
    {
        for (auto& actor : actors)
        {
            actor->on_input_event(input_event);
        }
    }
}