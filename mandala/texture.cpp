//png
#include <png.h>

//mandala
#include "opengl.hpp"
#include "texture.hpp"
#include "image.hpp"
#include "app.hpp"

namespace mandala
{
    //TODO: move all these GL calls into the gpu_mgr_t class
    texture_t::texture_t(color_type_e color_type, uint32_t width, uint32_t height) :
        width(width),
        height(height)
    {
        glGenTextures(1, &id); glCheckError();
        glBindTexture(GL_TEXTURE_2D, id); glCheckError();

        get_formats(color_type, internal_format, format, type);

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, nullptr); glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glCheckError();
        glBindTexture(GL_TEXTURE_2D, 0); glCheckError();
    }

    texture_t::texture_t(color_type_e color_type, vec2_u32_t size) :
        texture_t(color_type, size.x, size.y)
    {
    }

    texture_t::texture_t(std::shared_ptr<image_t> image)
    {
        hash = image->hash;
        height = image->height;
        width = image->width;

        get_formats(image->color_type, internal_format, format, type);

        glGenTextures(1, &id); glCheckError();
        glBindTexture(GL_TEXTURE_2D, id); glCheckError();

        GLint unpack_alignment;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment); glCheckError();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); glCheckError();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image->width, image->height, 0, format, type, image->data.data()); glCheckError();
        glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment); glCheckError();
        glBindTexture(GL_TEXTURE_2D, 0); glCheckError();
    }

    texture_t::texture_t(std::istream& istream) :
        texture_t(std::make_shared<image_t>(istream))
    {
    }

    texture_t::~texture_t()
    {
        glDeleteTextures(1, &id);
    }

    void texture_t::get_formats(color_type_e color_type, int32_t& internal_format, int32_t& format, int32_t& type)
    {
        switch (color_type)
        {
        case color_type_e::g:
            format = GL_LUMINANCE;
            internal_format = 1;
            type = GL_UNSIGNED_BYTE;
            break;
        case color_type_e::rgb:
            format = GL_RGB;
            internal_format = GL_RGB;
            type = GL_UNSIGNED_BYTE;
            break;
        case color_type_e::rgba:
            format = GL_RGBA;
            internal_format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
        case color_type_e::ga:
            format = GL_LUMINANCE_ALPHA;
            internal_format = 2;
            type = GL_UNSIGNED_BYTE;
            break;
        case color_type_e::depth:
            format = GL_DEPTH_COMPONENT;
            internal_format = GL_DEPTH_COMPONENT;
            type = GL_UNSIGNED_BYTE;
            break;
        case color_type_e::depth_stencil:
            format = GL_DEPTH_STENCIL;
            internal_format = GL_DEPTH24_STENCIL8;
            type = GL_UNSIGNED_INT_24_8;
            break;
        default:
            throw std::exception();
        }
    }
}
