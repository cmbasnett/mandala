//mandala
#include "gui_canvas.hpp"
#include "gpu.hpp"
#include "gpu_program_mgr.hpp"
#include "gui_gpu_program.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
    gui_canvas_t::gui_canvas_t()
    {
        std::array<index_buffer_type::index_type, 4> indices =
        {
            0, 1, 2, 3
        };

        index_buffer = std::make_shared<index_buffer_type>();
        index_buffer->data(indices, gpu_t::buffer_usage_e::static_draw);

        vertex_buffer = std::make_shared<vertex_buffer_type>();
    }

    void gui_canvas_t::render_override(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
		world_matrix *= glm::translate(bounds().min.x, bounds().min.y, 0.0f);

		gpu.frame_buffers.push(frame_buffer);

        gpu.viewports.push(gpu_viewport_type(bounds()));

        gui_node_t::render(world_matrix, view_projection_matrix);

        gpu.viewports.pop();

        gpu.frame_buffers.pop();

        const auto gpu_program = gpu_programs.get<gui_gpu_program_t>();

        gpu.programs.push(gpu_program);

        gpu_program->world_matrix(world_matrix);
        gpu_program->view_projection_matrix(view_projection_matrix);

        gpu.buffers.push(gpu_t::buffer_target_e::array, vertex_buffer);
        gpu.buffers.push(gpu_t::buffer_target_e::element_array, index_buffer);

        gpu.textures.bind(0, frame_buffer->color_texture);

        gpu.draw_elements(gpu_t::primitive_type_e::quads, 4, gpu_t::index_type_e::unsigned_byte, 0);

        gpu.textures.unbind(0);

        gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
        gpu.buffers.pop(gpu_t::buffer_target_e::array);

        gpu.programs.pop();
    }

    void gui_canvas_t::clean()
    {
		gui_node_t::clean();

        static const auto vertex_count = 4;

        frame_buffer = std::make_shared<frame_buffer_t>(gpu_frame_buffer_type_e::color_depth_stencil, static_cast<gpu_frame_buffer_size_type>(size()));

        vertex_type vertices[vertex_count] =
        {
            vertex_type(vertex_type::position_type(0, 0), vertex_type::texcoord_type(0, 0)),
            vertex_type(vertex_type::position_type(size().x, 0), vertex_type::texcoord_type(1, 0)),
            vertex_type(vertex_type::position_type(size().x, size().y), vertex_type::texcoord_type(1, 1)),
            vertex_type(vertex_type::position_type(0, size().y), vertex_type::texcoord_type(0, 1))
        };
        vertex_buffer->data(vertices, vertex_count, gpu_t::buffer_usage_e::static_draw);
    }
}
