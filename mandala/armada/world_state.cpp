//std
#include <memory>

//glm
#include <glm\ext.hpp>

//mandala
#include "../state_mgr.hpp"
#include "../app.hpp"
#include "../platform.hpp"
#include "../model.hpp"
#include "../model_instance.hpp"
#include "../animation.hpp"
#include "../color_types.hpp"
#include "../texture.hpp"
#include "../material.hpp"

//armada
#include "world_state.hpp"
#include "pause_state.hpp"
#include "world_hud_state.hpp"

namespace mandala
{
	namespace armada
	{
		world_state_t::world_state_t()
        {
            auto model = app.resources.get<model_t>(hash_t("boblampclean.md5m"));
            auto model_instance = std::make_shared<model_instance_t>(model);
            model_instance->animation = app.resources.get<animation_t>(hash_t("boblampclean.md5a"));
            model_instances.push_back(model_instance);

			hud_state = std::make_shared<world_hud_state_t>();

            camera.speed_max = 512;

            auto window_size = platform.get_window_size();

            frame_buffer_color0_texture = std::make_shared<texture_t>(color_type_t::rgb, window_size.x, window_size.y);
            frame_buffer = std::make_shared<frame_buffer_t>();
            frame_buffer->attach(frame_buffer_t::mode_t::read_draw, frame_buffer_t::attachment_type_t::color0, frame_buffer_color0_texture);
		}

		void world_state_t::tick(float32_t dt)
        {
            camera.tick(dt);

            for (auto& model_instance : model_instances)
            {
                model_instance->tick(dt);
            }

			state_t::tick(dt);
		}

		void world_state_t::render()
        {
            frame_buffer->bind(frame_buffer_t::mode_t::read_draw);

            skybox.render(camera);

            auto light_position = vec3_t(0, 20, 100);

            for (auto& model_instance : model_instances)
            {
                model_instance->render(camera, light_position);
            }

            state_t::render();

            frame_buffer->unbind(frame_buffer_t::mode_t::read_draw);

            skybox.render(camera);

            for (auto& model_instance : model_instances)
            {
                model_instance->model->meshes[0]->material->diffuse.texture = frame_buffer_color0_texture;
            }

            for (auto& model_instance : model_instances)
            {
                model_instance->render(camera, light_position);
            }
		}

		void world_state_t::on_enter()
		{
			app.states.push(hud_state);
		}

		void world_state_t::on_active()
		{
		}

		void world_state_t::on_passive()
		{
		}

		void world_state_t::on_exit()
		{
			app.states.pop(hud_state);
		}
		
		void world_state_t::on_stop_input()
		{
			platform.is_cursor_centered = false;
			platform.set_cursor_hidden(false);
		}

		void world_state_t::on_start_input()
		{
			platform.is_cursor_centered = true;
			platform.set_cursor_hidden(true);
		}

		void world_state_t::on_input_event(input_event_t& input_event)
		{
			//TODO: devise some way for states to register
			//input event handlers and iterate through them.

			if (input_event.device_type == input_event_t::device_type_t::keyboard &&
				input_event.keyboard.key == input_event_t::keyboard_t::key_t::escape &&
				input_event.keyboard.type == input_event_t::keyboard_t::type_t::key_press)
			{
				app.states.push(std::make_shared<pause_state_t>());

				input_event.is_consumed = true;
			}

            if (!input_event.is_consumed)
            {
                camera.on_input_event(input_event);
            }
		}
	};
};
