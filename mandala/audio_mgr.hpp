#pragma once

//std
#include <memory>
#include <vector>
#include <array>

//mandala
#include "index_type.hpp"

namespace mandala
{
	struct sound_t;
    struct audio_source_t;
    struct audio_device_t;
    struct audio_context_t;

	struct audio_mgr_t
	{
        struct doppler_t
        {
            float32_t factor = 1.0f;
            float32_t speed_of_sound = 343.0f;
        };

        struct listener_t
        {
            vec3_t position;
            vec3_t velocity;
        };

		struct channel_t
		{
		};

		static const int channel_count = 16;

		typedef index_type<channel_count>::type channel_index_type;

		audio_mgr_t();

		void tick(float32_t dt);

        std::shared_ptr<audio_source_t> create_source();

        listener_t listener;
        doppler_t doppler;
        std::vector<std::shared_ptr<audio_source_t>> sources;
        std::vector<std::shared_ptr<audio_device_t>> devices;

	private:
		std::shared_ptr<audio_context_t> _context;
	};

	extern audio_mgr_t audio;
}
