//mandala
#include "texture.hpp"
#include "image.hpp"
#include "gpu.hpp"

namespace mandala
{
	texture_t::texture_t(color_type_e color_type, size_value_type width, size_value_type height, const void* data) :
        color_type(color_type),
        size(width, height)
	{
        id = gpu.create_texture(color_type, width, height, data);
	}

    texture_t::texture_t(color_type_e color_type, size_type size, const void* data) :
		texture_t(color_type,
                  size.x,
                  size.y,
                  data)
	{
	}

    texture_t::texture_t(const std::shared_ptr<image_t>& image) :
        texture_t(image->get_color_type(),
                  image->get_width(),
                  image->get_height(),
                  image->get_data().data())
    {
    }

    texture_t::texture_t(std::istream& istream) :
        texture_t(std::make_shared<image_t>(istream))
    {
    }

    texture_t::~texture_t()
    {
        gpu.destroy_texture(id);
    }

    void texture_t::set_size(const size_type & size)
    {
        if (size == get_size())
        {
            return;
        }
        
        gpu.resize_texture(shared_from_this(), size.x, size.y);
    }
}