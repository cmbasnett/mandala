//std
#include <memory>

//glm
#include <glm\ext.hpp>

//mandala
#include "../state_mgr.h"
#include "../app.h"
#include "../platform.h"

//armada
#include "world_state.h"
#include "pause_state.h"
#include "world_hud_state.h"

namespace mandala
{
	namespace armada
	{
		world_state_t::world_state_t() :
            world(nullptr)
		{
			world = std::make_shared<world_t>();
			hud_state = std::make_shared<world_hud_state_t>();
		}

		void world_state_t::tick(float32_t dt)
		{
			world->tick(dt);

			state_t::tick(dt);
		}

		void world_state_t::render()
		{
			world->render();

			state_t::render();
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

			world->camera.on_input_event(input_event);
		}
	};
};
