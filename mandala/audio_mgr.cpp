//al
#include <AL\al.h>

//glm
#include <glm\ext.hpp>

//mandala
#include "audio_mgr.h"
#include "sound.h"

namespace mandala
{
	audio_mgr_t::device_t::device_t()
	{
		ptr = alcOpenDevice(nullptr);

		if (ptr == nullptr)
		{
			throw std::exception();
		}
	}

	audio_mgr_t::device_t::~device_t()
	{
		alcCloseDevice(ptr);
	}

	audio_mgr_t::context_t::context_t(const std::shared_ptr<device_t>& device)
	{
		if (device == nullptr)
		{
			throw std::exception();
		}

		ptr = alcCreateContext(device->ptr, nullptr);

		if (ptr == nullptr)
		{
			throw std::exception();
		}
	}

	audio_mgr_t::context_t::~context_t()
	{
		alcDestroyContext(ptr);
	}

	audio_mgr_t::source_t::source_t()
	{
		alGenSources(1, &id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	audio_mgr_t::source_t::~source_t()
	{
		alDeleteSources(1, &id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	audio_mgr_t::source_t::state_t audio_mgr_t::source_t::get_state() const
	{
		ALint state;
		alGetSourcei(id, AL_SOURCE_STATE, &state);

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

	void audio_mgr_t::source_t::play()
	{
		alSourcePlay(id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	void audio_mgr_t::source_t::pause()
	{
		alSourcePause(id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	void audio_mgr_t::source_t::rewind()
	{
		alSourceRewind(id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	void audio_mgr_t::source_t::stop()
	{
		alSourceStop(id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	void audio_mgr_t::source_t::queue_buffer(uint32_t buffer)
	{
		alSourceQueueBuffers(id, 1, &buffer);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	void audio_mgr_t::source_t::unqueue_buffer(uint32_t buffer)
	{
		alSourceUnqueueBuffers(id, 1, &buffer);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}
	}

	audio_mgr_t::audio_mgr_t()
	{
		device = std::make_shared<device_t>();
		context = std::make_shared<context_t>(device);

		if (alcMakeContextCurrent(context->ptr) == ALC_FALSE)
		{
			throw std::exception();
		}
	}

	audio_mgr_t::~audio_mgr_t()
	{
	}

	void audio_mgr_t::tick(float32_t dt)
	{
		auto sources_itr = sources.begin();

		while (sources_itr != sources.end())
		{
			auto& source = *sources_itr;

			if (source.use_count() == 1 && source->get_state() == source_t::state_t::stopped)
			{
				sources_itr = sources.erase(sources_itr);
			}
			else
			{
				++sources_itr;
			}
		}
	}

	std::shared_ptr<audio_mgr_t::source_t> audio_mgr_t::play(std::shared_ptr<sound_t> sound)
	{
		if (sound == nullptr)
		{
			throw std::exception();
		}

		auto source = std::make_shared<source_t>();

		source->queue_buffer(sound->buffer);
		source->play();

		sources.push_back(source);

		return source;
	}
};
