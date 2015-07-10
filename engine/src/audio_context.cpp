//openal
#include "openal.hpp"

//mandala
#include "audio_device.hpp"
#include "audio_context.hpp"

namespace mandala
{
    audio_context_t::audio_context_t(const boost::shared_ptr<audio_device_t>& device)
    {
        if (device == nullptr)
        {
            throw std::exception();
        }

        pointer = alcCreateContext(*device.get(), nullptr); alCheckError();

        if (pointer == nullptr)
        {
            throw std::exception();
        }

        alcProcessContext(pointer); alCheckError();
    }

    audio_context_t::~audio_context_t()
    {
        alcDestroyContext(pointer); alCheckError();
    }
}
