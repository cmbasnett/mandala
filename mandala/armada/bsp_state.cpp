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
            light_camera.position = vec3_t(30, 0, 30);
            light_camera.target = vec3_t(0, 0, 0);

            model_instance = std::make_shared<model_instance_t>(resources.get<model_t>(hash_t("boblampclean.md5m")));
            //model_instance->animation = resources.get<model_animation_t>(hash_t("boblampclean.md5a"));

            frame_buffer = std::make_shared<frame_buffer_t>(gpu_frame_buffer_type_e::color_depth, static_cast<gpu_frame_buffer_size_type>(layout->bounds().size()));

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
			crosshair_image->set_sprite(sprite_ref_t(hash_t("crosshairs.tpsb"), hash_t("crosshair2.png")));
			crosshair_image->set_anchor_flags(gui_anchor_flag_all);

			bsp_render_image = std::make_shared<gui_image_t>();
			bsp_render_image->set_dock_mode(gui_dock_mode_e::fill);

			auto sprite_set = std::make_shared<sprite_set_t>(frame_buffer->color_texture);
			resources.put<sprite_set_t>(sprite_set, hash_t("temp"));

			sprite_t sprite(sprite_ref_t(sprite_set->hash, sprite_set->regions.begin()->second.hash));
			bsp_render_image->set_sprite(sprite);

			layout->adopt(bsp_render_image);
			layout->adopt(debug_label);
			layout->adopt(crosshair_image);

			const std::array<uint8_t, 24> indices = {
				frustum_corner_index_left_top_near, frustum_corner_index_right_top_near,
				frustum_corner_index_right_top_near, frustum_corner_index_right_bottom_near,
				frustum_corner_index_right_bottom_near, frustum_corner_index_left_bottom_near,
				frustum_corner_index_left_bottom_near, frustum_corner_index_left_top_near,
				frustum_corner_index_left_top_near, frustum_corner_index_left_top_far,
				frustum_corner_index_right_top_near, frustum_corner_index_right_top_far,
				frustum_corner_index_right_bottom_near, frustum_corner_index_right_bottom_far,
				frustum_corner_index_left_bottom_near, frustum_corner_index_left_bottom_far,
				frustum_corner_index_left_top_far, frustum_corner_index_right_top_far,
				frustum_corner_index_right_top_far, frustum_corner_index_right_bottom_far,
				frustum_corner_index_right_bottom_far, frustum_corner_index_left_bottom_far,
				frustum_corner_index_left_bottom_far, frustum_corner_index_left_top_far,
				};

			frustum_index_buffer = std::make_shared<index_buffer_t<uint8_t>>();
			frustum_index_buffer->data(indices, gpu_t::buffer_usage_e::static_draw);

			frustum_vertex_buffer = std::make_shared<vertex_buffer_t<basic_gpu_vertex_t>>();

            static const auto bias_matrix = { 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f };
        }

        bsp_state_t::~bsp_state_t()
        {
        }

        void bsp_state_t::tick(float32_t dt)
        {
            camera.tick(dt);
            light_camera.tick(dt);

            audio.doppler.factor = 0.0f;
            audio.listener.position = camera.position;
            audio.listener.velocity = camera.velocity;

            render_data.leaf_index = bsp->get_leaf_index_from_position(camera.position);

            std::wostringstream oss;
            oss << L"position: [" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << "]" << std::endl;
            oss << L"rotation: [pitch=" << camera.pitch << ", yaw=" << camera.yaw << "]" << std::endl;
            oss << L"leaf index: " << bsp->render_stats.leaf_index << std::endl;
            oss << L"leafs rendered: " << bsp->render_stats.leaf_count << std::endl;
			oss << L"faces rendered: " << bsp->render_stats.face_count << std::endl;
			debug_label->set_string(oss.str());

			if (should_update_camera_frustum)
			{
				camera_frustum = camera.frustum;

				const auto& corners = camera_frustum.corners();

				std::array<basic_gpu_vertex_t, frustum_corner_count> vertices =
				{
					basic_gpu_vertex_t(corners[frustum_corner_index_left_top_near], vec4_t(1)),
					basic_gpu_vertex_t(corners[frustum_corner_index_left_top_far], vec4_t(1)),
					basic_gpu_vertex_t(corners[frustum_corner_index_left_bottom_near], vec4_t(1)),
					basic_gpu_vertex_t(corners[frustum_corner_index_left_bottom_far], vec4_t(1)),
					basic_gpu_vertex_t(corners[frustum_corner_index_right_top_near], vec4_t(1)),
					basic_gpu_vertex_t(corners[frustum_corner_index_right_top_far], vec4_t(1)),
					basic_gpu_vertex_t(corners[frustum_corner_index_right_bottom_near], vec4_t(1)),
					basic_gpu_vertex_t(corners[frustum_corner_index_right_bottom_far], vec4_t(1)),
				};

				frustum_vertex_buffer->data(vertices, gpu_t::buffer_usage_e::dynamic_draw);
			}
			else
			{
				camera.frustum = camera_frustum;
			}

            model_instance->tick(dt);

            gui_state_t::tick(dt);
        }

        void bsp_state_t::render()
        {
			if (states.is_state_ticking(shared_from_this()))
			{
                gpu_viewport_type viewport;
                viewport.x = 0;
                viewport.y = 0;
                viewport.width = frame_buffer->size.x;
                viewport.height = frame_buffer->size.y;

                gpu.viewports.push(viewport);
                gpu.frame_buffers.push(frame_buffer);

                skybox.render(camera);

                bsp->render(camera);

                model_instance->render(camera, light_camera.position);

				gpu.buffers.push(gpu_t::buffer_target_e::array, frustum_vertex_buffer);
				gpu.buffers.push(gpu_t::buffer_target_e::element_array, frustum_index_buffer);

				const auto gpu_program = gpu_programs.get<basic_gpu_program_t>();

				gpu.programs.push(gpu_program);

				gpu_program->world_matrix(mat4_t());
				gpu_program->view_projection_matrix(camera.projection_matrix * camera.view_matrix);

				gpu.draw_elements(gpu_t::primitive_type_e::lines, 24, gpu_t::index_type_e::unsigned_byte, 0);

				gpu.programs.pop();

				gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
				gpu.buffers.pop(gpu_t::buffer_target_e::array);

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

			if (input_event.device_type == input_event_t::device_type_e::keyboard &&
				input_event.keyboard.key == input_event_t::keyboard_t::key_e::f2 &&
				input_event.keyboard.type == input_event_t::keyboard_t::type_e::key_press)
			{
				should_update_camera_frustum = !should_update_camera_frustum;

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

                    auto& sound = resources.get<sound_t>(hash_t("garand_shoot.wav"));

					auto source = audio.create_source();
					source->position(camera.position);
					source->max_distance(500.0f);
					source->reference_distance(250.0f);
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
                frame_buffer = std::make_shared<frame_buffer_t>(gpu_frame_buffer_type_e::color_depth, static_cast<gpu_frame_buffer_size_type>(layout->bounds().size()));

                auto sprite_set = std::make_shared<sprite_set_t>(frame_buffer->color_texture);

                //TODO: we should be able to lose reference of bsp_render_image's sprite 
                std::stringstream ss;
                ss << rand();
                
                resources.put<sprite_set_t>(sprite_set, hash_t(ss.str()));

                sprite_t sprite(sprite_ref_t(sprite_set->hash, sprite_set->regions.begin()->second.hash));
                bsp_render_image->set_sprite(sprite);
                bsp_render_image->dirty();
            }
        }
#endif
    };
};
