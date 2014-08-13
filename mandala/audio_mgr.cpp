//std
#include <array>

//al
#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alext.h>

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
	audio_mgr_t::audio_mgr_t()
	{
		auto device = std::make_shared<audio_device_t>();
        devices.push_back(device);

		context = std::make_shared<audio_context_t>(device);

        set_context(context);
    }

    audio_mgr_t::~audio_mgr_t()
    {
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
			auto& source = *sources_itr;

			if (source.use_count() == 1 && source->state() == audio_source_t::state_t::stopped)
			{
				sources_itr = sources.erase(sources_itr);
			}
			else
			{
				++sources_itr;
			}
		}
	}

    void audio_mgr_t::set_context(const std::shared_ptr<audio_context_t>& context)
    {
        if (alcMakeContextCurrent(context->ptr()) == ALC_FALSE)
        {
            throw std::exception();
        }

        if (alGetError() != ALC_NO_ERROR)
        {
            throw std::exception();
        }

        this->context = context;
    }

	std::shared_ptr<audio_source_t> audio_mgr_t::create_source()
	{
        auto source = std::make_shared<audio_source_t>();

		sources.push_back(source);

		return source;
	}
}
