#pragma once

//std
#include <memory>

//mandala
#include "../mandala.h"
#include "../state.h"
#include "../model_instance.h"
#include "../gui/node.h"
#include "../quake_camera.h"

//armada
#include "world_hud_state.h"
#include "skybox.h"

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
		};
	};
};
