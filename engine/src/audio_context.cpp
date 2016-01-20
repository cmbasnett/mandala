//openal
#include "openal.hpp"

//naga
#include "audio_device.hpp"
#include "audio_context.hpp"

namespace naga
{
    audio_context::audio_context(const boost::shared_ptr<audio_device>& device)
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

    audio_context::~audio_context()
    {
        alcDestroyContext(pointer); alCheckError();
    }
}
