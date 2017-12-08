#pragma once

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "types.hpp"
#include "audio_defs.hpp"

namespace naga
{
	struct Sound;

    struct AudioSource
    {
        typedef u32 IdType;

		AudioSource();
		~AudioSource();

		IdType get_id() const { return id; }
		AudioSourceState get_state() const;
        void set_location(const vec3& location);
        void set_velocity(const vec3& velocity);
        void set_gain(f32 gain);
        void set_reference_distance(f32 reference_distance);
        void set_max_distance(f32 max_distance);

        void play();
        void pause();
        void rewind();
        void stop();
		void queue_sound(const boost::shared_ptr<Sound>& sound);
		void unqueue_sound(const boost::shared_ptr<Sound>& sound);

    private:
		AudioSource(const AudioSource&) = delete;
		AudioSource& operator=(const AudioSource&) = delete;

		IdType id;
    };
}
