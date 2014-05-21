#pragma once

//mandala
#include "../state.h"
#include "../quake_camera.h"
#include "../gui_state.h"
#include "../gui/label.h"
#include "skybox.h"

namespace mandala
{
	namespace armada
	{
		struct rpg_state_t : gui_state_t
		{
			struct render_info_t
			{
				size_t leaf_index = 0;
			};

			quake_camera_t camera;
			std::shared_ptr<gui::label_t> debug_label;
			std::shared_ptr<gui::image_t> crosshair_image;
			skybox_t skybox;
			render_info_t render_info;

			rpg_state_t();
			virtual ~rpg_state_t();

			virtual void tick(float32_t dt) override;
			virtual void render() override;
			virtual void on_input_event(input_event_t& input_event);
			virtual void on_stop_input() override;
			virtual void on_start_input() override;

		private:
			rpg_state_t(const rpg_state_t&) = delete;
			rpg_state_t& operator=(const rpg_state_t&) = delete;
		};
	};
};