#pragma once

//std
#include <string>

//al
#include <AL\alc.h>

namespace mandala
{
    struct audio_device_t
    {
        audio_device_t();
        audio_device_t(const std::string& name);
        ~audio_device_t();

        ALCdevice* ptr = nullptr;

    private:
        audio_device_t(const audio_device_t&) = delete;
        audio_device_t& operator=(const audio_device_t&) = delete;
    };
}
