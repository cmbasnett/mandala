//mandala
#include "audio_device.hpp"

//openal
#include "openal.hpp"

namespace mandala
{
    audio_device_t::audio_device_t()
    {
        pointer = alcOpenDevice(nullptr); alCheckError();

        if (pointer == nullptr)
        {
            throw std::exception();
        }
    }

    audio_device_t::audio_device_t(const std::string& name)
    {
        pointer = alcOpenDevice(name.c_str()); alCheckError();

        if (pointer == nullptr)
        {
            throw std::exception();
        }
    }

    audio_device_t::~audio_device_t()
    {
        alcCloseDevice(pointer); alCheckError();
    }
}
