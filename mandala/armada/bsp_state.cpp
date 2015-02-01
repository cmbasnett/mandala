//std
#include <sstream>
#include <random>

//mandala
#include "../platform.hpp"
#include "../resource_mgr.hpp"
#include "../audio_mgr.hpp"
#include "../state_mgr.hpp"
#include "../bsp.hpp"
#include "../bitmap_font.hpp"
#include "../model.hpp"
#include "../model_instance.hpp"
#include "../model_animation.hpp"
#include "../gui_image.hpp"
#include "../gui_label.hpp"
#include "../sound.hpp"
#include "../audio_source.hpp"
#include "../frame_buffer.hpp"
#include "../interpolation.hpp"
#include "../basic_gpu_program.hpp"
#include "../gpu_program_mgr.hpp"

#if defined(MANDALA_PC)
#include "../window_event.hpp"
#endif

//armada
#include "bsp_state.hpp"
#include "pause_state.hpp"
#include "console_state.hpp"

namespace mandala
{
    namespace armada
    {
        std::mt19937 mt19937;

        bsp_state_t::bsp_state_t()
        {
            bsp_frame_buffer = std::make_shared<frame_buffer_t>(gpu_frame_buffer_type_e::color_depth, static_cast<gpu_frame_buffer_size_type>(layout->get_bounds().size()));

            auto sprite_set = std::make_shared<sprite_set_t>(bsp_frame_buffer->get_color_texture());
            sprite_t sprite(sprite_set, sprite_set->get_regions().begin()->second.hash);

            skybox.model_instance = std::make_shared<model_instance_t>(resources.get<model_t>(hash_t("skybox.md5m")));
            pause_state = std::make_shared<pause_state_t>();
            console_state = std::make_shared<console_state_t>();

			camera.speed_max = 512;
			camera.far = 8192;

            bsp = resources.get<bsp_t>(hash_t("dod_flash.bsp"));

            debug_label = std::make_shared<gui_label_t>();
            debug_label->set_bitmap_font(resources.get<bitmap_font_t>(hash_t("inconsolata_12.fnt")));
            debug_label->set_color(vec4_t(1));
            debug_label->set_dock_mode(gui_dock_mode_e::fill);
            debug_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::bottom);
            debug_label->set_justification(gui_label_t::justification_e::left);
            debug_label->set_padding(padding_t(16));

            crosshair_image = std::make_shared<gui_image_t>();
			crosshair_image->set_is_autosized_to_texture(true);
            crosshair_image->set_sprite(sprite_t(hash_t("white.tpsb"), hash_t("white.png")));
			crosshair_image->set_anchor_flags(gui_anchor_flag_all);

			bsp_render_image = std::make_shared<gui_image_t>();
			bsp_render_image->set_dock_mode(gui_dock_mode_e::fill);
            bsp_render_image->set_sprite(sprite);

			layout->adopt(bsp_render_image);
			layout->adopt(crosshair_image);
            layout->adopt(debug_label);
        }

        bsp_state_t::~bsp_state_t()
        {
        }

        void bsp_state_t::tick(float32_t dt)
        {
            camera.tick(dt);

            audio.doppler.factor = 0.0f;
            audio.listener.position = camera.position;
            audio.listener.velocity = camera.velocity;

            render_data.leaf_index = bsp->get_leaf_index_from_position(camera.position);

            std::wostringstream oss;
            oss << L"position: [" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << "]" << std::endl;
            oss << L"rotation: [pitch=" << camera.pitch << ", yaw=" << camera.yaw << "]" << std::endl;
            oss << L"leaf index: " << bsp->get_render_stats().leaf_index << std::endl;
            oss << L"leafs rendered: " << bsp->get_render_stats().leaf_count << std::endl;
			oss << L"faces rendered: " << bsp->get_render_stats().face_count << std::endl;
			debug_label->set_string(oss.str());

            gui_state_t::tick(dt);
        }

        void bsp_state_t::render()
        {
			if (states.is_state_ticking(shared_from_this()))
			{
                gpu_viewport_type viewport;
                viewport.x = 0;
                viewport.y = 0;
                viewport.width = bsp_frame_buffer->get_size().x;
                viewport.height = bsp_frame_buffer->get_size().y;

                gpu.viewports.push(viewport);
                gpu.frame_buffers.push(bsp_frame_buffer);

                skybox.render(camera);
                bsp->render(camera);

				gpu.frame_buffers.pop();
				gpu.viewports.pop();
			}

			gui_state_t::render();
        }

        void bsp_state_t::on_input_event(input_event_t& input_event)
		{
			gui_state_t::on_input_event(input_event);

			if (input_event.is_consumed)
			{
				return;
			}

#if defined(MANDALA_PC)
			if (input_event.device_type == input_event_t::device_type_e::keyboard &&
				input_event.keyboard.key == input_event_t::keyboard_t::key_e::grave_accent &&
				input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press)
			{
				states.push(console_state, state_flag_render_tick);

				input_event.is_consumed = true;

				return;
			}

			if ((input_event.device_type == input_event_t::device_type_e::keyboard &&
				input_event.keyboard.key == input_event_t::keyboard_t::key_e::escape &&
				input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press) ||
				(input_event.device_type == input_event_t::device_type_e::gamepad &&
				input_event.gamepad.type == input_event_t::gamepad_t::type_e::release &&
				input_event.gamepad.button_index == 0))
			{
				states.push(pause_state, state_flag_render);

				input_event.is_consumed = true;

				return;
			}
#endif

			camera.on_input_event(input_event);

			if (input_event.is_consumed)
			{
				return;
			}

#if defined(MANDALA_PC)
			if (input_event.device_type == input_event_t::device_type_e::touch)
			{
				if (input_event.touch.button == input_event_t::touch_t::button_e::right)
				{
					if (input_event.touch.type == input_event_t::touch_t::type_e::press)
					{
                        camera.fov /= 2;
                        camera.sensitivity /= 2;

						input_event.is_consumed = true;

						return;
					}
					else if (input_event.touch.type == input_event_t::touch_t::type_e::release)
					{
                        camera.fov *= 2;;
                        camera.sensitivity *= 2;

						input_event.is_consumed = true;

						return;
					}
				}
				else if (input_event.touch.type == input_event_t::touch_t::type_e::scroll)
				{
					if (input_event.touch.position_delta.y > 0)
					{
						bsp->render_settings.lightmap_gamma += 0.1f;

						input_event.is_consumed = true;

						return;
					}
					else
					{
						bsp->render_settings.lightmap_gamma -= 0.1f;

						input_event.is_consumed = true;

						return;
					}
				}
				else if (input_event.touch.button == input_event_t::touch_t::button_e::left &&
					input_event.touch.type == input_event_t::touch_t::type_e::press)
				{
					std::uniform_real_distribution<float32_t> pitch_target_delta_distribution(1.0f, 2.0f);
					std::uniform_real_distribution<float32_t> yaw_target_delta_distribution(-0.5f, 0.5f);

					auto pitch_target_delta = pitch_target_delta_distribution(mt19937);
					auto yaw_target_delta = yaw_target_delta_distribution(mt19937);

					camera.pitch_target += pitch_target_delta;
					camera.yaw_target += yaw_target_delta;

                    const auto& sound = resources.get<sound_t>(hash_t("garand_shoot.wav"));

                    auto source = std::make_shared<audio_source_t>();
					source->set_position(camera.position);
					source->set_max_distance(500.0f);
					source->set_reference_distance(250.0f);
					source->queue_sound(sound);
					source->play();

					input_event.is_consumed = true;

					return;
				}
			}
#endif

			gui_state_t::on_input_event(input_event);
        }

        void bsp_state_t::on_stop_input()
        {
			camera.local_velocity_target = vec3_t(0);	//TODO: hide local_velocity from public access

            platform.is_cursor_centered = false;

            platform.set_cursor_hidden(false);
        }

        void bsp_state_t::on_start_input()
        {
            platform.is_cursor_centered = true;

            platform.set_cursor_hidden(true);
        }

#if defined(MANDALA_PC)
        void bsp_state_t::on_window_event(window_event_t& window_event)
        {
            gui_state_t::on_window_event(window_event);

            if (window_event.type == window_event_t::type_e::resize)
            {
                bsp_frame_buffer = std::make_shared<frame_buffer_t>(gpu_frame_buffer_type_e::color_depth, static_cast<gpu_frame_buffer_size_type>(layout->get_bounds().size()));

                auto sprite_set = std::make_shared<sprite_set_t>(bsp_frame_buffer->get_color_texture());
                sprite_t sprite(sprite_set, sprite_set->get_regions().begin()->second.hash);
                bsp_render_image->set_sprite(sprite);
            }
        }
#endif
    };
};
