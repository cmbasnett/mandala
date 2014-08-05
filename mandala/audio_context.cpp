//al
#include <AL\al.h>

//mandala
#include "audio_device.hpp"
#include "audio_context.hpp"

namespace mandala
{
    audio_context_t::audio_context_t(const std::shared_ptr<audio_device_t>& device)
    {
        if (device == nullptr)
        {
            throw std::exception();
        }

        ptr = alcCreateContext(device->ptr, nullptr);

        if (ptr == nullptr)
        {
            throw std::exception();
        }

        if (alGetError() != AL_NO_ERROR)
        {
            throw std::exception();
        }

        alcProcessContext(ptr);

        if (alGetError() != AL_NO_ERROR)
        {
            throw std::exception();
        }
    }

    audio_context_t::~audio_context_t()
    {
        alcDestroyContext(ptr);

        if (alGetError() != AL_NO_ERROR)
        {
            throw std::exception();
        }
    }
}
