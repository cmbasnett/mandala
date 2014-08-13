#pragma once

//openal
#include <AL\al.h>

//std
#include <exception>

namespace mandala
{
	inline void alCheckError()
	{
		ALenum error = alGetError();

		if (error != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}
}
