#pragma once

//std
#include <map>
#include <vector>
#include <array>

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "index_type.hpp"
#include "audio_defs.hpp"

namespace naga
{
	struct Sound;
    struct AudioSource;
    struct AudioDevice;
    struct AudioContext;

    struct AudioSystem
    {
		struct Doppler
        {
            f32 factor = 1.0f;
            f32 speed_of_sound = 343.0f;
        };

        struct Listener
        {
            vec3 location;
            vec3 velocity;
        };

        struct Channel
        {
        };

        static const int CHANNEL_COUNT = 16;

        typedef IndexType<CHANNEL_COUNT>::Type ChannelIndexType;
		typedef std::map<u32, boost::shared_ptr<AudioSource>> SourcesType;

		AudioSystem();

        void tick(f32 dt);

        Listener listener;
		Doppler doppler;

        u32 create_source();
        void destroy_source(u32 source_id);

        AudioSourceState get_source_state(u32 source_id) const;
        void set_source_location(u32 source_id, const vec3& velocity) const;
        void set_source_velocity(u32 source_id, const vec3& velocity) const;
        void set_source_gain(u32 source_id, f32 gain) const;
        void set_source_reference_distance(u32 source_id, f32 reference_distance) const;
        void set_source_max_distance(u32 source_id, f32 max_distance) const;

        void play_source(u32 source_id) const;
        void pause_source(u32 source_id) const;
        void rewind_source(u32 source_id) const;
        void stop_source(u32 source_id) const;

		void source_queue_sound(u32 source_id, const boost::shared_ptr<Sound>& sound) const;
		void source_unqueue_sound(u32 source_id, const boost::shared_ptr<Sound>& sound) const;

		const SourcesType& get_sources() const { return sources; }

    private:
        boost::shared_ptr<AudioContext> context;
        SourcesType sources;
        std::vector<boost::shared_ptr<AudioDevice>> devices;
    };

    extern AudioSystem audio;
}
