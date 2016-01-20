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
    struct camera_params;

    struct bsp : public resource
    {
        typedef i32 node_index_type;

        enum class content_type : i32
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

        enum class render_mode : u8
        {
            NORMAL,
            COLOR,
            TEXTURE,
            GLOW,
            SOLID,
            ADDITIVE
        };

        struct node
        {
            static const auto CHILD_COUNT = 2;

            u32 plane_index = 0;
            std::array<i16, CHILD_COUNT> child_indices;
            aabb3_i16 aabb;
            u16 face_start_index = 0;
            u16 face_count = 0;
        };

        struct face
        {
            typedef u8 lighting_style_type;

            static const auto LIGHTING_STYLE_COUNT = 4;
            static const lighting_style_type LIGHTING_STYLE_NONE = 255;

            u16 plane_index = 0;
            u16 plane_side = 0;
            u32 surface_edge_start_index = 0;
            u16 surface_edge_count = 0;
            u16 texture_info_index = 0;
            std::array<lighting_style_type, LIGHTING_STYLE_COUNT> lighting_styles;
            u32 lightmap_offset = 0;
        };

        struct leaf
        {
            static const auto AMBIENT_SOUND_LEVEL_COUNT = 4;
            
            typedef u8 ambient_sound_level_type;
            typedef aabb3_i16 aabb_type;

            content_type content_type = content_type::EMPTY;
            i32 visibility_offset = 0;
            aabb_type aabb;
            u16 mark_surface_start_index = 0;
            u16 mark_surface_count = 0;
            std::array<ambient_sound_level_type, AMBIENT_SOUND_LEVEL_COUNT> ambient_sound_levels;
        };

        struct edge
        {
            typedef u16 vertex_index_type;

            static const auto VERTEX_INDEX_COUNT = 2;

            std::array<vertex_index_type, VERTEX_INDEX_COUNT> vertex_indices;
        };

        struct texture_info
        {
            struct
            {
                vec3 axis;
                f32 offset = 0;
            } s, t;

            u32 texture_index = 0;
            u32 flags = 0;
        };

        struct clip_node
        {
            static const auto CHILD_COUNT = 2;

            typedef i16 child_index_type;

            i32 plane_index = 0;
            std::array<child_index_type, CHILD_COUNT> child_indices;
        };

        struct model
        {
            static const auto HEAD_NODE_INDEX_COUNT = 4;

            typedef i32 head_node_index_type;

            aabb3 aabb;
            vec3 origin;
            std::array<head_node_index_type, HEAD_NODE_INDEX_COUNT> head_node_indices;
            i32 vis_leafs = 0;
            i32 face_start_index = 0;
            i32 face_count = 0;
        };

        struct bsp_texture
        {
            static const auto MIPMAP_OFFSET_COUNT = 4;

            u32 width;
            u32 height;
            u32 mipmap_offsets[MIPMAP_OFFSET_COUNT];
        };

        struct bsp_plane
        {
            typedef plane3 plane_type;

            enum class type : u32
            {
                X,
                Y,
                Z,
                ANY_X,
                ANY_Y,
                ANY_Z
            };

            plane3 plane;
            type type = type::X;
        };

        typedef bsp_gpu_program::vertex_type vertex_type;
        typedef vertex_buffer<vertex_type> vertex_buffer_type;

        typedef u32 index_type;
        typedef index_buffer<index_type> index_buffer_type;

        struct trace_args
        {
            line3 line;
        };

        struct trace_result
        {
            bool did_hit = false;
            bool is_all_solid = false;
            vec3 location;
            plane3 plane;
            f32 ratio = 0.0f;
        };

        struct render_settings
        {
            f32 lightmap_gamma = 1.0f;
        };

        struct render_stats
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

        bsp(std::istream& istream);

        void render(const camera_params& camera_params);

        i32 get_leaf_index_from_location(const vec3& location) const;

        const render_stats& get_render_stats() const { return render_stats; }

        render_settings render_settings;  //TODO: sort this out elsewhere

    private:
        std::vector<bsp_plane> planes;
        std::vector<edge> edges;
        std::vector<face> faces;
        std::vector<i32> surface_edges;
        std::vector<node> nodes;
        std::vector<leaf> leafs;
        std::vector<u16> mark_surfaces;
        std::vector<texture_info> texture_infos;
        std::vector<boost::shared_ptr<texture>> face_lightmap_textures;
        std::vector<clip_node> clip_nodes;
        std::vector<model> models;
        std::vector<bsp_entity> entities;
        std::vector<size_t> brush_entity_indices;
        std::map<size_t, boost::dynamic_bitset<>> leaf_pvs_map;
        std::vector<size_t> face_start_indices;
        size_t vis_leaf_count = 0;
        std::vector<boost::shared_ptr<texture>> textures;
        render_stats render_stats;
        boost::shared_ptr<vertex_buffer_type> vertex_buffer;
        boost::shared_ptr<index_buffer_type> index_buffer;

        bsp(const bsp&) = delete;
        bsp& operator=(const bsp&) = delete;
    };
}
