#pragma once

//naga
#include "types.hpp"
#include "heightmap.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "basic_gpu_program.hpp"
#include "line.hpp"
#include "game_component.hpp"
#include "quadtree.hpp"
#include "triangle.hpp"

namespace naga
{
    struct CameraParameters;

    struct TerrainComponent : GameComponent
    {
        static const char* component_name;

		static const auto MAX_SIZE = 2049;
        static const auto CHUNK_SIZE = 8;
        static const auto MAX_CHUNKS = (MAX_SIZE / CHUNK_SIZE) * (MAX_SIZE / CHUNK_SIZE);
        static const auto PATCHES_PER_CHUNK = (CHUNK_SIZE * CHUNK_SIZE);
        static const auto VERTICES_PER_CHUNK = ((CHUNK_SIZE + 1) * (CHUNK_SIZE + 1));
        static const auto TRIANGLES_PER_PATCH = 2;
		static const auto TRIANGLES_PER_CHUNK = PATCHES_PER_CHUNK * TRIANGLES_PER_PATCH;
        static const auto INDICES_PER_PATCH = TRIANGLES_PER_PATCH * 3;
        static const auto INDICES_PER_STRIP = INDICES_PER_PATCH * CHUNK_SIZE;
        static const auto INDICES_PER_CHUNK = INDICES_PER_STRIP * CHUNK_SIZE;

		typedef IndexType<MAX_SIZE>::Type SizeType;
		typedef IndexType<MAX_CHUNKS>::Type ChunkIndexType;
		typedef IndexType<INDICES_PER_CHUNK * MAX_CHUNKS>::Type IndexType;
		typedef IndexBuffer<IndexType> IndexBufferType;
        typedef basic_gpu_program::VertexType VertexType;
		typedef VertexBuffer<VertexType> VertexBufferType;

		f32 get_height(const vec2& location) const;
		vec3 trace(const Line3& ray);

        // overrides
		void on_render(CameraParameters& camera_parameters) override;

        // getters
		const boost::shared_ptr<Heightmap>& get_heightmap() const { return heightmap; }
		const vec3 get_scale() const { return scale; }

        // setters
		void set_heightmap(const boost::shared_ptr<Image>& image);
		void set_scale(const vec3& scale);

    private:
        void update_chunks(const details::Rectangle<u64>& rectangle);

		vec3 scale = vec3(1.0f, 16.0f, 1.0f);
        boost::shared_ptr<Heightmap> heightmap;
        boost::shared_ptr<VertexBufferType> vertex_buffer;
		boost::shared_ptr<IndexBufferType> index_buffer;
		i32 width;
		i32 depth;
        size_t chunk_count;
        boost::shared_ptr<Texture> texture;
		boost::shared_ptr<QuadTree> quadtree;
		std::vector<vec3> vertices;
		std::vector<Triangle3> triangles;

		std::vector<const QuadTree::Node*> traced_nodes;
    };
}
