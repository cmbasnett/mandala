#pragma once

//std
#include <exception>

//glew
#include "GL\glew.h"

namespace mandala
{
    //TODO: get rid of this
    inline void glCheckError()
    {
        GLenum error = GL_NO_ERROR;

        error = glGetError();

        if (error != GL_NO_ERROR)
        {
            throw std::exception();
        }
    }
}
