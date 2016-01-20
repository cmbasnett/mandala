#pragma once

//naga
#include "gui_node.hpp"
#include "frame_buffer.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "blur_horizontal_gpu_program.hpp"

namespace naga
{
    struct gui_canvas : gui_node
    {
        gui_canvas();

        static const auto INDEX_COUNT = 4;

        typedef blur_horizontal_gpu_program::vertex_type vertex_type;
        typedef vertex_buffer<vertex_type> vertex_buffer_type;
        typedef index_type<INDEX_COUNT>::type index_type;
        typedef index_buffer<index_type> index_buffer_type;

        virtual void on_render_begin(mat4& world_matrix, mat4& view_projection_matrix) override;
        virtual void on_render_end(mat4& world_matrix, mat4& view_projection_matrix) override;
        virtual void on_clean_end() override;

        const boost::shared_ptr<frame_buffer>& get_frame_buffer() const { return frame_buffer; }

    private:
        boost::shared_ptr<frame_buffer> frame_buffer;
        static boost::weak_ptr<vertex_buffer_type> vertex_buffer;
        static boost::weak_ptr<index_buffer_type> index_buffer;
    };
}