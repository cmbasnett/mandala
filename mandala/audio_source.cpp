//openal
#include "openal.hpp"

//glm
#include <glm\gtc\type_ptr.hpp>

//mandala
#include "audio_source.hpp"
#include "sound.hpp"

namespace mandala
{
    audio_source_t::audio_source_t()
	{
		alGenSources(1, &_id); alCheckError();
	}

    audio_source_t::~audio_source_t()
	{
		alDeleteSources(1, &_id); alCheckError();
	}

	audio_source_t::id_type audio_source_t::id() const
	{
		return _id;
	}

	audio_source_t::state_t audio_source_t::state() const
	{
		ALint state;
		alGetSourcei(_id, AL_SOURCE_STATE, &state); alCheckError();

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
		alSourcefv(_id, AL_POSITION, glm::value_ptr(position)); alCheckError();
    }

    void audio_source_t::velocity(const vec3_t& velocity) const
    {
		alSourcefv(_id, AL_VELOCITY, glm::value_ptr(velocity)); alCheckError();
    }

    void audio_source_t::gain(float32_t gain) const
    {
		alSourcef(_id, AL_GAIN, gain); alCheckError();
    }

    void audio_source_t::reference_distance(float32_t reference_distance)
    {
		alSourcef(_id, AL_REFERENCE_DISTANCE, reference_distance); alCheckError();
    }

    void audio_source_t::max_distance(float32_t max_distance)
    {
		alSourcef(_id, AL_MAX_DISTANCE, max_distance); alCheckError();
    }

	void audio_source_t::play()
	{
		alSourcePlay(_id); alCheckError();
	}

	void audio_source_t::pause()
	{
		alSourcePause(_id); alCheckError();
	}

	void audio_source_t::rewind()
	{
		alSourceRewind(_id); alCheckError();
	}

	void audio_source_t::stop()
	{
		alSourceStop(_id); alCheckError();
	}

	void audio_source_t::queue_sound(const std::shared_ptr<sound_t>& sound)
	{
		alSourceQueueBuffers(_id, 1, &sound->buffer_id); alCheckError();
	}

    void audio_source_t::unqueue_sound(const std::shared_ptr<sound_t>& sound)
	{
		alSourceUnqueueBuffers(_id, 1, &sound->buffer_id); alCheckError();
	}
}