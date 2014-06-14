//std
#include <memory>
#include <array>

//glm
#include <glm\ext.hpp>

//mandala
#include "../state_mgr.hpp"
#include "../app.hpp"
#include "../platform.hpp"
#include "../model.hpp"
#include "../model_instance.hpp"
#include "../animation.hpp"
#include "../color_types.hpp"
#include "../texture.hpp"
#include "../material.hpp"
#include "../gpu_program.hpp"
#include "../http_request.hpp"

//armada
#include "world_state.hpp"
#include "pause_state.hpp"
#include "world_hud_state.hpp"

namespace mandala
{
	namespace armada
	{
		world_state_t::world_state_t()
        {
            auto model = app.resources.get<model_t>(hash_t("boblampclean.md5m"));
            auto model_instance = std::make_shared<model_instance_t>(model);
            model_instance->animation = app.resources.get<animation_t>(hash_t("boblampclean.md5a"));
            model_instances.push_back(model_instance);

			hud_state = std::make_shared<world_hud_state_t>();

            camera.speed_max = 512;

            auto window_size = platform.get_window_size();

            frame_buffer = std::make_shared<frame_buffer_t>(window_size.x, window_size.y);

            blur_h_gpu_program = app.resources.get<gpu_program_t>(hash_t("blur_h.gpu"));
            blur_v_gpu_program = app.resources.get<gpu_program_t>(hash_t("blur_v.gpu"));

            std::array<vertex_t, 4> vertices = {
                vertex_t(vec3_t(0, 0, 0), vec2_t(0, 0)),
                vertex_t(vec3_t(window_size.x, 0, 0), vec2_t(1, 0)),
                vertex_t(vec3_t(window_size.x, window_size.y, 0), vec2_t(1, 1)),
                vertex_t(vec3_t(0, window_size.y, 0), vec2_t(0, 1))
            };

            std::array<uint16_t, 6> indices = {
                0, 1, 2, 0, 2, 3
            };

            glGenBuffers(1, &vertex_buffer); glCheckError();
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); glCheckError();
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * vertices.size(), vertices.data(), GL_STATIC_DRAW); glCheckError();
            glBindBuffer(GL_ARRAY_BUFFER, 0); glCheckError();

            glGenBuffers(1, &index_buffer); glCheckError();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer); glCheckError();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indices.size(), indices.data(), GL_STATIC_DRAW); glCheckError();
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); glCheckError();
		}

		void world_state_t::tick(float32_t dt)
        {
            camera.tick(dt);

            for (auto& model_instance : model_instances)
            {
                model_instance->tick(dt);
            }

			state_t::tick(dt);
		}

        void world_state_t::render()
        {
            //TODO: this whole thing is way too fuckin long. devise a way for FBO render chaining to be defined in data.
            frame_buffer->bind();

            glClearColor(0, 0, 0, 1); glCheckError();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glCheckError();

            skybox.render(camera);

            vec3_t light_position(0, 0, 100);

            for (auto& model_instance : model_instances)
            {
                model_instance->render(camera, light_position);
            }

            state_t::render();

            frame_buffer->unbind();

            frame_buffer->bind();
            {
                auto window_size = platform.get_window_size();
                auto view_projection_matrix = glm::ortho(0.0f, static_cast<float32_t>(window_size.x), 0.0f, static_cast<float32_t>(window_size.y));

                glUseProgram(blur_h_gpu_program->id); glCheckError();

                mat4_t world_matrix(1.0f);

                static auto t = 0.0f;

                static const auto world_matrix_location = glGetUniformLocation(blur_h_gpu_program->id, "world_matrix");
                static const auto view_projection_matrix_location = glGetUniformLocation(blur_h_gpu_program->id, "view_projection_matrix");
                static const auto color_texture_location = glGetUniformLocation(blur_h_gpu_program->id, "color_texture");
                static const auto color_texture_size_location = glGetUniformLocation(blur_h_gpu_program->id, "color_texture_size");
                static const auto position_location = glGetAttribLocation(blur_h_gpu_program->id, "position");
                static const auto texcoord_location = glGetAttribLocation(blur_h_gpu_program->id, "texcoord");
                static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_t, position));
                static const auto texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_t, texcoord));
                static const auto vertex_size = sizeof(vertex_t);

                t += 0.1f;

                //bind buffers
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); glCheckError();
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer); glCheckError();

                //world matrix
                glUniformMatrix4fv(world_matrix_location, 1, false, glm::value_ptr(world_matrix)); glCheckError();

                //view projection matrix
                glUniformMatrix4fv(view_projection_matrix_location, 1, false, glm::value_ptr(view_projection_matrix)); glCheckError();

                //color texture size
                if (color_texture_size_location >= 0)
                {
                    glUniform2fv(color_texture_size_location, 1, glm::value_ptr(vec2_t(frame_buffer->color_texture->width, frame_buffer->color_texture->height)));
                }

                //position
                if (position_location != -1)
                {
                    glEnableVertexAttribArray(position_location); glCheckError();
                    glVertexAttribPointer(position_location, 3, GL_FLOAT, false, vertex_size, position_offset); glCheckError();
                }

                //texcoord
                if (texcoord_location != -1)
                {
                    glEnableVertexAttribArray(texcoord_location); glCheckError();
                    glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, false, vertex_size, texcoord_offset); glCheckError();
                }

                //color texture
                if (color_texture_location != -1)
                {
                    glActiveTexture(GL_TEXTURE0); glCheckError();
                    glBindTexture(GL_TEXTURE_2D, frame_buffer->color_texture->id); glCheckError();
                    glUniform1i(color_texture_location, 0); glCheckError();
                }

                glDisable(GL_CULL_FACE); glCheckError();

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); glCheckError();

                glDisableVertexAttribArray(position_location); glCheckError();
                glDisableVertexAttribArray(texcoord_location); glCheckError();

                glUseProgram(0); glCheckError();
            }

            frame_buffer->unbind();
            {
                auto window_size = platform.get_window_size();
                auto view_projection_matrix = glm::ortho(0.0f, static_cast<float32_t>(window_size.x), 0.0f, static_cast<float32_t>(window_size.y));

                glUseProgram(blur_v_gpu_program->id); glCheckError();

                mat4_t world_matrix(1.0f);

                static const auto world_matrix_location = glGetUniformLocation(blur_v_gpu_program->id, "world_matrix");
                static const auto view_projection_matrix_location = glGetUniformLocation(blur_v_gpu_program->id, "view_projection_matrix");
                static const auto color_texture_location = glGetUniformLocation(blur_v_gpu_program->id, "color_texture");
                static const auto color_texture_size_location = glGetUniformLocation(blur_v_gpu_program->id, "color_texture_size");
                static const auto position_location = glGetAttribLocation(blur_v_gpu_program->id, "position");
                static const auto texcoord_location = glGetAttribLocation(blur_v_gpu_program->id, "texcoord");
                static const auto position_offset = reinterpret_cast<void*>(offsetof(vertex_t, position));
                static const auto texcoord_offset = reinterpret_cast<void*>(offsetof(vertex_t, texcoord));
                static const auto vertex_size = sizeof(vertex_t);

                //bind buffers
                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer); glCheckError();
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer); glCheckError();

                //world matrix
                glUniformMatrix4fv(world_matrix_location, 1, false, glm::value_ptr(world_matrix)); glCheckError();

                //view projection matrix
                glUniformMatrix4fv(view_projection_matrix_location, 1, false, glm::value_ptr(view_projection_matrix)); glCheckError();

                //color texture size
                if (color_texture_size_location >= 0)
                {
                    glUniform2fv(color_texture_size_location, 1, glm::value_ptr(vec2_t(frame_buffer->color_texture->width, frame_buffer->color_texture->height)));
                }

                //position
                if (position_location != -1)
                {
                    glEnableVertexAttribArray(position_location); glCheckError();
                    glVertexAttribPointer(position_location, 3, GL_FLOAT, false, vertex_size, position_offset); glCheckError();
                }

                //texcoord
                if (texcoord_location != -1)
                {
                    glEnableVertexAttribArray(texcoord_location); glCheckError();
                    glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, false, vertex_size, texcoord_offset); glCheckError();
                }

                //color texture
                if (color_texture_location != -1)
                {
                    glActiveTexture(GL_TEXTURE0); glCheckError();
                    glBindTexture(GL_TEXTURE_2D, frame_buffer->color_texture->id); glCheckError();
                    glUniform1i(color_texture_location, 0); glCheckError();
                }

                glDisable(GL_CULL_FACE); glCheckError();

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0); glCheckError();

                glDisableVertexAttribArray(position_location); glCheckError();
                glDisableVertexAttribArray(texcoord_location); glCheckError();

                glUseProgram(0); glCheckError();
            }
		}

		void world_state_t::on_enter()
		{
			app.states.push(hud_state);
		}

		void world_state_t::on_active()
		{
		}

		void world_state_t::on_passive()
		{
		}

		void world_state_t::on_exit()
		{
			app.states.pop(hud_state);
		}
		
		void world_state_t::on_stop_input()
		{
			platform.is_cursor_centered = false;
			platform.set_cursor_hidden(false);
		}

		void world_state_t::on_start_input()
		{
			platform.is_cursor_centered = true;
			platform.set_cursor_hidden(true);
		}

		void world_state_t::on_input_event(input_event_t& input_event)
		{
			//TODO: devise some way for states to register
			//input event handlers and iterate through them.

			if (input_event.device_type == input_event_t::device_type_t::keyboard &&
				input_event.keyboard.key == input_event_t::keyboard_t::key_t::escape &&
				input_event.keyboard.type == input_event_t::keyboard_t::type_t::key_press)
			{
				app.states.push(std::make_shared<pause_state_t>());

				input_event.is_consumed = true;
			}

            if (!input_event.is_consumed)
            {
                camera.on_input_event(input_event);
            }
		}
	};
};
