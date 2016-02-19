//naga
#include "scene.hpp"
#include "camera_component.hpp"
#include "game_object.hpp"
#include "gpu.hpp"
#include "frame_buffer.hpp"
#include "camera_params.hpp"
#include "physics_simulation.hpp"
#include "resource_mgr.hpp"
#include "bsp.hpp"
#include "terrain.hpp"
#include "image.hpp"

namespace naga
{
    scene::scene()
    {
        //bsp = resources.get<naga::bsp>(hash("dod_flash.bsp"));
        terrain = boost::make_shared<naga::terrain>(resources.get<image>(hash("mountains512.png")));
    }

    void scene::render(const boost::shared_ptr<frame_buffer>& frame_buffer, const boost::shared_ptr<game_object>& camera) const
    {
        gpu_viewport_type viewport;
        viewport.width = frame_buffer->get_size().x;
        viewport.height = frame_buffer->get_size().y;

        gpu.viewports.push(viewport);
        gpu.frame_buffers.push(frame_buffer);

        auto depth_state = gpu.depth.get_state();
        depth_state.should_test = true;
        depth_state.should_write_mask = true;
        gpu.depth.push_state(depth_state);

        gpu.clear(gpu_t::CLEAR_FLAG_COLOR | gpu_t::CLEAR_FLAG_DEPTH | gpu_t::CLEAR_FLAG_STENCIL);

        auto camera_comp = camera->get_component<camera_component>();

        if (camera_comp)
        {
            const auto camera_params = camera_comp->get_params(viewport);

            //bsp->render(camera_params);
            terrain->render(camera_params);
        }

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

    void scene::add_game_object(const boost::shared_ptr<game_object>& game_object)
    {
        //TODO: the same actor can be added twice (double tick etc.)
        game_objects.push_back(game_object);
    }
}