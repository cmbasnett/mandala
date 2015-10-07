//glm
#include <glm\gtc\type_ptr.hpp>

//mandala
#include "audio_source.hpp"
#include "sound.hpp"
#include "audio_mgr.hpp"

namespace mandala
{
    audio_source::audio_source()
    {
        id = audio.create_source();
    }

    audio_source::~audio_source()
    {
        audio.destroy_source(id);
    }

    audio_source_state audio_source::get_state() const
    {
        return audio.get_source_state(id);
    }

    void audio_source::set_location(const vec3& location)
    {
        audio.set_source_location(id, location);
    }

    void audio_source::set_velocity(const vec3& velocity)
    {
        audio.set_source_velocity(id, velocity);
    }

    void audio_source::set_gain(f32 gain)
    {
        audio.set_source_gain(id, gain);
    }

    void audio_source::set_reference_distance(f32 reference_distance)
    {
        audio.set_source_reference_distance(id, reference_distance);
    }

    void audio_source::set_max_distance(f32 max_distance)
    {
        audio.set_source_max_distance(id, max_distance);
    }

    void audio_source::play()
    {
        audio.play_source(id);
    }

    void audio_source::pause()
    {
        audio.pause_source(id);
    }

    void audio_source::rewind()
    {
        audio.rewind_source(id);
    }

    void audio_source::stop()
    {
        audio.stop_source(id);
    }

    void audio_source::queue_sound(const boost::shared_ptr<sound>& sound)
    {
        audio.source_queue_sound(id, sound);
    }

    void audio_source::unqueue_sound(const boost::shared_ptr<sound>& sound)
    {
        audio.source_unqueue_sound(id, sound);
    }
}