//naga
#include "scene.hpp"
#include "camera_component.hpp"
#include "game_object.hpp"
#include "gpu.hpp"
#include "frame_buffer.hpp"
#include "camera_params.hpp"
#include "physics_simulation.hpp"
#include "resource_manager.hpp"
#include "bsp.hpp"
#include "terrain_component.hpp"
#include "image.hpp"

namespace naga
{
	Scene::Scene()
    {
        //bsp = resources.get<naga::bsp>(hash("dod_flash.bsp"));
		physics = boost::make_shared<PhysicsSimulation>();
    }

	void Scene::render(const boost::shared_ptr<FrameBuffer>& frame_buffer, const boost::shared_ptr<GameObject>& camera) const
    {
        GpuViewportType viewport;
        viewport.width = frame_buffer->get_size().x;
        viewport.height = frame_buffer->get_size().y;

        gpu.viewports.push(viewport);
        gpu.frame_buffers.push(frame_buffer);

        auto depth_state = gpu.depth.get_state();
        depth_state.should_test = true;
        depth_state.should_write_mask = true;
        gpu.depth.push_state(depth_state);

        gpu.clear(Gpu::CLEAR_FLAG_COLOR | Gpu::CLEAR_FLAG_DEPTH | Gpu::CLEAR_FLAG_STENCIL);

		if (camera)
		{
			auto camera_comp = camera->get_component<CameraComponent>();

			if (camera_comp)
			{
				auto camera_parameters = camera_comp->get_parameters(viewport);

				// TODO: this is inefficient, have components
				// register themselves with the scene
				// and only iterate over components that can
				// be drawn
				for (auto& game_object : game_objects)
				{
					game_object->render(camera_parameters);
				}
			}
		}

        gpu.depth.pop_state();

        gpu.frame_buffers.pop();
        gpu.viewports.pop();
    }

	void Scene::tick(f32 dt)
    {
        physics->step(dt);

        for (auto& game_object : game_objects)
        {
            game_object->on_tick(dt);
        }
    }

	void Scene::on_input_event(InputEvent& input_event)
    {
        for (auto& game_object : game_objects)
        {
            game_object->on_input_event(input_event);
        }
    }

	boost::shared_ptr<GameObject> Scene::create_game_object()
    {
		auto game_object = boost::make_shared<GameObject>();
        game_object->scene = shared_from_this();

        game_objects.emplace_back(game_object);

        return game_object;
    }


	void Scene::remove_game_object(const boost::shared_ptr<GameObject>& game_object)
    {
        if (game_object->get_scene() != shared_from_this())
        {
            throw std::exception("");
        }
	}

	TraceResult Scene::trace(const vec3& start, const vec3& end) const
	{
		auto trace_result = physics->trace(start, end);

		return trace_result;
	}
}