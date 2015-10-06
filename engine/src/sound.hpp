#pragma once

//std
#include <vector>

//mandala
#include "resource.hpp"

namespace mandala
{
    struct sound_t : resource_t
    {
        typedef u32 buffer_id_type;
        typedef std::chrono::milliseconds duration_type;

        sound_t(std::istream& istream);
        virtual ~sound_t();

        i16 get_channel_count() const { return channel_count; }
        i32 get_sample_rate() const { return sample_rate; }
        i32 get_byte_rate() const { return byte_rate; }
        i16 get_block_align() const { return block_align; }
        i16 get_bits_per_sample() const { return bits_per_sample; }
        buffer_id_type get_buffer_id() const { return buffer_id; }
        const duration_type& get_duration() const { return duration; }

    private:
        i16 channel_count = 0;
        i32 sample_rate = 0;
        i32 byte_rate = 0;
        i16 block_align = 0;
        i16 bits_per_sample = 0;
        buffer_id_type buffer_id = 0;
        duration_type duration;
    };
}
