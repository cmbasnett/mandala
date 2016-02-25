#pragma once

//naga
#include "types.hpp"
#include "heightmap.hpp"
#include "quadtree.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "basic_gpu_program.hpp"
#include "line.hpp"
#include "terrain_chunk.hpp"

//bullet
#include <BulletCollision\CollisionShapes\btHeightfieldTerrainShape.h>

namespace naga
{
    struct camera_params;

    struct terrain
    {
        static const auto MAX_SIZE = 2048;
        static const auto CHUNK_SIZE = 16;
        static const auto MAX_CHUNKS = (MAX_SIZE / CHUNK_SIZE) * (MAX_SIZE / CHUNK_SIZE);
        static const auto PATCHES_PER_CHUNK = (CHUNK_SIZE * CHUNK_SIZE);
        static const auto VERTICES_PER_CHUNK = (CHUNK_SIZE * CHUNK_SIZE) + ((CHUNK_SIZE + 1) * (CHUNK_SIZE + 1));
        static const auto INDICES_PER_PATCH = 12;
        static const auto INDICES_PER_STRIP = INDICES_PER_PATCH * CHUNK_SIZE;
        static const auto INDICES_PER_CHUNK = INDICES_PER_STRIP * CHUNK_SIZE;

		typedef index_type<MAX_SIZE>::type size_type;
		typedef index_type<MAX_CHUNKS>::type chunk_index_type;
		typedef index_type<INDICES_PER_CHUNK * MAX_CHUNKS>::type index_type;
        typedef index_buffer<index_type> index_buffer_type;
        typedef basic_gpu_program::vertex_type vertex_type;
        typedef vertex_buffer<vertex_type> vertex_buffer_type;

        terrain(const boost::shared_ptr<image>& image);
        terrain(size_type width, size_type height);

		void render(const camera_params& camera) const;
		f32 get_height(const vec2& location) const;
		vec3 trace(const line3& ray) const;

        const heightmap& get_heightmap() const { return heightmap; }
        const quadtree& get_quadtree() const { return quadtree; }

    private:
        void update_chunks(const rectangle_u64& rectangle);

        heightmap heightmap;
        quadtree quadtree;
        boost::multi_array<terrain_chunk, 2> chunks;
        boost::shared_ptr<vertex_buffer_type> vertex_buffer;
        boost::shared_ptr<index_buffer_type> index_buffer;
        size_t chunk_count;
    };
}