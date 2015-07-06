//mandala
#include "texture.hpp"
#include "image.hpp"
#include "gpu.hpp"

//mandala
#include <boost\make_shared.hpp>

namespace mandala
{
    texture_t::texture_t(color_type_e color_type, const size_type& size, const void* data) :
        color_type(color_type),
        size(size)
	{
        id = gpu.create_texture(color_type, size, data);
	}

    texture_t::texture_t(const boost::shared_ptr<image_t>& image) :
        texture_t(image->get_color_type(),
                  image->get_size(),
                  image->get_data().data())
    {
    }

    texture_t::texture_t(std::istream& istream) :
        texture_t(boost::make_shared<image_t>(istream))
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
        
        gpu.resize_texture(shared_from_this(), size);

        this->size = size;
    }
}