//mandala
#include "gui_canvas.hpp"
#include "gpu.hpp"
#include "gpu_program_mgr.hpp"
#include "gui_gpu_program.hpp"
#include "blur_horizontal_gpu_program.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
    gui_canvas_t::gui_canvas_t()
    {
        index_buffer = std::make_shared<index_buffer_type>();
        index_buffer->data({ 0, 1, 2, 3 }, gpu_t::buffer_usage_e::static_draw);

        vertex_buffer = std::make_shared<vertex_buffer_type>();
        auto vertices =
        {
            vertex_type(vertex_type::location_type(0, 0, 0), vertex_type::texcoord_type(0, 0)),
            vertex_type(vertex_type::location_type(1, 0, 0), vertex_type::texcoord_type(1, 0)),
            vertex_type(vertex_type::location_type(1, 1, 0), vertex_type::texcoord_type(1, 1)),
            vertex_type(vertex_type::location_type(0, 1, 0), vertex_type::texcoord_type(0, 1))
        };
        vertex_buffer->data(vertices, gpu_t::buffer_usage_e::static_draw);
    }

    void gui_canvas_t::on_render_begin(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
		gpu.frame_buffers.push(frame_buffer);

        gpu.viewports.push(gpu_viewport_type(get_bounds()));

        gui_node_t::on_render_begin(world_matrix, view_projection_matrix);
    }

    void gui_canvas_t::on_render_end(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
        world_matrix *= glm::translate(get_bounds().min.x, get_bounds().min.y, 0.0f);
        world_matrix *= glm::scale(get_size().x, get_size().y, 1.0f);   //TODO: verify correctness

        gpu.viewports.pop();

        gpu.frame_buffers.pop();

        gpu.buffers.push(gpu_t::buffer_target_e::array, vertex_buffer);
        gpu.buffers.push(gpu_t::buffer_target_e::element_array, index_buffer);

        const auto gpu_program = gpu_programs.get<blur_horizontal_gpu_program_t>();

        gpu.programs.push(gpu_program);

        static const size_t diffuse_texture_index = 0;

        gpu_program->world_matrix(world_matrix);
        gpu_program->view_projection_matrix(view_projection_matrix);
        gpu_program->diffuse_texture_index(diffuse_texture_index);

        gpu.textures.bind(diffuse_texture_index, frame_buffer->get_color_texture());

        gpu.draw_elements(gpu_t::primitive_type_e::triangle_fan, 4, index_buffer_type::data_type, 0);

        gpu.textures.unbind(diffuse_texture_index);

        gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
        gpu.buffers.pop(gpu_t::buffer_target_e::array);

        gpu.programs.pop();

        gui_node_t::on_render_end(world_matrix, view_projection_matrix);
    }

    void gui_canvas_t::on_cleaned()
    {
        const auto frame_buffer_size = static_cast<gpu_frame_buffer_size_type>(get_size());

        if (!frame_buffer)
        {
            frame_buffer = std::make_shared<frame_buffer_t>(gpu_frame_buffer_type_e::color_depth_stencil, frame_buffer_size);
        }
        else
        {
            frame_buffer->set_size(frame_buffer_size);
        }
    }
}
