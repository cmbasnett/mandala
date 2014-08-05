#pragma once

//std
#include <memory>

//al
#include <AL\alc.h>

namespace mandala
{
    struct audio_device_t;

    struct audio_context_t
    {
        audio_context_t(const std::shared_ptr<audio_device_t>& device);
        ~audio_context_t();

        ALCcontext* ptr = nullptr;

    private:
        audio_context_t(const audio_context_t&) = delete;
        audio_context_t& operator=(const audio_context_t&) = delete;
    };
}
