#pragma once

//mandala
#include "../state.hpp"
#include "../quake_camera.hpp"
#include "../gui_state.hpp"
#include "../bsp.hpp"
#include "../vertex_buffer.hpp"
#include "../gpu_vertices.hpp"

//armada
#include "skybox.hpp"

namespace mandala
{
	struct bsp_t;
    struct frame_buffer_t;
    struct gui_label_t;
    struct gui_image_t;

	namespace armada
	{
		struct pause_state_t;
		struct console_state_t;

		struct bsp_state_t : gui_state_t
		{
			struct render_data_t
			{
				size_t leaf_index = 0;
			};

			std::shared_ptr<bsp_t> bsp;
            quake_camera_t camera;
            camera_t light_camera;
			std::shared_ptr<gui_label_t> debug_label;
			std::shared_ptr<gui_image_t> crosshair_image;
			std::shared_ptr<gui_image_t> bsp_render_image;
			skybox_t skybox;
            render_data_t render_data;
            bsp_t::render_stats_t render_stats;
            frustum_t camera_frustum;
            bool should_update_camera_frustum = true;
			std::shared_ptr<pause_state_t> pause_state;
			std::shared_ptr<console_state_t> console_state;
            std::shared_ptr<frame_buffer_t> shadow_frame_buffer;
            std::shared_ptr<frame_buffer_t> frame_buffer;
			std::shared_ptr<vertex_buffer_t<basic_gpu_vertex_t>> frustum_vertex_buffer;
			std::shared_ptr<index_buffer_t<uint8_t>> frustum_index_buffer;
            std::shared_ptr<model_instance_t> model_instance;

			bsp_state_t();
			virtual ~bsp_state_t();

			virtual void tick(float32_t dt) override;
			virtual void render() override;
			virtual void on_input_event(input_event_t& input_event) override;
			virtual void on_stop_input() override;
			virtual void on_start_input() override;

		private:
			bsp_state_t(const bsp_state_t&) = delete;
            bsp_state_t& operator=(const bsp_state_t&) = delete;
		};
	};
};