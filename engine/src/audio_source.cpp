//glm
#include <glm\gtc\type_ptr.hpp>

//naga
#include "audio_source.hpp"
#include "sound.hpp"
#include "audio_system.hpp"

namespace naga
{
	AudioSource::AudioSource()
    {
        id = audio.create_source();
    }

	AudioSource::~AudioSource()
    {
        audio.destroy_source(id);
    }

	AudioSourceState AudioSource::get_state() const
    {
        return audio.get_source_state(id);
    }

	void AudioSource::set_location(const vec3& location)
    {
        audio.set_source_location(id, location);
    }

	void AudioSource::set_velocity(const vec3& velocity)
    {
        audio.set_source_velocity(id, velocity);
    }

	void AudioSource::set_gain(f32 gain)
    {
        audio.set_source_gain(id, gain);
    }

	void AudioSource::set_reference_distance(f32 reference_distance)
    {
        audio.set_source_reference_distance(id, reference_distance);
    }

	void AudioSource::set_max_distance(f32 max_distance)
    {
        audio.set_source_max_distance(id, max_distance);
    }

	void AudioSource::play()
    {
        audio.play_source(id);
    }

	void AudioSource::pause()
    {
        audio.pause_source(id);
    }

	void AudioSource::rewind()
    {
        audio.rewind_source(id);
    }

	void AudioSource::stop()
    {
        audio.stop_source(id);
    }

	void AudioSource::queue_sound(const boost::shared_ptr<Sound>& sound)
    {
        audio.source_queue_sound(id, sound);
    }

	void AudioSource::unqueue_sound(const boost::shared_ptr<Sound>& sound)
    {
        audio.source_unqueue_sound(id, sound);
    }
}