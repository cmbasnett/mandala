//openal
#include "openal.hpp"

//naga
#include "audio_device.hpp"
#include "audio_context.hpp"

namespace naga
{
	AudioContext::AudioContext(const boost::shared_ptr<AudioDevice>& device)
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

	AudioContext::~AudioContext()
    {
        alcDestroyContext(pointer); alCheckError();
    }
}
