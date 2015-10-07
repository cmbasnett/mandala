//std
#include <sstream>
#include <random>
#include <fstream>
#include <thread>

//mandala
#include "bsp_state.hpp"
#include "platform.hpp"
#include "resource_mgr.hpp"
#include "audio_mgr.hpp"
#include "state_mgr.hpp"
#include "bsp.hpp"
#include "bitmap_font.hpp"
#include "model.hpp"
#include "model_instance.hpp"
#include "model_animation.hpp"
#include "gui_image.hpp"
#include "gui_label.hpp"
#include "sound.hpp"
#include "audio_source.hpp"
#include "frame_buffer.hpp"
#include "interpolation.hpp"
#include "basic_gpu_program.hpp"
#include "gpu_program_mgr.hpp"
#include "gui_canvas.hpp"
#include "image.hpp"
#include "python.hpp"
#include "gui_layout.hpp"

#if defined(MANDALA_PC)
#include "window_event.hpp"
#endif

namespace mandala
{
    namespace armada
    {
        std::mt19937 mt19937;

        bsp_state_t::bsp_state_t() :
            model(boost::make_shared<model_instance>(hash_t("boblampclean.md5m")))
        {
            model->play(hash_t("boblampclean.md5a"));

            camera.speed_max = 512;
            camera.far = 8192;

            bsp_image = boost::make_shared<gui_image_t>();
            bsp_image->set_dock_mode(gui_dock_mode_e::FILL);

            bsp_canvas = boost::make_shared<gui_canvas_t>();
            bsp_canvas->set_dock_mode(gui_dock_mode_e::FILL);
            bsp_canvas->adopt(bsp_image);

            get_layout()->adopt(bsp_canvas);
        }

        void bsp_state_t::tick(f32 dt)
        {
            model->tick(dt);
            //camera.tick(dt);

            audio.doppler.factor = 0.0f;
            audio.listener.location = camera.pose.location;
            audio.listener.velocity = camera.velocity;

            //render_data.leaf_index = world.get_bsp()->get_leaf_index_from_location(camera.pose.location);

            state_t::tick(dt);
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

                gpu.clear(gpu_t::CLEAR_FLAG_COLOR | gpu_t::CLEAR_FLAG_DEPTH | gpu_t::CLEAR_FLAG_STENCIL);

                //TODO: need to create a proper level thing here! scene graph and whatever
                //bsp->render(camera);
                //model->render(camera, vec3(0));

                gpu.frame_buffers.pop();
                gpu.viewports.pop();
            }

            state_t::render();
        }

        bool bsp_state_t::on_input_event(input_event_t& input_event)
        {
#if defined(MANDALA_PC)
            if (input_event.device_type == input_event_t::device_type_e::KEYBOARD &&
                input_event.keyboard.key == input_event_t::keyboard_t::key_e::ESCAPE &&
                input_event.keyboard.type == input_event_t::keyboard_t::type_e::KEY_PRESS)
            {
                //py.exec("states.push(PopupState(), STATE_FLAG_RENDER | STATE_FLAG_TICK)");

                //auto texture = bsp_canvas->get_frame_buffer()->get_color_texture();

                //auto begin = std::chrono::system_clock::now();

                //std::vector<u8> data;
                //gpu.get_texture_data(texture, data);

                //std::ofstream ostream = std::ofstream("screenshot.png", std::ios::binary);
                //auto image = boost::make_shared<image_t>(texture->get_size(), 8, texture->get_color_type(), data.data(), data.size());
                //ostream << *image;

                //auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begin);

                //input_event.is_consumed = true;

                return true;
            }
#endif

            //TODO: this is wack
            if (camera.on_input_event(input_event))
            {
                return true;
            }

#if defined(MANDALA_PC)
            if (input_event.device_type == input_event_t::device_type_e::TOUCH)
            {
                if (input_event.touch.button == input_event_t::touch_t::button_e::RIGHT)
                {
                    if (input_event.touch.type == input_event_t::touch_t::type_e::PRESS)
                    {
                        camera.fov /= 2;
                        camera.sensitivity /= 2;

                        return true;
                    }
                    else if (input_event.touch.type == input_event_t::touch_t::type_e::RELEASE)
                    {
                        camera.fov *= 2;;
                        camera.sensitivity *= 2;

                        return true;
                    }
                }
                else if (input_event.touch.type == input_event_t::touch_t::type_e::SCROLL)
                {
                    if (input_event.touch.location_delta.y > 0)
                    {
                        //bsp->render_settings.lightmap_gamma += 0.1f;

                        return true;
                    }
                    else
                    {
                        //bsp->render_settings.lightmap_gamma -= 0.1f;

                        return true;
                    }
                }
                else if (input_event.touch.button == input_event_t::touch_t::button_e::LEFT &&
                    input_event.touch.type == input_event_t::touch_t::type_e::PRESS)
                {
                    std::uniform_real_distribution<f32> pitch_target_delta_distribution(1.0f, 2.0f);
                    std::uniform_real_distribution<f32> yaw_target_delta_distribution(-0.5f, 0.5f);

                    auto pitch_target_delta = pitch_target_delta_distribution(mt19937);
                    auto yaw_target_delta = yaw_target_delta_distribution(mt19937);

                    camera.pitch_target += pitch_target_delta;
                    camera.yaw_target += yaw_target_delta;

                    const auto& sound = resources.get<sound_t>(hash_t("garand_shoot.wav"));

                    auto source = boost::make_shared<audio_source_t>();
                    source->set_location(camera.pose.location);
                    source->set_max_distance(500.0f);
                    source->set_reference_distance(250.0f);
                    source->queue_sound(sound);
                    source->play();

                    return true;
                }
            }
#endif
            
            return false;
        }

        void bsp_state_t::on_stop_input()
        {
            camera.local_velocity_target = vec3(0);    //TODO: hide local_velocity from public access

            platform.is_cursor_centered = false;

            platform.set_cursor_hidden(false);
        }

        void bsp_state_t::on_start_input()
        {
            platform.is_cursor_centered = true;

            platform.set_cursor_hidden(true);
        }

        void bsp_state_t::on_enter()
        {
            state_t::on_enter();

            bsp_frame_buffer = boost::make_shared<frame_buffer_t>(gpu_frame_buffer_type_e::COLOR_DEPTH, static_cast<gpu_frame_buffer_size_type>(get_layout()->get_bounds().size()));
            auto sprite_set = boost::make_shared<sprite_set_t>(bsp_frame_buffer->get_color_texture());
            sprite_t sprite(sprite_set, sprite_set->get_regions().begin()->second->hash);
            bsp_image->set_sprite(sprite);
        }

#if defined(MANDALA_PC)
        void bsp_state_t::on_window_event(window_event_t& window_event)
        {
            state_t::on_window_event(window_event);

            if (window_event.type == window_event_t::type_e::RESIZE && bsp_frame_buffer)
            {
                bsp_frame_buffer->set_size(static_cast<gpu_frame_buffer_size_type>(get_layout()->get_bounds().size()));

                auto sprite_set = boost::make_shared<sprite_set_t>(bsp_frame_buffer->get_color_texture());
                sprite_t sprite(sprite_set, sprite_set->get_regions().begin()->second->hash);
                bsp_image->set_sprite(sprite);
            }
        }
#endif
    };
};
