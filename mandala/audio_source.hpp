#pragma once

//std
#include <memory>

//mandala
#include "types.hpp"
#include "audio_defs.hpp"

namespace mandala
{
    struct sound_t;

    struct audio_source_t
	{
		typedef uint32_t id_type;

        audio_source_t();
        ~audio_source_t();

        id_type get_id() const { return id; }
        audio_source_state_e get_state() const;
        void set_position(const vec3_t& position);
        void set_velocity(const vec3_t& velocity);
        void set_gain(float32_t gain);
        void set_reference_distance(float32_t reference_distance);
        void set_max_distance(float32_t max_distance);

        void play();
        void pause();
        void rewind();
        void stop();
        void queue_sound(const std::shared_ptr<sound_t>& sound);
        void unqueue_sound(const std::shared_ptr<sound_t>& sound);

    private:
        audio_source_t(const audio_source_t&) = delete;
        audio_source_t& operator=(const audio_source_t&) = delete;

		id_type id;
    };
}
