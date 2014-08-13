#pragma once

//std
#include <memory>

//openal
#include <AL\alc.h>

namespace mandala
{
    struct audio_device_t;

    struct audio_context_t
    {
		typedef ALCcontext* ptr_type;

        audio_context_t(const std::shared_ptr<audio_device_t>& device);
        ~audio_context_t();

		ptr_type ptr() const { return _ptr; }

    private:
        audio_context_t(const audio_context_t&) = delete;
		audio_context_t& operator=(const audio_context_t&) = delete;

		ptr_type _ptr = nullptr;
    };
}
