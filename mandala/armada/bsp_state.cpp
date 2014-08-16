//std
#include <sstream>
#include <random>

//mandala
#include "../app.hpp"
#include "../platform.hpp"
#include "../bsp.hpp"
#include "../bitmap_font.hpp"
#include "../model.hpp"
#include "../model_instance.hpp"
#include "../gui_image.hpp"
#include "../gui_label.hpp"
#include "../sound.hpp"
#include "../audio_source.hpp"
#include "../frame_buffer.hpp"

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
			frame_buffer = std::make_shared<frame_buffer_t>(static_cast<frame_buffer_t::size_type>(layout->bounds().size()));

			skybox.model_instance = std::make_shared<model_instance_t>(app.resources.get<model_t>(hash_t("skybox.md5m")));
			pause_state = std::make_shared<pause_state_t>();
			console_state = std::make_shared<console_state_t>();

			camera.speed_max = 512;
			camera.far = 8192;

            bsp = app.resources.get<bsp_t>(hash_t("dod_flash.bsp"));

            debug_label = std::make_shared<gui_label_t>();
			debug_label->set_bitmap_font(app.resources.get<bitmap_font_t>(hash_t("terminal_16.fnt")));
			debug_label->set_color(vec4_t(1));
			debug_label->set_dock_mode(gui_dock_mode_e::fill);
			debug_label->set_vertical_alignment(gui_label_t::vertical_alignment_e::top);
			debug_label->set_justification(gui_label_t::justification_e::left);
			debug_label->set_padding(padding_t(16));

            crosshair_image = std::make_shared<gui_image_t>();
			crosshair_image->set_is_autosized_to_texture(true);
			crosshair_image->set_sprite(sprite_ref_t(hash_t("crosshairs.json"), hash_t("crosshair2.png")));
			crosshair_image->set_anchor_flags(gui_anchor_flag_all);

			bsp_render_image = std::make_shared<gui_image_t>();
			bsp_render_image->set_dock_mode(gui_dock_mode_e::fill);

			auto sprite_set = std::make_shared<sprite_set_t>(frame_buffer->color_texture);
			app.resources.put<sprite_set_t>(sprite_set, hash_t(""));

			sprite_t sprite;
			sprite = sprite_ref_t(sprite_set->hash, sprite_set->regions.begin()->second.hash);

			bsp_render_image->set_sprite(sprite);

			layout->adopt(bsp_render_image);
            layout->adopt(debug_label);
			layout->adopt(crosshair_image);

			layout->clean();
        }

        bsp_state_t::~bsp_state_t()
        {
        }

        void bsp_state_t::tick(float32_t dt)
        {
            camera.tick(dt);

            app.audio.doppler.factor = 0.0f;
            app.audio.listener.position = camera.position;
            app.audio.listener.velocity = camera.velocity;

            render_data.leaf_index = bsp->get_leaf_index_from_position(camera.position);

            std::wostringstream oss;
            oss << L"position: [" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << "]" << std::endl;
            oss << L"rotation: [pitch=" << camera.pitch << ", yaw=" << camera.yaw << "]" << std::endl;
            oss << L"leaf index: " << bsp->render_stats.leaf_index << std::endl;
            oss << L"leafs rendered: " << bsp->render_stats.leaf_count << std::endl;
            oss << L"faces rendered: " << bsp->render_stats.face_count << std::endl;
			debug_label->set_string(oss.str());

            layout->clean();

            gui_state_t::tick(dt);
        }

        void bsp_state_t::render()
        {
			if (app.states.is_state_ticking(shared_from_this()))
			{
				gpu.frame_buffers.push(frame_buffer);

				skybox.render(camera);

				bsp->render(camera);

				gpu.frame_buffers.pop();
			}

			gui_state_t::render();
        }

        void bsp_state_t::on_input_event(input_event_t& input_event)
		{
			if (input_event.device_type == input_event_t::device_type_e::keyboard &&
				input_event.keyboard.key == input_event_t::keyboard_t::key_e::f1 &&
				input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press)
			{
				app.states.push(console_state, state_flag_render_tick);

				input_event.is_consumed = true;
				return;
			}

			if ((input_event.device_type == input_event_t::device_type_e::keyboard &&
				input_event.keyboard.key == input_event_t::keyboard_t::key_e::escape &&
				input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press) ||
				(input_event.device_type == input_event_t::device_type_e::gamepad &&
				input_event.gamepad.type == input_event_t::gamepad_t::type_e::button_release &&
				input_event.gamepad.button_index == 0))
			{
				app.states.push(pause_state, state_flag_render);

				input_event.is_consumed = true;
				return;
			}

			camera.on_input_event(input_event);

			if (input_event.is_consumed)
			{
				return;
			}

			if (input_event.device_type == input_event_t::device_type_e::touch)
			{
				if (input_event.touch.button == input_event_t::touch_t::button_e::right)
				{
					if (input_event.touch.type == input_event_t::touch_t::type_e::button_press)
					{
						camera.fov = 10;
						camera.sensitivity = 0.01f;

						input_event.is_consumed = true;
						return;
					}
					else if (input_event.touch.type == input_event_t::touch_t::type_e::button_release)
					{
						camera.fov = 70;
						camera.sensitivity = 0.1f;

						input_event.is_consumed = true;
						return;
					}
				}
				else if (input_event.touch.button == input_event_t::touch_t::button_e::left &&
					input_event.touch.type == input_event_t::touch_t::type_e::button_press)
				{
					std::uniform_real_distribution<float32_t> pitch_target_delta_distribution(1.0f, 2.0f);
					std::uniform_real_distribution<float32_t> yaw_target_delta_distribution(-0.5f, 0.5f);

					auto pitch_target_delta = pitch_target_delta_distribution(mt19937);
					auto yaw_target_delta = yaw_target_delta_distribution(mt19937);

					camera.pitch_target += pitch_target_delta;
					camera.yaw_target += yaw_target_delta;

					auto sound = app.resources.get<sound_t>(hash_t("garand_shoot.wav"));

					auto source = app.audio.create_source();
					source->position(camera.position);
					source->max_distance(500.0f);
					source->reference_distance(250.0f);
					source->queue_sound(sound);
					source->play();

					input_event.is_consumed = true;
					return;
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
			}

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
    };
};
