//naga
#include "scene.hpp"
#include "camera_component.hpp"
#include "game_object.hpp"
#include "gpu.hpp"
#include "frame_buffer.hpp"
#include "camera_params.hpp"

namespace naga
{
    void scene::render(const boost::shared_ptr<frame_buffer>& frame_buffer, const boost::shared_ptr<game_object>& camera) const
    {
        gpu_viewport_type viewport;
        viewport.width = frame_buffer->get_size().x;
        viewport.height = frame_buffer->get_size().y;

        gpu.viewports.push(viewport);
        gpu.frame_buffers.push(frame_buffer);

        auto& depth_state = gpu.depth.get_state();
        depth_state.should_test = true;
        depth_state.should_write_mask = true;
        gpu.depth.push_state(depth_state);

        gpu.clear(gpu_t::CLEAR_FLAG_COLOR | gpu_t::CLEAR_FLAG_DEPTH | gpu_t::CLEAR_FLAG_STENCIL);

        //auto camera_component = camera->get_component<camera_component>();

        //const auto camera_params = camera_component->get_params(viewport);

        gpu.depth.pop_state();

        gpu.frame_buffers.pop();
        gpu.viewports.pop();
    }

    void scene::tick(f32 dt)
    {
        for (auto& game_object : game_objects)
        {
            game_object->on_tick(dt);
        }
    }

    void scene::on_input_event(input_event_t& input_event)
    {
        for (auto& game_object : game_objects)
        {
            game_object->on_input_event(input_event);
        }
    }
}