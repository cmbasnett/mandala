#pragma once

//mandala
#include "gui_node.hpp"
#include "frame_buffer.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "gpu_vertices.hpp"

namespace mandala
{
    struct gui_canvas_t : gui_node_t
	{
        gui_canvas_t();

        static const auto index_count = 4;

        typedef position_texcoord_vertex_t vertex_type;
        typedef vertex_buffer_t<vertex_type> vertex_buffer_type;

        typedef index_type<index_count>::type index_type;
        typedef index_buffer_t<index_type> index_buffer_type;

        virtual void on_render_begin(mat4_t world_matrix, mat4_t view_projection_matrix) override;
        virtual void on_render_end(mat4_t world_matrix, mat4_t view_projection_matrix) override;
        virtual void on_cleaned() override;

        const std::shared_ptr<frame_buffer_t>& get_frame_buffer() const { return frame_buffer; }

    private:
        std::shared_ptr<frame_buffer_t> frame_buffer;
        std::shared_ptr<vertex_buffer_type> vertex_buffer;
        std::shared_ptr<index_buffer_type> index_buffer;
    };
}