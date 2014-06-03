#pragma once

//std
#include <memory>

//mandala
#include "../mandala.hpp"
#include "../state.hpp"
#include "../model_instance.hpp"
#include "../gui/node.hpp"
#include "../quake_camera.hpp"
#include "../render_buffer.hpp"
#include "../frame_buffer.hpp"

//armada
#include "world_hud_state.hpp"
#include "skybox.hpp"

namespace mandala
{
	namespace armada
	{
		struct world_state_t : state_t
		{
			world_state_t();

			virtual void tick(float32_t dt) override;
			virtual void render() override;
			virtual void on_input_event(input_event_t& input_event) override;
			virtual void on_enter() override;
			virtual void on_exit() override;
			virtual void on_active() override;
			virtual void on_passive() override;
			virtual void on_stop_input() override;
			virtual void on_start_input() override;
			
			std::shared_ptr<gui::node_t> root_node;
			std::shared_ptr<world_hud_state_t> hud_state;
            quake_camera_t camera;
            skybox_t skybox;
            std::vector<std::shared_ptr<model_instance_t>> model_instances;
            std::shared_ptr<frame_buffer_t> frame_buffer;
            std::shared_ptr<render_buffer_t> render_buffer;
            std::shared_ptr<texture_t> frame_buffer_color0_texture;
		};
	};
};
