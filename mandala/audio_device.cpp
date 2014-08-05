//std
#include <exception>

//mandala
#include "audio_device.hpp"

//al
#include <AL\al.h>

namespace mandala
{
    audio_device_t::audio_device_t()
    {
        ptr = alcOpenDevice(nullptr);

        if (ptr == nullptr)
        {
            throw std::exception();
        }

        if (alGetError() != AL_NO_ERROR)
        {
            throw std::exception();
        }
    }

    audio_device_t::audio_device_t(const std::string& name)
    {
        ptr = alcOpenDevice(name.c_str());

        if (ptr == nullptr)
        {
            throw std::exception();
        }

        if (alGetError() != AL_NO_ERROR)
        {
            throw std::exception();
        }
    }

    audio_device_t::~audio_device_t()
    {
        alcCloseDevice(ptr);

        if (alGetError() != AL_NO_ERROR)
        {
            throw std::exception();
        }
    }
}
