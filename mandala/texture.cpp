//png
#include "png.h"

//mandala
#include "texture.h"
#include "image.h"
#include "app.h"

namespace mandala
{
	texture_t::texture_t(std::shared_ptr<image_t> image)
	{
		height = image->height;
		width = image->width;
		hash = image->hash;

		switch (image->color_type)
		{
			case image_t::color_type_t::g:
			{
				format = GL_LUMINANCE;
				internal_format = 1;
				has_alpha = false;
			}
				break;
			case image_t::color_type_t::rgb:
			{
				format = GL_RGB;
				internal_format = 3;
				has_alpha = false;
			}
				break;
			case image_t::color_type_t::rgba:
			{
				format = GL_RGBA;
				internal_format = 4;
				has_alpha = true;
			}
				break;
			case image_t::color_type_t::ga:
			{
				format = GL_LUMINANCE_ALPHA;
				internal_format = 2;
				has_alpha = true;
			}
				break;
			default:
			{
				throw std::exception();
			}
				break;
		}

		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);

		GLint unpack_alignment;

		glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 2);
		//glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image->width, image->height, 0, format, GL_UNSIGNED_BYTE, image->data.data());

		glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);
	}

	texture_t::texture_t(std::istream& istream) :
		texture_t(std::make_unique<image_t>(istream))
	{
	}

	texture_t::~texture_t()
	{
		glDeleteTextures(1, &handle);
	}
};
