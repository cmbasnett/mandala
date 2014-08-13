//openal
#include "openal.hpp"

//mandala
#include "audio_device.hpp"
#include "audio_context.hpp"

namespace mandala
{
    audio_context_t::audio_context_t(const std::shared_ptr<audio_device_t>& device)
    {
        if (device == nullptr)
        {
            throw std::exception();
        }

		_ptr = alcCreateContext(device->ptr(), nullptr); alCheckError();

        if (_ptr == nullptr)
        {
            throw std::exception();
        }

		alcProcessContext(_ptr); alCheckError();
    }

    audio_context_t::~audio_context_t()
    {
		alcDestroyContext(_ptr); alCheckError();
    }
}
