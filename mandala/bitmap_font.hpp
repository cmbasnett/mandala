#pragma once

//mandala
#include "resource.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "index_type.hpp"
#include "padding.hpp"

//std
#include <vector>
#include <memory>
#include <map>
#include <set>
#include <vector>

namespace mandala
{
	struct texture_t;

	struct bitmap_font_t : resource_t
	{
        typedef std::wstring string_type;
        typedef string_type::value_type char_type;
		typedef uint32_t character_id_type;

		struct character_t
		{
			character_id_type id = 0;
            rectangle_u16_t rectangle;
            vec2_i16_t offset;
			int16_t advance_x = 0;
			uint8_t texture_index = 0;
			uint8_t channel = 0;
		};

		struct kerning_pair_t
		{
            character_id_type first = 0;
            character_id_type second = 0;
			int16_t amount = 0;
		};

		struct vertex_t
		{
            typedef vec2_t position_type;
            typedef vec2_t texcoord_type;

            position_type position;
            texcoord_type texcoord;
        };

        static const auto characters_max = 0xFFFF;
        static const auto vertices_per_character = 4;
        static const auto indices_per_character = 6;
        static const auto indices_max = characters_max * indices_per_character;

        typedef vertex_t vertex_type;
        typedef index_type<indices_max>::type index_type;
        typedef vertex_buffer_t<vertex_type> vertex_buffer_type;
        typedef index_buffer_t<index_type> index_buffer_type;

		bitmap_font_t(std::istream& istream);

        void render_string(const string_type& string, mat4_t world_matrix, mat4_t view_projection_matrix, const rgba_type& base_color, std::stack<rgba_type>& color_stack, const std::vector<std::pair<size_t, rgba_type>>& color_pushes, const std::vector<size_t>& color_pop_indices) const;
        int16_t get_kerning_amount(char_type lhs, char_type rhs) const;
        void get_string_pages(std::vector<uint8_t>& pages, const string_type& string) const;
        int16_t get_string_width(const string_type& string) const;
		
		int16_t size = 0;
		bool is_smooth = false;
		bool is_unicode = false;
		bool is_italic = false;
		bool is_bold = false;
		bool is_fixed_height = false;
		uint8_t char_set = 0;
		uint16_t stretch_height = 0;
		uint8_t antialiasing = 0;
        padding_u8_t padding;
		uint8_t spacing_horizontal = 0;
		uint8_t spacing_vertical = 0;
		uint8_t outline = 0;
		uint16_t line_height = 0;
		uint16_t base = 0;
		uint16_t width = 0;
		uint16_t height = 0;
		uint16_t page_count = 0;
		uint8_t flags_1 = 0;
		uint8_t alpha_channel = 0;
		uint8_t red_channel = 0;
		uint8_t green_channel = 0;
		uint8_t blue_channel = 0;
		std::vector<character_id_type> character_ids;
		std::map<character_id_type, size_t> character_indices;
		std::map<character_id_type, character_t> characters;
        std::vector<kerning_pair_t> kerning_pairs;
        std::shared_ptr<vertex_buffer_type> vertex_buffer;
        std::shared_ptr<index_buffer_type> index_buffer;
		std::vector<std::shared_ptr<texture_t>> page_textures;

	private:
		bitmap_font_t(const bitmap_font_t&) = delete;
		bitmap_font_t& operator=(const bitmap_font_t&) = delete;
	};
}
