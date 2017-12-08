#pragma once

//std
#include <vector>
#include <map>

//naga
#include "resource.hpp"
#include "aabb.hpp"
#include "texture.hpp"
#include "plane.hpp"
#include "line.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "bsp_entity.hpp"
#include "bsp_gpu_program.hpp"

//boost
#include <boost\shared_ptr.hpp>
#include <boost\dynamic_bitset.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\optional.hpp>

namespace naga
{
	struct CameraParameters;

	struct BSP : public Resource
    {
        typedef i32 NodeIndexType;

        enum class ContentType : i32
        {
            EMPTY = -1,
            SOLID = -2,
            WATER = -3,
            SLIME = -4,
            LAVA = -5,
            SKY = -6,
            ORIGIN = -7,
            CLIP = -8,
            CURRENT_0 = -9,
            CURRENT_90 = -10,
            CURRENT_180 = -11,
            CURRENT_270 = -12,
            CURRENT_UP = -13,
            CURRENT_DOWN = -14,
            TRANSLUCENT = -15
        };

        enum class RenderMode : u8
        {
            NORMAL,
            COLOR,
            TEXTURE,
            GLOW,
            SOLID,
            ADDITIVE
        };

        struct Node
        {
            static const auto CHILD_COUNT = 2;

            u32 plane_index = 0;
            std::array<i16, CHILD_COUNT> child_indices;
            details::AABB3<i16> aabb;
            u16 face_start_index = 0;
            u16 face_count = 0;
        };

        struct Face
        {
            typedef u8 LightingStyleType;

            static const auto LIGHTING_STYLE_COUNT = 4;
			static const LightingStyleType LIGHTING_STYLE_NONE = 255;

            u16 plane_index = 0;
            u16 plane_side = 0;
            u32 surface_edge_start_index = 0;
            u16 surface_edge_count = 0;
            u16 texture_info_index = 0;
			std::array<LightingStyleType, LIGHTING_STYLE_COUNT> lighting_styles;
            u32 lightmap_offset = 0;
        };

        struct Leaf
        {
            static const auto AMBIENT_SOUND_LEVEL_COUNT = 4;
            
            typedef u8 AmbientSoundLevelType;
            typedef details::AABB3<i16> AABBType;

			ContentType content_type = ContentType::EMPTY;
            i32 visibility_offset = 0;
			AABBType aabb;
            u16 mark_surface_start_index = 0;
            u16 mark_surface_count = 0;
			std::array<AmbientSoundLevelType, AMBIENT_SOUND_LEVEL_COUNT> ambient_sound_levels;
        };

        struct Edge
        {
            typedef u16 VertexIndexType;

            static const auto VERTEX_INDEX_COUNT = 2;

			std::array<VertexIndexType, VERTEX_INDEX_COUNT> vertex_indices;
        };

        struct TextureInfo
        {
            struct
            {
                vec3 axis;
                f32 offset = 0;
            } s, t;

            u32 texture_index = 0;
            u32 flags = 0;
        };

        struct ClipNode
        {
            static const auto CHILD_COUNT = 2;

            typedef i16 ChildIndexType;

            i32 plane_index = 0;
			std::array<ChildIndexType, CHILD_COUNT> child_indices;
        };

        struct Model
        {
            static const auto HEAD_NODE_INDEX_COUNT = 4;

            AABB3 aabb;
            vec3 origin;
			std::array<NodeIndexType, HEAD_NODE_INDEX_COUNT> head_node_indices;
            i32 vis_leafs = 0;
            i32 face_start_index = 0;
            i32 face_count = 0;
        };

        struct BSPTexture
        {
            static const auto MIPMAP_OFFSET_COUNT = 4;

            u32 width;
            u32 height;
            u32 mipmap_offsets[MIPMAP_OFFSET_COUNT];
        };

        struct BSPPlane
        {
            typedef Plane3 PlaneType;

            enum class Type : u32
            {
                X,
                Y,
                Z,
                ANY_X,
                ANY_Y,
                ANY_Z
            };

			PlaneType plane;
			Type type = Type::X;
        };

        typedef bsp_gpu_program::VertexType VertexType;
        typedef VertexBuffer<VertexType> VertexBufferType;

        typedef u32 IndexType;
		typedef IndexBuffer<IndexType> IndexBufferType;

        struct TraceArgs
        {
            Line3 line;
        };

        struct TraceResult
        {
            bool did_hit = false;
            bool is_all_solid = false;
            vec3 location;
            Plane3 plane;
            f32 ratio = 0.0f;
        };

        struct RenderSettings
        {
            f32 lightmap_gamma = 1.0f;
        };

        struct RenderStats
        {
            u32 face_count = 0;
            u32 leaf_count = 0;
            u32 leaf_index = 0;

            void reset()
            {
                face_count = 0;
                leaf_count = 0;
                leaf_index = 0;
            }
        };

        BSP(std::istream& istream);

		void render(const CameraParameters& camera_parameters);

        i32 get_leaf_index_from_location(const vec3& location) const;

        const RenderStats& get_render_stats() const { return render_stats; }

        RenderSettings render_settings;  //TODO: sort this out elsewhere

    private:
        std::vector<BSPPlane> planes;
        std::vector<Edge> edges;
        std::vector<Face> faces;
        std::vector<i32> surface_edges;
        std::vector<Node> nodes;
        std::vector<Leaf> leafs;
        std::vector<u16> mark_surfaces;
        std::vector<TextureInfo> texture_infos;
        std::vector<boost::shared_ptr<Texture>> face_lightmap_textures;
        std::vector<ClipNode> clip_nodes;
        std::vector<Model> models;
		std::vector<BSPEntity> entities;
        std::vector<size_t> brush_entity_indices;
        std::map<size_t, boost::dynamic_bitset<>> leaf_pvs_map;
        std::vector<size_t> face_start_indices;
        size_t vis_leaf_count = 0;
		std::vector<boost::shared_ptr<Texture>> textures;
        RenderStats render_stats;
        boost::shared_ptr<VertexBufferType> vertex_buffer;
        boost::shared_ptr<IndexBufferType> index_buffer;

		BSP(const BSP&) = delete;
		BSP& operator=(const BSP&) = delete;
    };
}
