#pragma once

//std
#include <memory>
#include <vector>

//al
#include <AL\alext.h>

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

		audio_mgr_t();
		~audio_mgr_t();

		void tick(float32_t dt);

        void set_context(const std::shared_ptr<audio_context_t>& context);
        std::shared_ptr<audio_source_t> create_source();

        listener_t listener;
        doppler_t doppler;
        std::vector<std::shared_ptr<audio_source_t>> sources;
        std::vector<std::shared_ptr<audio_device_t>> devices;

	private:
		std::shared_ptr<audio_context_t> context;
	};
};
