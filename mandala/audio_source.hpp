#pragma once

//glm
#include <glm\gtc\type_ptr.hpp>

//al
#include <AL\al.h>

//std
#include <memory>

//mandala
#include "types.hpp"

namespace mandala
{
    struct sound_t;

    struct audio_source_t
	{
		enum class state_t
		{
			initial,
			playing,
			paused,
			stopped
		};

        audio_source_t();
        ~audio_source_t();

        inline uint32_t id() const { return _id; }

        state_t state() const;
        void position(const vec3_t& position) const;
        void velocity(const vec3_t& velocity) const;
        void gain(float32_t gain) const;
        void reference_distance(float32_t reference_distance);
        void max_distance(float32_t max_distance);

        void play();
        void pause();
        void rewind();
        void stop();
        void queue_sound(const std::shared_ptr<sound_t>& sound);
        void unqueue_sound(const std::shared_ptr<sound_t>& sound);


    private:
        audio_source_t(const audio_source_t&) = delete;
        audio_source_t& operator=(const audio_source_t&) = delete;

        uint32_t _id;
    };
}
