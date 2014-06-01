#pragma once

//mandala
#include "../state.hpp"
#include "../quake_camera.hpp"
#include "../gui_state.hpp"
#include "skybox.hpp"

namespace mandala
{
	namespace gui
	{
		struct label_t;
		struct image_t;
	};

	struct bsp_t;

	namespace armada
	{
		struct pause_state_t;

		struct bsp_state_t : gui_state_t
		{
			struct render_info_t
			{
				size_t leaf_index = 0;
			};

			std::shared_ptr<bsp_t> bsp;
			quake_camera_t camera;
			std::shared_ptr<gui::label_t> debug_label;
			std::shared_ptr<gui::image_t> crosshair_image;
			skybox_t skybox;
			render_info_t render_info;
			std::vector<sprite_ref_t> crosshair_sprite_refs;
			int32_t crosshair_sprite_index = 0;
			std::shared_ptr<pause_state_t> pause_state;

			bsp_state_t();
			virtual ~bsp_state_t();

			virtual void tick(float32_t dt) override;
			virtual void render() override;
			virtual void on_input_event(input_event_t& input_event);
			virtual void on_stop_input() override;
			virtual void on_start_input() override;

		private:
			bsp_state_t(const bsp_state_t&) = delete;
			bsp_state_t& operator=(const bsp_state_t&) = delete;
		};
	};
};