#pragma once

//boost
#include <boost\shared_ptr.hpp>

//mandala
#include "types.hpp"
#include "audio_defs.hpp"

namespace mandala
{
    struct sound_t;

    struct audio_source_t
    {
        typedef u32 id_type;

        audio_source_t();
        ~audio_source_t();

        id_type get_id() const { return id; }
        audio_source_state_e get_state() const;
        void set_location(const vec3& location);
        void set_velocity(const vec3& velocity);
        void set_gain(f32 gain);
        void set_reference_distance(f32 reference_distance);
        void set_max_distance(f32 max_distance);

        void play();
        void pause();
        void rewind();
        void stop();
        void queue_sound(const boost::shared_ptr<sound_t>& sound);
        void unqueue_sound(const boost::shared_ptr<sound_t>& sound);

    private:
        audio_source_t(const audio_source_t&) = delete;
        audio_source_t& operator=(const audio_source_t&) = delete;

        id_type id;
    };
}
