//naga
#include "audio_device.hpp"

//openal
#include "openal.hpp"

namespace naga
{
    audio_device::audio_device()
    {
        pointer = alcOpenDevice(nullptr); alCheckError();

        if (pointer == nullptr)
        {
            throw std::exception();
        }
    }

    audio_device::audio_device(const std::string& name)
    {
        pointer = alcOpenDevice(name.c_str()); alCheckError();

        if (pointer == nullptr)
        {
            throw std::exception();
        }
    }

    audio_device::~audio_device()
    {
        alcCloseDevice(pointer); alCheckError();
    }
}
