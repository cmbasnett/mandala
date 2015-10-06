#pragma once

//mandala
#include "state.hpp"
#include "quake_camera.hpp"
#include "state.hpp"
#include "bsp.hpp"
#include "vertex_buffer.hpp"

namespace mandala
{
    struct bsp_t;
    struct model_instance_t;
    struct gui_image_t;
    struct gui_canvas_t;
    struct frame_buffer_t;

    namespace armada
    {
        struct pause_state_t;
        struct console_state_t;

        struct bsp_state_t : state_t
        {
            struct render_data_t
            {
                size_t leaf_index = 0;
            };

            bsp_state_t();

            virtual void tick(f32 dt) override;
            virtual void render() override;
            virtual bool on_input_event(input_event_t& input_event) override;
#if defined(MANDALA_PC)
            virtual void on_window_event(window_event_t& window_event) override;
#endif
            virtual void on_stop_input() override;
            virtual void on_start_input() override;
            virtual void on_enter() override;

        private:
            boost::shared_ptr<model_instance_t> model;
            boost::shared_ptr<gui_image_t> bsp_image;
            boost::shared_ptr<gui_canvas_t> bsp_canvas;
            boost::shared_ptr<frame_buffer_t> bsp_frame_buffer;
            quake_camera_t camera;
            render_data_t render_data;
            bsp_t::render_stats_t render_stats;

            bsp_state_t(const bsp_state_t&) = delete;
            bsp_state_t& operator=(const bsp_state_t&) = delete;
        };
    };
};