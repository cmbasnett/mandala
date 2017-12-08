//std
#include <array>

//boost
#include <boost\make_shared.hpp>

//glm
#include <glm\ext.hpp>

//naga
#include "openal.hpp"
#include "audio_source.hpp"
#include "audio_device.hpp"
#include "audio_context.hpp"
#include "audio_system.hpp"
#include "sound.hpp"

namespace naga
{
    AudioSystem audio;

	AudioSystem::AudioSystem()
    {
        try
        {
            //auto device = boost::make_shared<audio_device>();
            //devices.push_back(device);

            //context = boost::make_shared<audio_context>(device);

            //if (alcMakeContextCurrent(*context) == ALC_FALSE)
            //{
            //    throw std::exception();
            //}

            //if (alGetError() != ALC_NO_ERROR)
            //{
            //    throw std::exception();
            //}
        }
        catch (std::exception&)
        {
        }
    }

	void AudioSystem::tick(f32 dt)
    {
        alDopplerFactor(doppler.factor);
        alDopplerVelocity(doppler.speed_of_sound);

        alListenerfv(AL_POSITION, glm::value_ptr(listener.location));
        alListenerfv(AL_VELOCITY, glm::value_ptr(listener.velocity));
        std::array<f32, 6> orientation;
        alListenerfv(AL_ORIENTATION, orientation.data());

        auto sources_itr = sources.begin();

        while (sources_itr != sources.end())
        {
            const auto& source = sources_itr->second;

            if (source.unique() && source->get_state() == AudioSourceState::STOPPED)
            {
                sources_itr = sources.erase(sources_itr);
            }
            else
            {
                ++sources_itr;
            }
        }
    }

    u32 AudioSystem::create_source()
    {
        u32 source_id;

        alGenSources(1, &source_id); alCheckError();

        return source_id;
    }

	void AudioSystem::destroy_source(u32 source_id)
    {
        alDeleteSources(1, &source_id); alCheckError();
    }

	AudioSourceState AudioSystem::get_source_state(u32 source_id) const
    {
        ALint state;

        alGetSourcei(source_id, AL_SOURCE_STATE, &state); alCheckError();

        switch (state)
        {
        case AL_INITIAL:
			return AudioSourceState::INITIAL;
        case AL_PLAYING:
			return AudioSourceState::PLAYING;
        case AL_PAUSED:
			return AudioSourceState::PAUSED;
        case AL_STOPPED:
			return AudioSourceState::STOPPED;
        default:
            throw std::exception();
        }
    }

    void AudioSystem::set_source_location(u32 source_id, const vec3 & location) const
    {
        alSourcefv(source_id, AL_POSITION, glm::value_ptr(location)); alCheckError();
    }

    void AudioSystem::set_source_velocity(u32 source_id, const vec3 & velocity) const
    {
        alSourcefv(source_id, AL_VELOCITY, glm::value_ptr(velocity)); alCheckError();
    }

    void AudioSystem::set_source_gain(u32 source_id, f32 gain) const
    {
        alSourcef(source_id, AL_GAIN, gain); alCheckError();
    }

    void AudioSystem::set_source_reference_distance(u32 source_id, f32 reference_distance) const
    {
        alSourcef(source_id, AL_REFERENCE_DISTANCE, reference_distance); alCheckError();
    }

    void AudioSystem::set_source_max_distance(u32 source_id, f32 max_distance) const
    {
        alSourcef(source_id, AL_MAX_DISTANCE, max_distance); alCheckError();
    }

    void AudioSystem::play_source(u32 source_id) const
    {
        alSourcePlay(source_id); alCheckError();
    }

    void AudioSystem::pause_source(u32 source_id) const
    {
        alSourcePause(source_id); alCheckError();
    }

    void AudioSystem::rewind_source(u32 source_id) const
    {
        alSourceRewind(source_id); alCheckError();
    }

    void AudioSystem::stop_source(u32 source_id) const
    {
        alSourceStop(source_id); alCheckError();
    }

	void AudioSystem::source_queue_sound(u32 source_id, const boost::shared_ptr<Sound>& sound) const
    {
        auto buffer_id = sound->get_buffer_id();

        alSourceQueueBuffers(source_id, 1, &buffer_id); alCheckError();
    }

    void AudioSystem::source_unqueue_sound(u32 source_id, const boost::shared_ptr<Sound>& sound) const
    {
        auto buffer_id = sound->get_buffer_id();

        alSourceUnqueueBuffers(source_id, 1, &buffer_id); alCheckError();
    }
}