//mandala
#include "frame_buffer.hpp"
#include "texture.hpp"
#include "gpu.hpp"

namespace mandala
{
    frame_buffer_t::frame_buffer_t(gpu_frame_buffer_type_e type, const gpu_frame_buffer_size_type& size) :
        type(type),
        size(size)
    {
		id = gpu.create_frame_buffer(type, size, color_texture, depth_stencil_texture, depth_texture);
	}

    frame_buffer_t::~frame_buffer_t()
    {
		gpu.destroy_frame_buffer(id);
    }

    void frame_buffer_t::on_bind() const
    {
        //NOTE: setting these flags on binding could have unforseen consequences
        auto gpu_color_state = gpu.color.get_state();
        auto gpu_depth_state = gpu.depth.get_state();

        if ((static_cast<gpu_frame_buffer_type_flags_type>(type) & gpu_frame_buffer_type_flag_color) == gpu_frame_buffer_type_flag_color)
        {
            gpu_color_state.mask.r = true;
            gpu_color_state.mask.g = true;
            gpu_color_state.mask.b = true;
            gpu_color_state.mask.a = true;
        }
        else
        {
            gpu_color_state.mask.r = false;
            gpu_color_state.mask.g = false;
            gpu_color_state.mask.b = false;
            gpu_color_state.mask.a = false;
        }

        if ((static_cast<gpu_frame_buffer_type_flags_type>(type) & gpu_frame_buffer_type_flag_depth) == gpu_frame_buffer_type_flag_depth)
        {
            gpu_depth_state.should_write_mask = true;
        }
        else
        {
            gpu_depth_state.should_write_mask = false;
        }

        gpu.color.push_state(gpu_color_state);
        gpu.depth.push_state(gpu_depth_state);

        //TODO: stencil mask
    }

    void frame_buffer_t::set_size(const gpu_frame_buffer_size_type & size)
    {
        if (size == get_size())
        {
            return;
        }

        if (color_texture != nullptr)
        {
            color_texture->set_size(size);
        }

        if (depth_texture != nullptr)
        {
            depth_texture->set_size(size);
        }

        if (depth_stencil_texture != nullptr)
        {
            depth_stencil_texture->set_size(size);
        }
    }
}
