#pragma once

//naga
#include "resource.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "index_type.hpp"
#include "padding.hpp"
#include "bitmap_font_gpu_program.hpp"

//std
#include <vector>
#include <map>
#include <set>
#include <vector>

//boost
#include <boost\shared_ptr.hpp>

namespace naga
{
    struct Texture;

    struct BitmapFont : Resource
	{
        typedef std::wstring StringType;
		typedef StringType::value_type CharType;
        typedef u32 CharacterIdType;

        struct Character
        {
			CharacterIdType id = 0;
            details::Rectangle<u16> rectangle;
            tvec2<i16> offset;
            i16 advance_x = 0;
            u8 texture_index = 0;
            u8 channel = 0;
        };

		struct KerningPair : std::pair<CharacterIdType, CharacterIdType>
        {
            i16 amount = 0;
        };

        static const auto CHARACTERS_MAX = 0xFFFF;
        static const auto VERTICES_PER_CHARACTER = 4;
        static const auto INDICES_PER_CHARACTER = 6;
        static const auto INDICES_MAX = CHARACTERS_MAX * INDICES_PER_CHARACTER;
		static const auto PAGES_MAX = 0xFF;

        typedef bitmap_font_gpu_program::VertexType VertexType;
		typedef IndexType<PAGES_MAX>::Type PageIndexType;
        typedef IndexType<INDICES_MAX>::Type IndexType;
		typedef VertexBuffer<VertexType> VertexBufferType;
        typedef IndexBuffer<IndexType> IndexBufferType;

        BitmapFont(std::istream& istream);

        void render_string(const StringType& string, mat4 world_matrix, mat4 view_projection_matrix, const vec4& base_color, std::stack<vec4>& color_stack, const std::vector<std::pair<size_t, vec4>>& color_pushes, const std::vector<size_t>& color_pop_indices) const;
		void get_string_pages(std::vector<PageIndexType>& pages, const StringType& string) const;
		i16 get_string_width(const StringType& string) const;

        i16 get_size() const { return size; }
        bool get_is_smooth() const { return is_smooth; }
        bool get_is_unicode() const { return is_unicode; }
        bool get_is_bold() const { return is_bold; }
        bool get_is_fixed_height() const { return is_fixed_height; }
        u8 get_char_set() const { return char_set; }
        u16 get_stretch_height() const { return stretch_height; }
        u8 get_antialiasing() const { return antialiasing; }
        const details::Padding<u8>& get_padding() const { return padding; }
        u8 get_spacing_horizontal() const { return spacing_horizontal; }
        u8 get_spacing_vertical() const { return spacing_vertical; }
        u8 get_outline() const { return outline; }
        u16 get_line_height() const { return line_height; }
        u16 get_base() const { return base; }
        u16 get_width() const { return width; }
        u16 get_height() const { return height; }
        u16 get_page_count() const { return page_count; }
        u8 get_flags_1() const { return flags_1; }
        u8 get_alpha_channel() const { return alpha_channel; }
        u8 get_red_channel() const { return red_channel; }
        u8 get_green_channel() const { return green_channel; }
        u8 get_blue_channel() const { return blue_channel; }
        const std::map<CharacterIdType, Character>& get_characters() const { return characters; }
        const std::vector<boost::shared_ptr<Texture>> get_page_textures() const { return page_textures; }

    private:
        i16 size = 0;
        bool is_smooth = false;
        bool is_unicode = false;
        bool is_italic = false;
        bool is_bold = false;
        bool is_fixed_height = false;
        u8 char_set = 0;    //TODO: represent with an enum?
        u16 stretch_height = 0;
        u8 antialiasing = 0;
        details::Padding<u8> padding;
        u8 spacing_horizontal = 0;
        u8 spacing_vertical = 0;
        u8 outline = 0;
        u16 line_height = 0;
        u16 base = 0;
        u16 width = 0;
        u16 height = 0;
        u16 page_count = 0;
        u8 flags_1 = 0;
        u8 alpha_channel = 0;
        u8 red_channel = 0;
        u8 green_channel = 0;
        u8 blue_channel = 0;
		std::vector<CharacterIdType> character_ids;
		std::map<CharacterIdType, size_t> character_indices;
        std::map<CharacterIdType, Character> characters;
        std::vector<KerningPair> kerning_pairs;
        boost::shared_ptr<VertexBufferType> vertex_buffer;
        boost::shared_ptr<IndexBufferType> index_buffer;
        std::vector<boost::shared_ptr<Texture>> page_textures;

		i16 get_kerning_amount(CharType lhs, CharType rhs) const;

		BitmapFont(const BitmapFont&) = delete;
		BitmapFont& operator=(const BitmapFont&) = delete;
    };
}
