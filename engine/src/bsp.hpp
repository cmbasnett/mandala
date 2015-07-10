#pragma once

//std
#include <vector>
#include <map>

//mandala
#include "resource.hpp"
#include "aabb.hpp"
#include "texture.hpp"
#include "plane.hpp"
#include "line.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "bsp_entity.hpp"

//boost
#include <boost\shared_ptr.hpp>
#include <boost\dynamic_bitset.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\optional.hpp>

namespace mandala
{
    struct camera_t;

    struct bsp_t : public resource_t
    {
        typedef int32_t node_index_type;

        enum class content_type_e : int32_t
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

        enum class render_mode_e : uint8_t
        {
            NORMAL,
            COLOR,
            TEXTURE,
            GLOW,
            SOLID,
            ADDITIVE
        };

        struct node_t
        {
            static const auto CHILD_COUNT = 2;

            uint32_t plane_index = 0;
            std::array<int16_t, CHILD_COUNT> child_indices;
            aabb3_i16_t aabb;
            uint16_t face_start_index = 0;
            uint16_t face_count = 0;
        };

        struct face_t
        {
            typedef uint8_t lighting_style_type;

            static const auto LIGHTING_STYLE_COUNT = 4;
            static const lighting_style_type LIGHTING_STYLE_NONE = 255;

            uint16_t plane_index = 0;
            uint16_t plane_side = 0;
            uint32_t surface_edge_start_index = 0;
            uint16_t surface_edge_count = 0;
            uint16_t texture_info_index = 0;
            std::array<lighting_style_type, LIGHTING_STYLE_COUNT> lighting_styles;
            uint32_t lightmap_offset = 0;
        };

        struct leaf_t
        {
            static const auto AMBIENT_SOUND_LEVEL_COUNT = 4;
            
            typedef uint8_t ambient_sound_level_type;
            typedef aabb3_i16_t aabb_type;

            content_type_e content_type = content_type_e::EMPTY;
            int32_t visibility_offset = 0;
            aabb_type aabb;
            uint16_t mark_surface_start_index = 0;
            uint16_t mark_surface_count = 0;
            std::array<ambient_sound_level_type, AMBIENT_SOUND_LEVEL_COUNT> ambient_sound_levels;
        };

        struct edge_t
        {
            typedef uint16_t vertex_index_type;

            static const auto VERTEX_INDEX_COUNT = 2;

            std::array<vertex_index_type, VERTEX_INDEX_COUNT> vertex_indices;
        };

        struct texture_info_t
        {
            struct
            {
                vec3_t axis;
                float32_t offset = 0;
            } s, t;

            uint32_t texture_index = 0;
            uint32_t flags = 0;
        };

        struct clip_node_t
        {
            static const auto CHILD_COUNT = 2;

            typedef int16_t child_index_type;

            int32_t plane_index = 0;
            std::array<child_index_type, CHILD_COUNT> child_indices;
        };

        struct model_t
        {
            static const auto HEAD_NODE_INDEX_COUNT = 4;

            typedef int32_t head_node_index_type;

            aabb3_t aabb;
            vec3_t origin;
            std::array<head_node_index_type, HEAD_NODE_INDEX_COUNT> head_node_indices;
            int32_t vis_leafs = 0;
            int32_t face_start_index = 0;
            int32_t face_count = 0;
        };

        struct bsp_texture_t
        {
            static const auto MIPMAP_OFFSET_COUNT = 4;

            uint32_t width;
            uint32_t height;
            uint32_t mipmap_offsets[MIPMAP_OFFSET_COUNT];
        };

        struct plane_t
        {
            typedef plane3_t plane_type;

            enum class type_e : uint32_t
            {
                x,
                y,
                z,
                any_x,
                any_y,
                any_z
            };

            plane_type plane;
            type_e type = type_e::x;
        };

        struct vertex_t
        {
            typedef vec3_t location_type;
            typedef vec3_t normal_type;
            typedef vec2_t texcoord_type;

            location_type location;
            normal_type normal;
            texcoord_type diffuse_texcoord;
            texcoord_type lightmap_texcoord;
        };

        typedef vertex_t vertex_type;
        typedef vertex_buffer_t<vertex_type> vertex_buffer_type;

        typedef uint32_t index_type;
        typedef index_buffer_t<index_type> index_buffer_type;

        struct trace_args_t
        {
            line3_t line;
        };

        struct trace_result_t
        {
            bool did_hit = false;
            bool is_all_solid = false;
            vec3_t location;
            plane3_t plane;
            float32_t ratio = 0.0f;
        };

        struct render_settings_t
        {
            float32_t lightmap_gamma = 1.0f;
        };

        struct render_stats_t
        {
            uint32_t face_count = 0;
            uint32_t leaf_count = 0;
            uint32_t leaf_index = 0;

            void reset()
            {
                face_count = 0;
                leaf_count = 0;
                leaf_index = 0;
            }
        };

        bsp_t(std::istream& istream);

        void render(const camera_t& camera);

        int32_t get_leaf_index_from_location(const vec3_t& location) const;

        const render_stats_t& get_render_stats() const { return render_stats; }

        render_settings_t render_settings;  //TODO: sort this out elsewhere

    private:
        std::vector<plane_t> planes;
        std::vector<edge_t> edges;
        std::vector<face_t> faces;
        std::vector<int32_t> surface_edges;
        std::vector<node_t> nodes;
        std::vector<leaf_t> leafs;
        std::vector<uint16_t> mark_surfaces;
        std::vector<texture_info_t> texture_infos;
        std::vector<boost::shared_ptr<texture_t>> face_lightmap_textures;
        std::vector<clip_node_t> clip_nodes;
        std::vector<model_t> models;
        std::vector<bsp_entity_t> entities;
        std::vector<size_t> brush_entity_indices;
        std::map<size_t, boost::dynamic_bitset<>> leaf_pvs_map;
        std::vector<size_t> face_start_indices;
        size_t vis_leaf_count = 0;
        std::vector<boost::shared_ptr<texture_t>> textures;
        render_stats_t render_stats;
        boost::shared_ptr<vertex_buffer_type> vertex_buffer;
        boost::shared_ptr<index_buffer_type> index_buffer;

        bsp_t(const bsp_t&) = delete;
        bsp_t& operator=(const bsp_t&) = delete;
    };
}
