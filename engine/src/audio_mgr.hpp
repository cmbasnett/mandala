#pragma once

//std
#include <map>
#include <vector>
#include <array>

//boost
#include <boost\shared_ptr.hpp>

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
            f32 factor = 1.0f;
            f32 speed_of_sound = 343.0f;
        };

        struct listener_t
        {
            vec3 location;
            vec3 velocity;
        };

        struct channel_t
        {
        };

        static const int CHANNEL_COUNT = 16;

        typedef index_type<CHANNEL_COUNT>::type channel_index_type;
        typedef std::map<u32, boost::shared_ptr<audio_source_t>> sources_type;

        audio_mgr_t();

        void tick(f32 dt);

        listener_t listener;
        doppler_t doppler;

        u32 create_source();
        void destroy_source(u32 source_id);

        audio_source_state_e get_source_state(u32 source_id) const;
        void set_source_location(u32 source_id, const vec3& velocity) const;
        void set_source_velocity(u32 source_id, const vec3& velocity) const;
        void set_source_gain(u32 source_id, f32 gain) const;
        void set_source_reference_distance(u32 source_id, f32 reference_distance) const;
        void set_source_max_distance(u32 source_id, f32 max_distance) const;

        void play_source(u32 source_id) const;
        void pause_source(u32 source_id) const;
        void rewind_source(u32 source_id) const;
        void stop_source(u32 source_id) const;

        void source_queue_sound(u32 source_id, const boost::shared_ptr<sound_t>& sound) const;
        void source_unqueue_sound(u32 source_id, const boost::shared_ptr<sound_t>& sound) const;

        const sources_type& get_sources() const { return sources; }

    private:
        boost::shared_ptr<audio_context_t> context;
        sources_type sources;
        std::vector<boost::shared_ptr<audio_device_t>> devices;
    };

    extern audio_mgr_t audio;
}
