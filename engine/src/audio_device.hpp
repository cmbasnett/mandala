#pragma once

//std
#include <string>

//openal
#include <AL\alc.h>

namespace naga
{
    struct audio_device
    {
        typedef ALCdevice* pointer_type;

        audio_device();
        audio_device(const std::string& name);
        ~audio_device();

        operator pointer_type() const { return pointer; }

    private:
        audio_device(const audio_device&) = delete;
        audio_device& operator=(const audio_device&) = delete;

        pointer_type pointer = nullptr;
    };
}
