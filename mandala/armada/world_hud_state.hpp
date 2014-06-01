#pragma once

//mandala
#include "../gui_state.hpp"
#include "../gui/gui_image.hpp"

namespace mandala
{
	struct input_event_t;
    struct model_instance_t;

	namespace armada
	{
		struct world_hud_state_t : gui_state_t
		{
			world_hud_state_t();
			virtual ~world_hud_state_t();

			virtual void on_input_event(const input_event_t& input_event, bool& is_consumed);

			std::shared_ptr<gui::image_t> crosshair_image;
			std::vector<sprite_t> crosshair_sprites;
            size_t crosshair_index = 0;
		};
	};
};
