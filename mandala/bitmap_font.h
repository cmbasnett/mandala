#pragma once

//mandala
#include "mandala.h"
#include "resource.h"

//std
#include <vector>
#include <memory>
#include <map>

namespace mandala
{
	struct texture_t;

	struct bitmap_font_t : resource_t
	{
		struct character_t
		{
			uint32_t id = 0;
			uint16_t x = 0;
			uint16_t y = 0;
			uint16_t width = 0;
			uint16_t height = 0;
			int16_t offset_x = 0;
			int16_t offset_y = 0;
			int16_t advance_x = 0;
			uint8_t page = 0;
			uint8_t channel = 0;
		};

		struct kerning_pair_t
		{
			uint32_t first = 0;
			uint32_t second = 0;
			int16_t amount = 0;
		};

		struct vertex_t
		{
			vec2_t position;
			vec2_t texcoord;
		};
		
		bitmap_font_t(std::istream& ifstream);
		virtual ~bitmap_font_t();

		void render_string(const std::wstring& string, const vec4_t& color_top, const vec4_t& color_bottom, mat4_t world, const mat4_t& view_projection) const;
		int16_t get_kerning_amount(wchar_t lhs, wchar_t rhs) const;
		void get_string_pages(std::vector<uint8_t>& pages, const std::wstring& string) const;
		int16_t get_string_width(const std::wstring& string) const;
		
		int16_t size = 0;
		bool is_smooth = false;
		bool is_unicode = false;
		bool is_italic = false;
		bool is_bold = false;
		bool is_fixed_height = false;
		uint8_t char_set = 0;
		uint16_t stretch_height = 0;
		uint8_t antialiasing = 0;
		uint8_t padding_top = 0;
		uint8_t padding_right = 0;
		uint8_t padding_bottom = 0;
		uint8_t padding_left = 0;
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
		std::vector<uint32_t> character_ids;
		std::map<uint32_t, size_t> character_indices;
		std::map<uint32_t, character_t> characters;
		std::vector<kerning_pair_t> kerning_pairs;
		uint32_t vertex_buffer = 0;
		uint32_t index_buffer = 0;
		std::vector<std::shared_ptr<texture_t>> page_textures;

	private:
		bitmap_font_t(const bitmap_font_t&) = delete;
		bitmap_font_t& operator=(const bitmap_font_t&) = delete;
	};
};
