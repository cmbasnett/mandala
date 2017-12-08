#pragma once

//naga
#include "sprite.hpp"
#include "gui_node.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "gui_image_gpu_program.hpp"

namespace naga
{
	struct GUIImage : public GUINode
    {
        enum class TriangleMode
        {
            BOTTOM_RIGHT,
            TOP_LEFT,
            TOP_RIGHT,
            BOTTOM_LEFT,
            SLICE,
            BOTH
        };

        static const size_t VERTEX_COUNT = 16;
        static const size_t INDEX_COUNT = 54;
        
        typedef gui_image_gpu_program::VertexType VertexType;
        typedef IndexType<INDEX_COUNT>::Type IndexType;
		typedef VertexBuffer<VertexType> VertexBufferType;
		typedef IndexBuffer<IndexType> IndexBufferType;

        GUIImage();

        const boost::optional<Sprite>& get_sprite() const { return sprite; }
        bool get_is_autosized_to_sprite() const { return is_autosized_to_sprite; }
        TriangleMode get_triangle_mode() const { return triangle_mode; }
        const vec2& get_texcoord_scale() const { return texcoord_scale; }
        const vec2& get_texcoord_origin() const { return texcoord_origin; }
        f32 get_texcoord_rotation() const { return texcoord_rotation; }
		const Padding& get_slice_padding() const { return slice_padding; }

		void set_sprite(boost::optional<Sprite> sprite);
        void set_is_autosized_to_sprite(bool is_autosized_to_sprite);
        void set_triangle_mode(TriangleMode triangle_mode) { this->triangle_mode = triangle_mode; }
        void set_texcoord_scale(const vec2& texcoord_scale) { this->texcoord_scale = texcoord_scale; }
        void set_texcoord_origin(const vec2& texcoord_origin) { this->texcoord_origin = texcoord_origin; }
        void set_texcoord_rotation(f32 texcoord_rotation) { this->texcoord_rotation = texcoord_rotation; }
		void set_slice_padding(const Padding& slice_padding) { this->slice_padding = slice_padding; }

    private:
        boost::shared_ptr<VertexBufferType> vertex_buffer;
        boost::shared_ptr<IndexBufferType> index_buffer;
		boost::optional<Sprite> sprite;
        bool is_autosized_to_sprite = false;
        TriangleMode triangle_mode = TriangleMode::BOTH;
        vec2 texcoord_scale = vec2(1.0f);
        vec2 texcoord_origin = vec2(0.5f);
        vec2 texcoord_offset;
        f32 texcoord_rotation = 0.0f;
        Padding slice_padding;

        virtual void on_render_begin(mat4& world_matrix, mat4& view_projection_matrix) override;
        virtual void on_clean_end() override;
    };
}
