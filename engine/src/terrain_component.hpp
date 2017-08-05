#pragma once

//naga
#include "types.hpp"
#include "heightmap.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "basic_gpu_program.hpp"
#include "line.hpp"
#include "game_component.hpp"

namespace naga
{
    struct camera_params;

    struct terrain_component : game_component
    {
        static const char* component_name;

		static const auto MAX_SIZE = 2049;
        static const auto CHUNK_SIZE = 8;
        static const auto MAX_CHUNKS = (MAX_SIZE / CHUNK_SIZE) * (MAX_SIZE / CHUNK_SIZE);
        static const auto PATCHES_PER_CHUNK = (CHUNK_SIZE * CHUNK_SIZE);
        static const auto VERTICES_PER_CHUNK = ((CHUNK_SIZE + 1) * (CHUNK_SIZE + 1));
        static const auto TRIANGLES_PER_PATCH = 2;
        static const auto INDICES_PER_PATCH = TRIANGLES_PER_PATCH * 3;
        static const auto INDICES_PER_STRIP = INDICES_PER_PATCH * CHUNK_SIZE;
        static const auto INDICES_PER_CHUNK = INDICES_PER_STRIP * CHUNK_SIZE;

		typedef index_type<MAX_SIZE>::type size_type;
		typedef index_type<MAX_CHUNKS>::type chunk_index_type;
		typedef index_type<INDICES_PER_CHUNK * MAX_CHUNKS>::type index_type;
        typedef index_buffer<index_type> index_buffer_type;
        typedef basic_gpu_program::vertex_type vertex_type;
        typedef vertex_buffer<vertex_type> vertex_buffer_type;

		f32 get_height(const vec2& location) const;
		vec3 trace(const line3& ray) const;

        // overrides
        void on_render(camera_params& camera) override;

        // getters
		const boost::shared_ptr<heightmap>& get_heightmap() const { return heightmap; }
		const vec3 get_scale() const { return scale; }

        // setters
		void set_heightmap(const boost::shared_ptr<image>& image);
		void set_scale(const vec3& scale);

    private:
        void update_chunks(const rectangle_u64& rectangle);

		vec3 scale = vec3(1.0f, 16.0f, 1.0f);
        boost::shared_ptr<heightmap> heightmap;
        boost::shared_ptr<vertex_buffer_type> vertex_buffer;
        boost::shared_ptr<index_buffer_type> index_buffer;
        size_t chunk_count;
        boost::shared_ptr<texture> texture;
    };
}