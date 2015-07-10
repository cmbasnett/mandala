#pragma once

//boost
#include <boost\shared_ptr.hpp>

//openal
#include <AL\alc.h>

namespace mandala
{
    struct audio_device_t;

    struct audio_context_t
    {
        typedef ALCcontext* pointer_type;

        audio_context_t(const boost::shared_ptr<audio_device_t>& device);
        ~audio_context_t();

        operator pointer_type() const { return pointer; }

    private:
        audio_context_t(const audio_context_t&) = delete;
        audio_context_t& operator=(const audio_context_t&) = delete;

        pointer_type pointer = nullptr;
    };
}
