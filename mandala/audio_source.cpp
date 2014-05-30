//al
#include <AL\al.h>

//glm
#include <glm\gtc\type_ptr.hpp>

//mandala
#include "audio_source.h"
#include "sound.h"

namespace mandala
{
    audio_source_t::audio_source_t()
	{
		alGenSources(1, &_id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

    audio_source_t::~audio_source_t()
	{
        alDeleteSources(1, &_id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	audio_source_t::state_t audio_source_t::state() const
	{
		ALint state;
        alGetSourcei(_id, AL_SOURCE_STATE, &state);

        if (alGetError() != AL_NO_ERROR)
        {
            throw std::exception();
        }

		switch (state)
		{
		case AL_INITIAL:
			return state_t::initial;
		case AL_PLAYING:
			return state_t::playing;
		case AL_PAUSED:
			return state_t::paused;
		case AL_STOPPED:
			return state_t::stopped;
		default:
			throw std::exception();
		}
    }

    void audio_source_t::position(const vec3_t& position) const
    {
        alSourcefv(_id, AL_POSITION, glm::value_ptr(position));
    }

    void audio_source_t::velocity(const vec3_t& velocity) const
    {
        alSourcefv(_id, AL_VELOCITY, glm::value_ptr(velocity));
    }

    void audio_source_t::gain(float32_t gain) const
    {
        alSourcef(_id, AL_GAIN, gain);
    }

    void audio_source_t::reference_distance(float32_t reference_distance)
    {
        alSourcef(_id, AL_REFERENCE_DISTANCE, reference_distance);
    }

    void audio_source_t::max_distance(float32_t max_distance)
    {
        alSourcef(_id, AL_MAX_DISTANCE, max_distance);
    }

	void audio_source_t::play()
	{
		alSourcePlay(_id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	void audio_source_t::pause()
	{
        alSourcePause(_id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	void audio_source_t::rewind()
	{
        alSourceRewind(_id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	void audio_source_t::stop()
	{
        alSourceStop(_id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	void audio_source_t::queue_sound(const std::shared_ptr<sound_t>& sound)
	{
        alSourceQueueBuffers(_id, 1, &sound->buffer_id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

    void audio_source_t::unqueue_sound(const std::shared_ptr<sound_t>& sound)
	{
        alSourceUnqueueBuffers(_id, 1, &sound->buffer_id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}
}