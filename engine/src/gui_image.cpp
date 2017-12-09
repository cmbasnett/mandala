//glm
#include "glm\ext.hpp"

//naga
#include "gpu_program_mgr.hpp"
#include "gpu_buffer_mgr.hpp"
#include "gpu_program.hpp"
#include "texture.hpp"
#include "sprite_set.hpp"
#include "sprite.hpp"
#include "gui_image.hpp"
#include "gpu.hpp"
#include "gui_image_gpu_program.hpp"
#include "collision.hpp"
#include "input_event.hpp"

namespace naga
{
	GUIImage::GUIImage() :
        index_buffer(gpu_buffers.make<IndexBufferType>()),
        vertex_buffer(gpu_buffers.make<VertexBufferType>())
    {
		std::initializer_list<IndexBufferType::IndexType> indices = {
            0, 1, 2, 
            0, 2, 3, 
            1, 2, 3, 
            1, 3, 0,
            0, 4, 6, 4, 7, 6,
            4, 5, 7, 5, 8, 7,
            5, 1, 8, 1, 9, 8,
            6, 7, 10, 7, 11, 10,
            7, 8, 11, 8, 12, 11,
            8, 9, 12, 9, 13, 12,
            10, 11, 3, 11, 14, 3,
            11, 12, 14, 12, 15, 14,
            12, 13, 15, 13, 2, 15
        };
        index_buffer->data(indices, Gpu::BufferUsage::STATIC_DRAW);
    }

    void GUIImage::on_clean_end()
    {
        //need to take size into consideration! (size is frame size, don't worry)
        const auto& size = get_size();
        auto slice_uv = slice_padding;

        slice_uv.bottom /= sprite->get_region().source_size.y;
        slice_uv.left /= sprite->get_region().source_size.x;
        slice_uv.right /= sprite->get_region().source_size.x;
        slice_uv.top /= sprite->get_region().source_size.y;

        //TODO: find a better way to generalize this so it's less of a mess
        //TODO: the UVs are not tiling here, they should be!

        // 3--E----F--2
        // |  |    |  |
        // A--B----C--D
        // |  |    |  |
        // 6--7----8--9
        // |  |    |  |
        // 0--4----5--1
        auto vertices = {
            VertexType(vec3(0, 0, 0), vec2(0, 0)),
            VertexType(vec3(size.x, 0, 0), vec2(1, 0)),
            VertexType(vec3(size.x, size.y, 0), vec2(1, 1)),
            VertexType(vec3(0, size.y, 0), vec2(0, 1)),

            VertexType(vec3(0 + slice_padding.left, 0, 0), vec2(slice_uv.left, 0)),
            VertexType(vec3(size.x - slice_padding.right, 0, 0), vec2(1.0f - slice_uv.right, 0)),

            VertexType(vec3(0, slice_padding.bottom, 0), vec2(0, slice_uv.bottom)),
            VertexType(vec3(slice_padding.left, slice_padding.bottom, 0), vec2(slice_uv.left, slice_uv.bottom)),
            VertexType(vec3(size.x - slice_padding.right, slice_padding.bottom, 0), vec2(1.0f - slice_uv.right, slice_uv.bottom)),
            VertexType(vec3(size.x, slice_padding.bottom, 0), vec2(1.0f, slice_uv.bottom)),

            VertexType(vec3(0, size.y - slice_padding.top, 0), vec2(0, 1.0f - slice_uv.top)),
            VertexType(vec3(slice_padding.left, size.y - slice_padding.top, 0), vec2(slice_uv.left, 1.0f - slice_uv.top)),
            VertexType(vec3(size.x - slice_padding.right, size.y - slice_padding.top, 0), vec2(1.0f - slice_uv.right, 1.0f - slice_uv.top)),
            VertexType(vec3(size.x, size.y - slice_padding.top, 0), vec2(1.0f, 1.0f - slice_uv.top)),

            VertexType(vec3(0 + slice_padding.left, size.y, 0), vec2(slice_uv.left, 1.0)),
            VertexType(vec3(size.x - slice_padding.right, size.y, 0), vec2(1.0f - slice_uv.right, 1.0f)),
        };

        vertex_buffer->data(vertices, Gpu::BufferUsage::STATIC_DRAW);
    }

    void GUIImage::set_sprite(boost::optional<Sprite> sprite)
    {
        this->sprite = sprite;

        if (sprite && is_autosized_to_sprite)
        {
            set_size_modes(GUISizeModes(GUISizeMode::ABSOLUTE, GUISizeMode::ABSOLUTE));
            set_size(static_cast<SizeType>(sprite->get_region().source_size));
        }
    }

    void GUIImage::set_is_autosized_to_sprite(bool is_autosized_to_sprite)
    {
        this->is_autosized_to_sprite = is_autosized_to_sprite;

        set_sprite(sprite);
    }

    void GUIImage::on_render_begin(mat4& world_matrix, mat4& view_projection_matrix)
    {
        if (sprite)
        {
            const auto& region = sprite->get_region();

            auto blend_state = gpu.blend.get_state();
            blend_state.is_enabled = true;
            blend_state.src_factor = Gpu::BlendFactor::SRC_ALPHA;
            blend_state.dst_factor = Gpu::BlendFactor::ONE_MINUS_SRC_ALPHA;

            gpu.blend.push_state(blend_state);

            //buffers
            gpu.buffers.push(Gpu::BufferTarget::ARRAY, vertex_buffer);
            gpu.buffers.push(Gpu::BufferTarget::ELEMENT_ARRAY, index_buffer);

            const auto& gpu_program = gpu_programs.get<gui_image_gpu_program>();

            //program
            gpu.programs.push(gpu_program);

            const auto& min = get_bounds().min;
            const auto& size = get_size();

            static const auto DIFFUSE_TEXTURE_INDEX = 0;

			auto color = get_color();

            gpu.set_uniform("diffuse_texture", DIFFUSE_TEXTURE_INDEX);
            gpu.set_uniform("world_matrix", world_matrix * glm::translate(min.x, min.y, 0.0f));
            gpu.set_uniform("view_projection_matrix", view_projection_matrix);
            gpu.set_uniform("color", color);
            gpu.set_uniform("texcoord_origin", texcoord_origin);
            gpu.set_uniform("texcoord_scale", texcoord_scale);
            gpu.set_uniform("rotation", (region.is_rotated ? 1.57079633f : 0) + texcoord_rotation); //TODO: make this a bit more readable
            gpu.set_uniform("region_min", region.frame_uv.min);
            gpu.set_uniform("region_max", region.frame_uv.max);
            gpu.set_uniform("frame_min", region.uv.min);
            gpu.set_uniform("frame_max", region.uv.max);

            gpu.textures.bind(DIFFUSE_TEXTURE_INDEX, sprite->get_texture());

            switch (triangle_mode)
            {
            case TriangleMode::BOTTOM_RIGHT:
                gpu.draw_elements(Gpu::PrimitiveType::TRIANGLES, 3, IndexBufferType::DATA_TYPE, 0);
                break;
            case TriangleMode::TOP_LEFT:
                gpu.draw_elements(Gpu::PrimitiveType::TRIANGLES, 3, IndexBufferType::DATA_TYPE, 3);
                break;
            case TriangleMode::TOP_RIGHT:
                gpu.draw_elements(Gpu::PrimitiveType::TRIANGLES, 3, IndexBufferType::DATA_TYPE, 6);
                break;
            case TriangleMode::BOTTOM_LEFT:
                gpu.draw_elements(Gpu::PrimitiveType::TRIANGLES, 3, IndexBufferType::DATA_TYPE, 9);
                break;
            case TriangleMode::BOTH:
                gpu.draw_elements(Gpu::PrimitiveType::TRIANGLES, 6, IndexBufferType::DATA_TYPE, 0);
                break;
            case TriangleMode::SLICE:
                gpu.draw_elements(Gpu::PrimitiveType::TRIANGLES, 54, IndexBufferType::DATA_TYPE, 12);
                break;
            }

            gpu.textures.unbind(DIFFUSE_TEXTURE_INDEX);

            gpu.programs.pop();

            gpu.buffers.pop(Gpu::BufferTarget::ELEMENT_ARRAY);
            gpu.buffers.pop(Gpu::BufferTarget::ARRAY);

            gpu.blend.pop_state();
        }

		GUINode::on_render_begin(world_matrix, view_projection_matrix);
    }
}
