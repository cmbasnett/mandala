//mandala
#include "gpu_buffer.hpp"

//glew
#include <GL\glew.h>

namespace mandala
{
    gpu_buffer_t::gpu_buffer_t()
    {
        glGenBuffers(1, &id);
    }

    gpu_buffer_t::~gpu_buffer_t()
    {
        glDeleteBuffers(1, &id);
    }
}