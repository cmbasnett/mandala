#pragma once

//std
#include <memory>
#include <map>
#include <vector>
#include <array>

//mandala
#include "index_type.hpp"
#include "audio_defs.hpp"

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
        typedef std::map<uint32_t, std::shared_ptr<audio_source_t>> sources_type;

		audio_mgr_t();

		void tick(float32_t dt);

        listener_t listener;
        doppler_t doppler;

        uint32_t create_source();
        void destroy_source(uint32_t source_id);

        audio_source_state_e get_source_state(uint32_t source_id) const;
        void set_source_location(uint32_t source_id, const vec3_t& velocity) const;
        void set_source_velocity(uint32_t source_id, const vec3_t& velocity) const;
        void set_source_gain(uint32_t source_id, float32_t gain) const;
        void set_source_reference_distance(uint32_t source_id, float32_t reference_distance) const;
        void set_source_max_distance(uint32_t source_id, float32_t max_distance) const;

        void play_source(uint32_t source_id) const;
        void pause_source(uint32_t source_id) const;
        void rewind_source(uint32_t source_id) const;
        void stop_source(uint32_t source_id) const;

        void source_queue_sound(uint32_t source_id, const std::shared_ptr<sound_t>& sound) const;
        void source_unqueue_sound(uint32_t source_id, const std::shared_ptr<sound_t>& sound) const;

        const sources_type& get_sources() const { return sources; }

	private:
		std::shared_ptr<audio_context_t> context;
        sources_type sources;
        std::vector<std::shared_ptr<audio_device_t>> devices;
	};

	extern audio_mgr_t audio;
}
