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
#include "terrain_component.hpp"
#include "image.hpp"

namespace naga
{
    scene::scene()
    {
        //bsp = resources.get<naga::bsp>(hash("dod_flash.bsp"));
        physics = boost::make_shared<physics_simulation>();
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

		if (camera)
		{
			auto camera_comp = camera->get_component<camera_component>();

			if (camera_comp)
			{
				auto camera_params = camera_comp->get_params(viewport);

				// TODO: this is inefficient, have components
				// register themselves with the scene
				// and only iterate over components that can
				// be drawn
				for (auto& game_object : game_objects)
				{
					game_object->render(camera_params);
				}
			}
		}

        gpu.depth.pop_state();

        gpu.frame_buffers.pop();
        gpu.viewports.pop();
    }

    void scene::tick(f32 dt)
    {
        physics->step(dt);

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

    boost::shared_ptr<game_object> scene::create_game_object()
    {
        auto game_object = boost::make_shared<naga::game_object>();
        game_object->scene = shared_from_this();

        game_objects.push_back(game_object);

        return game_object;
    }


    void scene::remove_game_object(const boost::shared_ptr<game_object>& game_object)
    {
        if (game_object->get_scene() != shared_from_this())
        {
            throw std::exception("");
        }
    }
}