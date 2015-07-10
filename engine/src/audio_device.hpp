#pragma once

//std
#include <string>

//openal
#include <AL\alc.h>

namespace mandala
{
    struct audio_device_t
    {
        typedef ALCdevice* pointer_type;

        audio_device_t();
        audio_device_t(const std::string& name);
        ~audio_device_t();

        operator pointer_type() const { return pointer; }

    private:
        audio_device_t(const audio_device_t&) = delete;
        audio_device_t& operator=(const audio_device_t&) = delete;

        pointer_type pointer = nullptr;
    };
}
