//glm
#include <glm\gtc\type_ptr.hpp>

//mandala
#include "audio_source.hpp"
#include "sound.hpp"
#include "audio_mgr.hpp"

namespace mandala
{
    audio_source_t::audio_source_t()
    {
        id = audio.create_source();
    }

    audio_source_t::~audio_source_t()
    {
        audio.destroy_source(id);
    }

    audio_source_state_e audio_source_t::get_state() const
    {
        return audio.get_source_state(id);
    }

    void audio_source_t::set_location(const vec3& location)
    {
        audio.set_source_location(id, location);
    }

    void audio_source_t::set_velocity(const vec3& velocity)
    {
        audio.set_source_velocity(id, velocity);
    }

    void audio_source_t::set_gain(f32 gain)
    {
        audio.set_source_gain(id, gain);
    }

    void audio_source_t::set_reference_distance(f32 reference_distance)
    {
        audio.set_source_reference_distance(id, reference_distance);
    }

    void audio_source_t::set_max_distance(f32 max_distance)
    {
        audio.set_source_max_distance(id, max_distance);
    }

    void audio_source_t::play()
    {
        audio.play_source(id);
    }

    void audio_source_t::pause()
    {
        audio.pause_source(id);
    }

    void audio_source_t::rewind()
    {
        audio.rewind_source(id);
    }

    void audio_source_t::stop()
    {
        audio.stop_source(id);
    }

    void audio_source_t::queue_sound(const boost::shared_ptr<sound_t>& sound)
    {
        audio.source_queue_sound(id, sound);
    }

    void audio_source_t::unqueue_sound(const boost::shared_ptr<sound_t>& sound)
    {
        audio.source_unqueue_sound(id, sound);
    }
}