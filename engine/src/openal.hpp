#pragma once

//openal
#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alext.h>

//std
#include <exception>

namespace naga
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
