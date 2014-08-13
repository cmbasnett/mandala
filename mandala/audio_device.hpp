#pragma once

//std
#include <string>

//openal
#include <AL\alc.h>

namespace mandala
{
    struct audio_device_t
    {
		typedef ALCdevice* ptr_type;

        audio_device_t();
        audio_device_t(const std::string& name);
		~audio_device_t();

		ptr_type ptr() const { return _ptr; }

    private:
        audio_device_t(const audio_device_t&) = delete;
		audio_device_t& operator=(const audio_device_t&) = delete;

		ptr_type _ptr = nullptr;
    };
}
