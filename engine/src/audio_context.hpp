#pragma once

//boost
#include <boost\shared_ptr.hpp>

//openal
#include <AL\alc.h>

namespace mandala
{
    struct audio_device;

    struct audio_context
    {
        typedef ALCcontext* pointer_type;

        audio_context(const boost::shared_ptr<audio_device>& device);
        ~audio_context();

        operator pointer_type() const { return pointer; }

    private:
        audio_context(const audio_context&) = delete;
        audio_context& operator=(const audio_context&) = delete;

        pointer_type pointer = nullptr;
    };
}
