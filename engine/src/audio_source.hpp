#pragma once

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "types.hpp"
#include "audio_defs.hpp"

namespace naga
{
    struct sound;

    struct audio_source
    {
        typedef u32 id_type;

        audio_source();
        ~audio_source();

        id_type get_id() const { return id; }
        audio_source_state get_state() const;
        void set_location(const vec3& location);
        void set_velocity(const vec3& velocity);
        void set_gain(f32 gain);
        void set_reference_distance(f32 reference_distance);
        void set_max_distance(f32 max_distance);

        void play();
        void pause();
        void rewind();
        void stop();
        void queue_sound(const boost::shared_ptr<sound>& sound);
        void unqueue_sound(const boost::shared_ptr<sound>& sound);

    private:
        audio_source(const audio_source&) = delete;
        audio_source& operator=(const audio_source&) = delete;

        id_type id;
    };
}
