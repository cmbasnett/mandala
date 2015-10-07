#pragma once

//std
#include <bitset>

//mandala
#include "types.hpp"
#include "heightmap.hpp"
#include "quadtree.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "basic_gpu_program.hpp"

namespace mandala
{
    struct camera;

    struct terrain
    {
        static const auto max_size = 2048;
        typedef index_type<max_size>::type size_type;
        static const auto chunk_size = 8;
        static const auto max_chunks = (max_size / chunk_size) * (max_size / chunk_size);
        typedef index_type<max_chunks>::type chunk_index_type;
        static const auto patches_per_chunk = (chunk_size * chunk_size);
        static const auto vertices_per_chunk = (chunk_size * chunk_size) + ((chunk_size + 1) * (chunk_size + 1));
        static const auto indices_per_patch = 12;
        static const auto indices_per_strip = indices_per_patch * chunk_size;
        static const auto indices_per_chunk = indices_per_strip * chunk_size;

        typedef index_type<indices_per_chunk * max_chunks>::type index_type;
        typedef index_buffer<index_type> index_buffer_type;
        typedef basic_gpu_program::vertex_type vertex_type;
        typedef vertex_buffer<vertex_type> vertex_buffer_type;

        struct chunk_t
        {
            std::bitset<patches_per_chunk> patch_holes;
        };

        terrain(size_type width, size_type height);

        void render(const camera& camera) const;

        const heightmap& get_heightmap() const { return heightmap; }
        const quadtree& get_quadtree() const { return quadtree; }

    private:
        heightmap heightmap;
        quadtree quadtree;
        boost::multi_array<chunk_t, 2> chunks;
        boost::shared_ptr<vertex_buffer_type> vertex_buffer;
        boost::shared_ptr<index_buffer_type> index_buffer;
    };
}