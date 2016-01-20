//naga
#include "texture.hpp"
#include "image.hpp"
#include "gpu.hpp"

//naga
#include <boost\make_shared.hpp>

namespace naga
{
    texture::texture(naga::color_type color_type, const size_type& size, const void* data) :
        color_type(color_type),
        size(size)
    {
        id = gpu.create_texture(color_type, size, data);
    }

    texture::texture(const boost::shared_ptr<image>& image) :
        texture(image->get_color_type(),
                  image->get_size(),
                  image->get_data().data())
    {
    }

    texture::texture(std::istream& istream) :
        texture(boost::make_shared<image>(istream))
    {
    }

    texture::~texture()
    {
        gpu.destroy_texture(id);
    }

    void texture::set_size(const size_type & size)
    {
        if (size == get_size())
        {
            return;
        }
        
        gpu.resize_texture(shared_from_this(), size);

        this->size = size;
    }
}