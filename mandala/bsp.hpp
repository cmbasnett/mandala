#pragma once

//std
#include <memory>
#include <vector>
#include <map>

//mandala
#include "resource.hpp"
#include "aabb.hpp"
#include "texture.hpp"
#include "plane.hpp"
#include "line.hpp"

//boost
#include <boost\dynamic_bitset.hpp>

namespace mandala
{
	struct camera_t;

	struct bsp_t : public resource_t
	{
		enum class content_type_e : int32_t
		{
			empty = -1,
			solid = -2,
			water = -3,
			slime = -4,
			lava = -5,
			sky = -6,
			origin = -7,
			clip = -8,
			current_0 = -9,
			current_90 = -10,
			current_180 = -11,
			current_270 = -12,
			current_up = -13,
			current_down = -14,
			translucent = -15
		};

		enum class render_mode_e : uint8_t
		{
			normal,
			color,
			texture,
			glow,
			solid,
			additive
		};

		struct node_t
		{
            static const auto child_count = 2;

			uint32_t plane_index = 0;
            std::array<int16_t, child_count> children;
			aabb3_i16_t aabb;
			uint16_t face_start_index = 0;
			uint16_t face_count = 0;
		};

		struct face_t
		{
            static const auto lighting_style_count = 4;

			uint16_t plane_index = 0;
			uint16_t plane_side = 0;
			uint32_t surface_edge_start_index = 0;
			uint16_t surface_edge_count = 0;
			uint16_t texture_info_index = 0;
            std::array<uint8_t, lighting_style_count> lighting_styles;
			uint32_t lightmap_offset = 0;
		};

		struct leaf_t
		{
            static const auto ambient_sound_level_count = 4;

			content_type_e content_type = content_type_e::empty;
			int32_t visibility_offset = 0;
			aabb3_i16_t aabb;
			uint16_t mark_surface_start_index = 0;
			uint16_t mark_surface_count = 0;
            std::array<uint8_t, ambient_sound_level_count> ambient_sound_levels;
		};

		struct edge_t
		{
            static const auto vertex_index_count = 2;

            std::array<uint16_t, vertex_index_count> vertex_indices;
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
            static const auto child_count = 2;

			int32_t plane_index = 0;
            std::array<int16_t, child_count> children;
		};

		struct model_t
		{
            static const auto head_node_count = 4;

			aabb3_t aabb;
			vec3_t origin;
            std::array<int32_t, head_node_count> head_nodes;
			int32_t vis_leafs = 0;
			int32_t face_start_index = 0;
			int32_t face_count = 0;
		};

		struct bsp_texture_t
		{
            static const auto mipmap_offset_count = 4;

			uint32_t width;
			uint32_t height;
            uint32_t mipmap_offsets[mipmap_offset_count];
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
			typedef vec3_t position_type;
			typedef vec3_t normal_type;
            typedef vec2_t texcoord_type;

            position_type position;
            normal_type normal;
            texcoord_type diffuse_texcoord;
            texcoord_type lightmap_texcoord;
		};

		struct trace_args_t
		{
			line3_t line;
		};

		struct trace_result_t
		{
			bool did_hit = false;
			bool is_all_solid = false;
			vec3_t position;
			plane3_t plane;
			float32_t ratio = 0.0f;
		};

		bsp_t(std::istream& istream);

		std::vector<plane_t> planes;
		std::vector<vec3_t> vertex_positions;
		std::vector<edge_t> edges;
		std::vector<face_t> faces;
		std::vector<int32_t> surface_edges;
		std::vector<node_t> nodes;
		std::vector<leaf_t> leafs;
		std::vector<uint16_t> mark_surfaces;
		std::vector<texture_info_t> texture_infos;
		std::vector<std::shared_ptr<texture_t>> face_lightmap_textures;
		std::vector<clip_node_t> clip_nodes;
		std::vector<model_t> models;
		std::map<size_t, boost::dynamic_bitset<>> leaf_pvs_map;
		std::vector<size_t> face_start_indices;
		size_t vis_leaf_count = 0;
		std::vector<std::shared_ptr<texture_t>> textures;
		std::vector<vertex_t> vertices;
		std::vector<uint32_t> indices;

		int32_t get_leaf_index_from_position(const vec3_t& position) const;
		trace_result_t trace(const trace_args_t& args) const;
		void render(const camera_t& camera) const;

	private:
		bsp_t(const bsp_t&) = delete;
		bsp_t& operator=(const bsp_t&) = delete;

		uint32_t vertex_buffer = 0;
		uint32_t index_buffer = 0;
	};
};
