#pragma once

//naga
#include "gui_node.hpp"
#include "frame_buffer.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "blur_horizontal_gpu_program.hpp"

namespace naga
{
	struct GUICanvas : GUINode
    {
		GUICanvas();

        static const auto INDEX_COUNT = 4;

        typedef blur_horizontal_gpu_program::VertexType VertexType;
		typedef VertexBuffer<VertexType> VertexBufferType;
        typedef IndexType<INDEX_COUNT>::Type IndexType;
		typedef IndexBuffer<IndexType> IndexBufferType;

        virtual void on_render_begin(mat4& world_matrix, mat4& view_projection_matrix) override;
        virtual void on_render_end(mat4& world_matrix, mat4& view_projection_matrix) override;
        virtual void on_clean_end() override;

		const boost::shared_ptr<FrameBuffer>& get_frame_buffer() const { return frame_buffer; }

    private:
        boost::shared_ptr<FrameBuffer> frame_buffer;
		static boost::weak_ptr<VertexBufferType> vertex_buffer;
		static boost::weak_ptr<IndexBufferType> index_buffer;
    };
}