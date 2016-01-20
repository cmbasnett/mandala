//al
#include <AL\al.h>
#include <iostream>

//naga
#include "sound.hpp"
#include "io.hpp"

#define WAV_CHUNK_TAG_LENGTH    (4)
#define WAV_CHUNK_TAG           (std::array<char, WAV_CHUNK_TAG_LENGTH> { { 'R', 'I', 'F', 'F' } })

#define WAV_CHUNK_FORMAT_LENGTH (4)
#define WAV_CHUNK_FORMAT        (std::array<char, WAV_CHUNK_FORMAT_LENGTH> { { 'W', 'A', 'V', 'E' } })

#define WAV_SUBCHUNK_ID_LENGTH  (4)
#define WAV_SUBCHUNK1_ID        (std::array<char, WAV_SUBCHUNK_ID_LENGTH> { { 'f', 'm', 't', ' ' } })
#define WAV_SUBCHUNK2_ID        (std::array<char, WAV_SUBCHUNK_ID_LENGTH> { { 'd', 'a', 't', 'a' } })

namespace naga
{
    //https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
    sound::sound(std::istream& istream)
    {
        std::array<char, WAV_CHUNK_TAG_LENGTH> chunk_tag;
        read(istream, chunk_tag);

        if (chunk_tag != WAV_CHUNK_TAG)
        {
            throw std::exception();
        }

        int32_t chunk_size = 0;
        read(istream, chunk_size);
        
        std::array<char, WAV_CHUNK_FORMAT_LENGTH> chunk_format;
        read(istream, chunk_format);

        if (chunk_format != WAV_CHUNK_FORMAT)
        {
            throw std::exception();
        }

        std::array<char, WAV_SUBCHUNK_ID_LENGTH> subchunk1_id;
        int32_t subchunk1_size = 0;
        int16_t audio_format = 0;

        read(istream, subchunk1_id);

        if (subchunk1_id != WAV_SUBCHUNK1_ID)
        {
            throw std::exception();
        }

        read(istream, subchunk1_size);
        read(istream, audio_format);
        read(istream, channel_count);
        read(istream, sample_rate);
        read(istream, byte_rate);
        read(istream, block_align);
        read(istream, bits_per_sample);

        std::array<char, WAV_SUBCHUNK_ID_LENGTH> subchunk2_id;
        read(istream, subchunk2_id);

        if (subchunk2_id != WAV_SUBCHUNK2_ID)
        {
            throw std::exception();
        }

        int32_t subchunk2_size = 0;
        read(istream, subchunk2_size);

        std::vector<uint8_t> data(subchunk2_size);
        read(istream, data, subchunk2_size);

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

        duration = data.size() / byte_rate * 1000;
    }

    sound::~sound()
    {
        alDeleteBuffers(1, &buffer_id);
    }
}
