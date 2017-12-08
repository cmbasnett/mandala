#pragma once

//boost
#include <boost\shared_ptr.hpp>

//openal
#include <AL\alc.h>

namespace naga
{
    struct AudioDevice;

    struct AudioContext
    {
        typedef ALCcontext* PointerType;

		AudioContext(const boost::shared_ptr<AudioDevice>& device);
		~AudioContext();

		operator PointerType() const { return pointer; }

    private:
		AudioContext(const AudioContext&) = delete;
		AudioContext& operator=(const AudioContext&) = delete;

		PointerType pointer = nullptr;
    };
}
