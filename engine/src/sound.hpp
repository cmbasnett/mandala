#pragma once

//std
#include <vector>

//mandala
#include "resource.hpp"

namespace mandala
{
    struct sound : resource
    {
        typedef uint32_t buffer_id_type;
        typedef uint64_t duration_type;

        sound(std::istream& istream);
        virtual ~sound();

        int16_t get_channel_count() const { return channel_count; }
        int32_t get_sample_rate() const { return sample_rate; }
        int32_t get_byte_rate() const { return byte_rate; }
        int16_t get_block_align() const { return block_align; }
        int16_t get_bits_per_sample() const { return bits_per_sample; }
        buffer_id_type get_buffer_id() const { return buffer_id; }
        uint64_t get_duration() const { return duration; }

    private:
        int16_t channel_count = 0;
        int32_t sample_rate = 0;
        int32_t byte_rate = 0;
        int16_t block_align = 0;
        int16_t bits_per_sample = 0;
        buffer_id_type buffer_id = 0;
        duration_type duration;
    };
}
