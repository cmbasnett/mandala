//al
#include <AL\al.h>
#include <iostream>

//mandala
#include "sound.hpp"

namespace mandala
{
	//https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
	sound_t::sound_t(std::istream& istream)
	{
		char chunk_tag[5] = { '\0' };
		int32_t chunk_size = 0;
		char chunk_format[5] = { '\0' };

		istream.read(chunk_tag, sizeof(char) * 4);

		if (strcmp(chunk_tag, "RIFF") != 0)
		{
			throw std::exception();
		}

		istream.read(reinterpret_cast<char*>(&chunk_size), sizeof(chunk_size));
		istream.read(reinterpret_cast<char*>(&chunk_format), sizeof(char)* 4);

		if (strcmp(chunk_format, "WAVE") != 0)
		{
			throw std::exception();
		}

		char subchunk1_id[5] = { '\0' };
		int32_t subchunk1_size = 0;
		int16_t audio_format = 0;

		istream.read(subchunk1_id, sizeof(char) * 4);

		if (strcmp(subchunk1_id, "fmt ") != 0)
		{
			throw std::exception();
		}

		istream.read(reinterpret_cast<char*>(&subchunk1_size), sizeof(subchunk1_size));
		istream.read(reinterpret_cast<char*>(&audio_format), sizeof(audio_format));
		istream.read(reinterpret_cast<char*>(&channel_count), sizeof(channel_count));
		istream.read(reinterpret_cast<char*>(&sample_rate), sizeof(sample_rate));
		istream.read(reinterpret_cast<char*>(&byte_rate), sizeof(byte_rate));
		istream.read(reinterpret_cast<char*>(&block_align), sizeof(block_align));
		istream.read(reinterpret_cast<char*>(&bits_per_sample), sizeof(bits_per_sample));

		char subchunk2_id[5] = { '\0' };
		int32_t subchunk2_size = 0;

		istream.read(subchunk2_id, sizeof(char)* 4);

		if (strcmp(subchunk2_id, "data") != 0)
		{
			throw std::exception();
		}

		istream.read(reinterpret_cast<char*>(&subchunk2_size), sizeof(subchunk2_size));

		std::vector<uint8_t> data(subchunk2_size);

		istream.read(reinterpret_cast<char*>(data.data()), subchunk2_size);

		alGenBuffers(1, &buffer_id);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}

		ALenum format;

		if (channel_count == 1)
		{
			if (bits_per_sample == 8)
			{
				format = AL_FORMAT_MONO8;
			}
			else if (bits_per_sample == 16)
			{
				format = AL_FORMAT_MONO16;
			}
		}
		else if (channel_count == 2)
		{
			if (bits_per_sample == 8)
			{
				format = AL_FORMAT_STEREO8;
			}
			else if (bits_per_sample == 16)
			{
				format = AL_FORMAT_STEREO16;
			}
		}

        alBufferData(buffer_id, format, data.data(), static_cast<ALsizei>(data.size()), sample_rate);

		if (alGetError() != AL_NO_ERROR)
		{
			throw std::exception();
		}

        duration = duration_type(static_cast<int64_t>((static_cast<float32_t>(data.size()) / byte_rate * duration_type::period::den)));
	}

	sound_t::~sound_t()
	{
        alDeleteBuffers(1, &buffer_id);
	}
};
