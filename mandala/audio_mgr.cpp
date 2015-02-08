//std
#include <array>

//al
#include "openal.hpp"

//glm
#include <glm\ext.hpp>

//mandala
#include "audio_source.hpp"
#include "audio_device.hpp"
#include "audio_context.hpp"
#include "audio_mgr.hpp"
#include "sound.hpp"

namespace mandala
{
	audio_mgr_t audio;

	audio_mgr_t::audio_mgr_t()
	{
		auto device = std::make_shared<audio_device_t>();

        devices.push_back(device);

		context = std::make_shared<audio_context_t>(device);

		if (alcMakeContextCurrent(*context) == ALC_FALSE)
		{
			throw std::exception();
		}

		if (alGetError() != ALC_NO_ERROR)
		{
			throw std::exception();
		}
    }

	void audio_mgr_t::tick(float32_t dt)
	{
        alDopplerFactor(doppler.factor);
        alDopplerVelocity(doppler.speed_of_sound);

        alListenerfv(AL_POSITION, glm::value_ptr(listener.position));
        alListenerfv(AL_VELOCITY, glm::value_ptr(listener.velocity));
        std::array<float32_t, 6> orientation;
        alListenerfv(AL_ORIENTATION, orientation.data());

        auto sources_itr = sources.begin();

		while (sources_itr != sources.end())
		{
            const auto& source = sources_itr->second;

			if (source.use_count() == 1 && source->get_state() == audio_source_state_e::stopped)
			{
				sources_itr = sources.erase(sources_itr);
			}
			else
			{
				++sources_itr;
			}
		}
	}

	uint32_t audio_mgr_t::create_source()
	{
        uint32_t source_id;

        alGenSources(1, &source_id); alCheckError();

		return source_id;
	}

    void audio_mgr_t::destroy_source(uint32_t source_id)
    {
        alDeleteSources(1, &source_id); alCheckError();
    }

    audio_source_state_e audio_mgr_t::get_source_state(uint32_t source_id) const
    {
        ALint state;

        alGetSourcei(source_id, AL_SOURCE_STATE, &state); alCheckError();

        switch (state)
        {
        case AL_INITIAL:
            return audio_source_state_e::initial;
        case AL_PLAYING:
            return audio_source_state_e::playing;
        case AL_PAUSED:
            return audio_source_state_e::paused;
        case AL_STOPPED:
            return audio_source_state_e::stopped;
        default:
            throw std::exception();
        }
    }

    void audio_mgr_t::set_source_location(uint32_t source_id, const vec3_t & location) const
    {
        alSourcefv(source_id, AL_POSITION, glm::value_ptr(location)); alCheckError();
    }

    void audio_mgr_t::set_source_velocity(uint32_t source_id, const vec3_t & velocity) const
    {
        alSourcefv(source_id, AL_VELOCITY, glm::value_ptr(velocity)); alCheckError();
    }

    void audio_mgr_t::set_source_gain(uint32_t source_id, float32_t gain) const
    {
        alSourcef(source_id, AL_GAIN, gain); alCheckError();
    }

    void audio_mgr_t::set_source_reference_distance(uint32_t source_id, float32_t reference_distance) const
    {
        alSourcef(source_id, AL_REFERENCE_DISTANCE, reference_distance); alCheckError();
    }

    void audio_mgr_t::set_source_max_distance(uint32_t source_id, float32_t max_distance) const
    {
        alSourcef(source_id, AL_MAX_DISTANCE, max_distance); alCheckError();
    }

    void audio_mgr_t::play_source(uint32_t source_id) const
    {
        alSourcePlay(source_id); alCheckError();
    }

    void audio_mgr_t::pause_source(uint32_t source_id) const
    {
        alSourcePause(source_id); alCheckError();
    }

    void audio_mgr_t::rewind_source(uint32_t source_id) const
    {
        alSourceRewind(source_id); alCheckError();
    }

    void audio_mgr_t::stop_source(uint32_t source_id) const
    {
        alSourceStop(source_id); alCheckError();
    }

    void audio_mgr_t::source_queue_sound(uint32_t source_id, const std::shared_ptr<sound_t>& sound) const
    {
        auto buffer_id = sound->get_buffer_id();

        alSourceQueueBuffers(source_id, 1, &buffer_id); alCheckError();
    }

    void audio_mgr_t::source_unqueue_sound(uint32_t source_id, const std::shared_ptr<sound_t>& sound) const
    {
        auto buffer_id = sound->get_buffer_id();

        alSourceUnqueueBuffers(source_id, 1, &buffer_id); alCheckError();
    }
}