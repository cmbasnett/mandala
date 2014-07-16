#pragma once

//std
#include <memory>

//mandala
#include "../state.hpp"
#include "../model_instance.hpp"
#include "../gui_node.hpp"
#include "../quake_camera.hpp"
#include "../frame_buffer.hpp"

//armada
#include "world_hud_state.hpp"
#include "skybox.hpp"

namespace mandala
{
    struct gpu_program_t;

	namespace armada
	{
		struct world_state_t : state_t
		{
            struct vertex_t
            {
                vertex_t(vec3_t position, vec2_t texcoord) :
                    position(position),
                    texcoord(texcoord)
                {
                }

                vec3_t position;
                vec2_t texcoord;
            };

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
			
			std::shared_ptr<gui_node_t> root_node;
			std::shared_ptr<world_hud_state_t> hud_state;
            quake_camera_t camera;
            skybox_t skybox;
            std::vector<std::shared_ptr<model_instance_t>> model_instances;
            std::shared_ptr<frame_buffer_t> frame_buffer;
            std::shared_ptr<gpu_program_t> blur_h_gpu_program;
            std::shared_ptr<gpu_program_t> blur_v_gpu_program;
            uint32_t vertex_buffer = 0;
            uint32_t index_buffer = 0;
		};
	};
};
