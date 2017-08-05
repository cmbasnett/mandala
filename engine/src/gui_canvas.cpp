//std
#include <chrono>
#include <fstream>

//boost
#include <boost\make_shared.hpp>

//glm
#include <glm\ext.hpp>

//naga
#include "gui_canvas.hpp"
#include "gpu.hpp"
#include "gpu_program_mgr.hpp"
#include "gpu_buffer_mgr.hpp"
#include "gui_image_gpu_program.hpp"
#include "blur_horizontal_gpu_program.hpp"
#include "texture.hpp"
#include "image.hpp"

namespace naga
{
    boost::weak_ptr<gui_canvas::index_buffer_type> gui_canvas::index_buffer;
    boost::weak_ptr<gui_canvas::vertex_buffer_type> gui_canvas::vertex_buffer;

    gui_canvas::gui_canvas()
    {
        if (index_buffer.expired())
        {
            index_buffer = gpu_buffers.make<index_buffer_type>().lock();
            index_buffer.lock()->data({ 0, 1, 2, 3 }, gpu_t::buffer_usage::STATIC_DRAW);
        }

        if (vertex_buffer.expired())
        {
            auto vertices =
            {
                vertex_type(vec3(0, 0, 0), vec2(0, 0)),
                vertex_type(vec3(1, 0, 0), vec2(1, 0)),
                vertex_type(vec3(1, 1, 0), vec2(1, 1)),
                vertex_type(vec3(0, 1, 0), vec2(0, 1))
            };
            vertex_buffer = gpu_buffers.make<vertex_buffer_type>().lock();
            vertex_buffer.lock()->data(vertices, gpu_t::buffer_usage::STATIC_DRAW);
        }
    }

	void gui_canvas::on_render_begin(mat4& world_matrix, mat4& view_projection_matrix)
	{
		//gpu_viewport_type viewport;
		//viewport.width = static_cast<gpu_viewport_type::scalar_type>(get_size().x);
		//viewport.height = static_cast<gpu_viewport_type::scalar_type>(get_size().y);
		//viewport.x = static_cast<gpu_viewport_type::scalar_type>(get_bounds().min.x);
		//viewport.y = static_cast<gpu_viewport_type::scalar_type>(get_bounds().min.y);
		//gpu.frame_buffers.push(frame_buffer);
		//gpu.viewports.push(viewport);
	}

    void gui_canvas::on_render_end(mat4& world_matrix, mat4& view_projection_matrix)
    {
        static const size_t DIFFUSE_TEXTURE_INDEX = 0;

        //gpu.viewports.pop();
        //gpu.frame_buffers.pop();

        //TODO: for each render pass, push/pop frame buffer, do gpu program etc.

        gpu.buffers.push(gpu_t::buffer_target::ARRAY, vertex_buffer.lock());
        gpu.buffers.push(gpu_t::buffer_target::ELEMENT_ARRAY, index_buffer.lock());

        const auto gpu_program = gpu_programs.get<blur_horizontal_gpu_program>();

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

        gpu.draw_elements(gpu_t::primitive_type::TRIANGLE_FAN, 4, index_buffer_type::DATA_TYPE, 0);

        gpu.textures.unbind(DIFFUSE_TEXTURE_INDEX);

        gpu.buffers.pop(gpu_t::buffer_target::ELEMENT_ARRAY);
        gpu.buffers.pop(gpu_t::buffer_target::ARRAY);

        gpu.programs.pop();
    }

    void gui_canvas::on_clean_end()
    {
        const auto frame_buffer_size = static_cast<gpu_frame_buffer_size_type>(get_size());

        if (!frame_buffer)
        {
            frame_buffer = boost::make_shared<naga::frame_buffer>(gpu_frame_buffer_type::COLOR_DEPTH_STENCIL, frame_buffer_size);
        }
        else if(frame_buffer_size != frame_buffer->get_size())
        {
            frame_buffer->set_size(frame_buffer_size);
        }
    }
}
