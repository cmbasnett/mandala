//naga
#include "audio_device.hpp"

//openal
#include "openal.hpp"

namespace naga
{
	AudioDevice::AudioDevice()
	{
		auto b = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");

		const auto device_name = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);

		pointer = alcOpenDevice(nullptr);
		alCheckError();

		if (pointer == nullptr)
		{
			throw std::exception();
		}
	}

	AudioDevice::AudioDevice(const std::string& name)
    {
        pointer = alcOpenDevice(name.c_str()); alCheckError();

        if (pointer == nullptr)
        {
            throw std::exception();
        }
    }

	AudioDevice::~AudioDevice()
    {
        alcCloseDevice(pointer); alCheckError();
    }
}
