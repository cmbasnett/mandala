//mandala
#include "gui_canvas.hpp"
#include "gpu.hpp"
#include "app.hpp"
#include "gui_gpu_program.hpp"

//glm
#include <glm\ext.hpp>

namespace mandala
{
    gui_canvas_t::gui_canvas_t()
    {
        std::array<index_buffer_type::index_type, 4> indices = {
            0, 1, 2, 3
        };

        index_buffer = std::make_shared<index_buffer_type>();
        index_buffer->data(indices, gpu_t::buffer_usage_e::static_draw);

        vertex_buffer = std::make_shared<vertex_buffer_type>();
    }

    void gui_canvas_t::render(mat4_t world_matrix, mat4_t view_projection_matrix)
    {
		world_matrix *= glm::translate(bounds().min.x, bounds().min.y, 0.0f);

		gpu.frame_buffers.push(frame_buffer);

		gpu_t::viewport_type viewport;
		viewport.x = static_cast<int32_t>(bounds().min.x);
		viewport.y = static_cast<int32_t>(bounds().min.y);
		viewport.width = static_cast<int32_t>(bounds().width());
		viewport.height = static_cast<int32_t>(bounds().height());

        gpu.viewports.push(viewport);

        gui_node_t::render(world_matrix, view_projection_matrix);

        gpu.viewports.pop();

        gpu.frame_buffers.pop();

		gpu.programs.push(gui_gpu_program);

		gui_gpu_program->world_matrix(world_matrix);
		gui_gpu_program->view_projection_matrix(view_projection_matrix);

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

		frame_buffer = std::make_shared<frame_buffer_t>(static_cast<uint32_t>(size().x), static_cast<uint32_t>(size().y));

        vertex_buffer_type::vertex_type vertices[vertex_count] = {
            vertex_type(vec2_t(0, 0), vec2_t(0, 0)),
			vertex_type(vec2_t(size().x, 0), vec2_t(1, 0)),
			vertex_type(vec2_t(size().x, size().y), vec2_t(1, 1)),
			vertex_type(vec2_t(0, size().y), vec2_t(0, 1))
        };
        vertex_buffer->data(vertices, vertex_count, gpu_t::buffer_usage_e::static_draw);
    }
}