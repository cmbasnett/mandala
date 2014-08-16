//std
#include <sstream>

//armada
#include "../app.hpp"
#include "../texture.hpp"
#include "../sprite_set.hpp"
#include "world_hud_state.hpp"

namespace mandala
{
	namespace armada
	{
		world_hud_state_t::world_hud_state_t()
		{
			crosshair_image = std::make_shared<gui_image_t>();
			crosshair_image->set_is_autosized_to_texture(true);
			crosshair_image->set_sprite(sprite_ref_t(hash_t("crosshairs.json"), hash_t("crosshair2.png")));
			crosshair_image->set_anchor_flags(gui_anchor_flag_all);

            layout->adopt(crosshair_image);

            layout->clean();
		}

		world_hud_state_t::~world_hud_state_t()
		{
		}

		void world_hud_state_t::on_input_event(const input_event_t& input_event, bool& is_consumed)
		{
			if (input_event.device_type == input_event_t::device_type_e::touch &&
				input_event.touch.type == input_event_t::touch_t::type_e::scroll)
			{
				if (input_event.touch.position_delta.y > 0)
				{
					++crosshair_index;
				}
				else
				{
					--crosshair_index;
				}
			}

			//crosshair_image->sprite = crosshair_textures[crosshair_index % crosshair_textures.size()];
		}
	};
};
