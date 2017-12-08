//naga
#include "texture.hpp"
#include "image.hpp"
#include "gpu.hpp"

//naga
#include <boost\make_shared.hpp>

namespace naga
{
	Texture::Texture(ColorType color_type, const vec2& size, const void* data) :
        color_type(color_type),
        size(size)
    {
		id = gpu.create_texture(color_type, static_cast<vec2_u32>(size), data);
    }

	Texture::Texture(const boost::shared_ptr<Image>& image) :
		Texture(image->get_color_type(),
				image->get_size(),
				image->get_data().data())
    {
    }

	Texture::Texture(std::istream& istream) :
		Texture(boost::make_shared<Image>(istream))
    {
    }

	Texture::~Texture()
    {
        gpu.destroy_texture(id);
    }

	void Texture::set_size(const vec2& size)
    {
        if (size == get_size())
        {
            return;
        }
        
		gpu.resize_texture(shared_from_this(), static_cast<vec2_u32>(size));

        this->size = size;
    }
}