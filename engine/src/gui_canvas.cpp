//std
#include <chrono>
#include <fstream>

//boost
#include <boost\make_shared.hpp>

//glm
#include <glm\ext.hpp>

//mandala
#include "gui_canvas.hpp"
#include "gpu.hpp"
#include "gpu_program_mgr.hpp"
#include "gpu_buffer_mgr.hpp"
#include "gui_image_gpu_program.hpp"
#include "blur_horizontal_gpu_program.hpp"
#include "texture.hpp"
#include "image.hpp"

namespace mandala
{
    gui_canvas_t::gui_canvas_t() :
        index_buffer(gpu_buffers.make<index_buffer_type>()),
        vertex_buffer(gpu_buffers.make<vertex_buffer_type>())
    {
        static const std::initializer_list<index_type> INDICES = { 0, 1, 2, 3 };

        index_buffer->data(INDICES, gpu_t::buffer_usage_e::STATIC_DRAW);

        auto vertices =
        {
            vertex_type(vec3_t(0, 0, 0), vec2_t(0, 0)),
            vertex_type(vec3_t(1, 0, 0), vec2_t(1, 0)),
            vertex_type(vec3_t(1, 1, 0), vec2_t(1, 1)),
            vertex_type(vec3_t(0, 1, 0), vec2_t(0, 1))
        };
        vertex_buffer->data(vertices, gpu_t::buffer_usage_e::STATIC_DRAW);
    }

    void gui_canvas_t::on_render_begin(mat4_t& world_matrix, mat4_t& view_projection_matrix)
    {
        gpu_viewport_type viewport;
        viewport.width = static_cast<gpu_viewport_type::scalar_type>(get_size().x);
        viewport.height = static_cast<gpu_viewport_type::scalar_type>(get_size().y);
        viewport.x = static_cast<gpu_viewport_type::scalar_type>(get_bounds().min.x);
        viewport.y = static_cast<gpu_viewport_type::scalar_type>(get_bounds().min.y);

        gpu.frame_buffers.push(frame_buffer);
        //gpu.viewports.push(viewport);
    }

    void gui_canvas_t::on_render_end(mat4_t& world_matrix, mat4_t& view_projection_matrix)
    {
        static const size_t DIFFUSE_TEXTURE_INDEX = 0;

        //gpu.viewports.pop();
        gpu.frame_buffers.pop();

        //TODO: for each render pass, push/pop frame buffer, do gpu program etc.

        gpu.buffers.push(gpu_t::buffer_target_e::ARRAY, vertex_buffer);
        gpu.buffers.push(gpu_t::buffer_target_e::ELEMENT_ARRAY, index_buffer);

        const auto gpu_program = gpu_programs.get<blur_horizontal_gpu_program_t>();

        gpu.programs.push(gpu_program);

        static auto t = 0.0f;

        t += 0.001f;

        auto gpu_world_matrix = world_matrix;
        gpu_world_matrix *= glm::translate(get_bounds().min.x, get_bounds().min.y, 0.0f);
        gpu_world_matrix *= glm::scale(get_size().x, get_size().y, 1.0f);   //TODO: verify correctness

        gpu.set_uniform("world_matrix", gpu_world_matrix);
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);
        gpu.set_uniform("diffuse_texture", DIFFUSE_TEXTURE_INDEX);
        gpu.set_uniform("t", t);

        gpu.textures.bind(DIFFUSE_TEXTURE_INDEX, frame_buffer->get_color_texture());

        gpu.draw_elements(gpu_t::primitive_type_e::TRIANGLE_FAN, 4, index_buffer_type::DATA_TYPE, 0);

        gpu.textures.unbind(DIFFUSE_TEXTURE_INDEX);

        gpu.buffers.pop(gpu_t::buffer_target_e::ELEMENT_ARRAY);
        gpu.buffers.pop(gpu_t::buffer_target_e::ARRAY);

        gpu.programs.pop();
    }

    void gui_canvas_t::on_clean_end()
    {
        const auto frame_buffer_size = static_cast<gpu_frame_buffer_size_type>(get_size());

        if (!frame_buffer)
        {
            frame_buffer = boost::make_shared<frame_buffer_t>(gpu_frame_buffer_type_e::COLOR_DEPTH_STENCIL, frame_buffer_size);
        }
        else if(frame_buffer_size != frame_buffer->get_size())
        {
            frame_buffer->set_size(frame_buffer_size);
        }
    }
}
