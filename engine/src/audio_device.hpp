#pragma once

//std
#include <string>

//openal
#include <AL\alc.h>

namespace naga
{
    struct AudioDevice
    {
        typedef ALCdevice* PointerType;

		AudioDevice();
		AudioDevice(const std::string& name);
		~AudioDevice();

		operator PointerType() const { return pointer; }

    private:
		AudioDevice(const AudioDevice&) = delete;
		AudioDevice& operator=(const AudioDevice&) = delete;

		PointerType pointer = nullptr;
    };
}
