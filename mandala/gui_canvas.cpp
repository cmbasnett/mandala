//std
#include <chrono>

//boost
#include <boost\make_shared.hpp>

//mandala
#include "gui_canvas.hpp"
#include "gpu.hpp"
#include "gpu_program_mgr.hpp"
#include "gpu_buffer_mgr.hpp"
#include "gui_image_gpu_program.hpp"
#include "blur_horizontal_gpu_program.hpp"
#include "app.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
    gui_canvas_t::gui_canvas_t() :
        index_buffer(gpu_buffers.make<index_buffer_type>()),
        vertex_buffer(gpu_buffers.make<vertex_buffer_type>())
    {
        std::initializer_list<index_type> indices = { 0, 1, 2, 3 };
        index_buffer->data(indices, gpu_t::buffer_usage_e::static_draw);

        auto vertices =
        {
            vertex_type(vertex_type::location_type(0, 0, 0), vertex_type::color_type(1), vertex_type::texcoord_type(0, 0)),
            vertex_type(vertex_type::location_type(1, 0, 0), vertex_type::color_type(1), vertex_type::texcoord_type(1, 0)),
            vertex_type(vertex_type::location_type(1, 1, 0), vertex_type::color_type(1), vertex_type::texcoord_type(1, 1)),
            vertex_type(vertex_type::location_type(0, 1, 0), vertex_type::color_type(1), vertex_type::texcoord_type(0, 1))
        };
        vertex_buffer->data(vertices, gpu_t::buffer_usage_e::static_draw);
    }

    void gui_canvas_t::on_render_begin(const mat4_t& world_matrix, const mat4_t& view_projection_matrix)
    {
		gpu.frame_buffers.push(frame_buffer);
        gpu.viewports.push(get_bounds());

        gui_node_t::on_render_begin(world_matrix, view_projection_matrix);
    }

    void gui_canvas_t::on_render_end(const mat4_t& world_matrix, const mat4_t& view_projection_matrix)
    {
        gpu.viewports.pop();
        gpu.frame_buffers.pop();

        //TODO: for each render pass, push/pop frame buffer, do gpu program etc.

        gpu.buffers.push(gpu_t::buffer_target_e::array, vertex_buffer);
        gpu.buffers.push(gpu_t::buffer_target_e::element_array, index_buffer);

        const auto gpu_program = gpu_programs.get<blur_horizontal_gpu_program_t>();

        gpu.programs.push(gpu_program);

        static const size_t diffuse_texture_index = 0;

        static auto t = 0.0f;

        t += 0.001f;

        auto gpu_world_matrix = world_matrix;
        gpu_world_matrix *= glm::translate(get_bounds().min.x, get_bounds().min.y, 0.0f);
        gpu_world_matrix *= glm::scale(get_size().x, get_size().y, 1.0f);   //TODO: verify correctness

        gpu.set_uniform("world_matrix", gpu_world_matrix);
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);
        gpu.set_uniform("diffuse_texture", diffuse_texture_index);
        gpu.set_uniform("t", t);

        gpu.textures.bind(diffuse_texture_index, frame_buffer->get_color_texture());

        gpu.draw_elements(gpu_t::primitive_type_e::triangle_fan, 4, index_buffer_type::data_type, 0);

        gpu.textures.unbind(diffuse_texture_index);

        gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
        gpu.buffers.pop(gpu_t::buffer_target_e::array);

        gpu.programs.pop();

        gui_node_t::on_render_end(world_matrix, view_projection_matrix);
    }

    void gui_canvas_t::on_clean_end()
    {
        const auto frame_buffer_size = static_cast<gpu_frame_buffer_size_type>(get_size());

        if (!frame_buffer)
        {
            frame_buffer = boost::make_shared<frame_buffer_t>(gpu_frame_buffer_type_e::color_depth_stencil, frame_buffer_size);
        }
        else if(frame_buffer_size != frame_buffer->get_size())
        {
            frame_buffer->set_size(frame_buffer_size);
        }
    }
}
