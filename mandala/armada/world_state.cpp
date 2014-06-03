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

            render_buffer = std::make_shared<render_buffer_t>(render_buffer_t::type_t::depth, window_size.x, window_size.y);
            frame_buffer_color0_texture = std::make_shared<texture_t>(color_type_t::rgba, window_size.x, window_size.y);
            frame_buffer = std::make_shared<frame_buffer_t>();
            frame_buffer->attach(frame_buffer_t::mode_t::read_draw, frame_buffer_t::attachment_type_t::color0, frame_buffer_color0_texture);
            frame_buffer->attach(frame_buffer_t::mode_t::read_draw, frame_buffer_t::attachment_type_t::depth, render_buffer);
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

            glClearColor(1, 1, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            skybox.render(camera);

            vec3_t light_position(0, 20, 100);

            for (auto& model_instance : model_instances)
            {
                model_instance->render(camera, light_position);
            }

            state_t::render();

            frame_buffer->unbind(frame_buffer_t::mode_t::read_draw);

            auto window_size = platform.get_window_size();

            auto projection_matrix = glm::ortho(0.0f, static_cast<float32_t>(window_size.x), 0.0f, static_cast<float32_t>(window_size.y));

            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf(glm::value_ptr(projection_matrix));

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glDisable(GL_CULL_FACE);

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, frame_buffer_color0_texture->id);
            glBegin(GL_TRIANGLE_FAN);

            glColor3f(1, 1, 0);
            glTexCoord2f(0, 0);
            glVertex2f(0, 0);

            glColor3f(1, 0, 0);
            glTexCoord2f(1, 0);
            glVertex2f(static_cast<float32_t>(window_size.x), 0);

            glColor3f(1, 0, 1);
            glTexCoord2f(1, 1);
            glVertex2f(static_cast<float32_t>(window_size.x), static_cast<float32_t>(window_size.y));

            glColor3f(0, 1, 0);
            glTexCoord2f(0, 1);
            glVertex2f(0, static_cast<float32_t>(window_size.y));
            glEnd();
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
