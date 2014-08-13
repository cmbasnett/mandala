//mandala
#include "audio_device.hpp"

//openal
#include "openal.hpp"

namespace mandala
{
    audio_device_t::audio_device_t()
    {
		_ptr = alcOpenDevice(nullptr); alCheckError();

		if (_ptr == nullptr)
        {
            throw std::exception();
        }
    }

    audio_device_t::audio_device_t(const std::string& name)
    {
		_ptr = alcOpenDevice(name.c_str()); alCheckError();

		if (_ptr == nullptr)
        {
            throw std::exception();
        }
    }

    audio_device_t::~audio_device_t()
    {
		alcCloseDevice(_ptr); alCheckError();
    }
}
