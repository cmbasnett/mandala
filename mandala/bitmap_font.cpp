//std
#include <set>

//glm
#include <glm\ext.hpp>

//mandala
#include "bitmap_font.hpp"
#include "app.hpp"
#include "hash.hpp"
#include "texture.hpp"
#include "gpu_program.hpp"

#define BMF_MAGIC			("BMF")
#define BMF_MAGIC_LENGTH	(3)
#define BMF_VERSION			(3)

#define BITMAP_FONT_VERTICES_PER_CHARACTER	(4)
#define BITMAP_FONT_INDICES_PER_CHARACTER	(6)
#define BITMAP_FONT_CHARACTER_INDEX_STRIDE	(sizeof(uint16_t) * BITMAP_FONT_INDICES_PER_CHARACTER)
#define BITMAP_FONT_PAGE_COUNT_MAX			(32)

namespace mandala
{
	bitmap_font_t::bitmap_font_t(std::istream& istream)
	{
		//magic
		char magic[BMF_MAGIC_LENGTH + 1] = { '\0' };
		istream.read(magic, BMF_MAGIC_LENGTH);

		if (strcmp(BMF_MAGIC, magic) != 0)
		{
			throw std::exception();
		}

		//version
		uint8_t version = 0;
		istream.read((char*)&version, sizeof(version));

		if (version != BMF_VERSION)
		{
			throw std::exception();
		}

		//info
		istream.seekg(5, std::ios_base::cur);
		istream.read((char*)&size, sizeof(size));

		enum : uint8_t
		{
			flag_smooth = (1 << 7),
			flag_unicode = (1 << 6),
			flag_italic = (1 << 5),
			flag_bold = (1 << 4),
			flag_fixed_height = (1 << 3),
			flag_reserved0 = (1 << 2),
			flag_reserved1 = (1 << 1),
			flag_reserved2 = (1 << 0)
		};

		uint8_t flags;

		istream.read((char*)&flags, sizeof(flags));

		is_smooth = (flags & flag_smooth) == flag_smooth;
		is_unicode = (flags & flag_unicode) == flag_unicode;
		is_italic = (flags & flag_italic) == flag_italic;
		is_bold = (flags & flag_bold) == flag_bold;
		is_fixed_height = (flags & flag_fixed_height) == flag_fixed_height;

		istream.read((char*)&char_set, sizeof(char_set));
		istream.read((char*)&stretch_height, sizeof(stretch_height));
		istream.read((char*)&antialiasing, sizeof(antialiasing));
		istream.read((char*)&padding_top, sizeof(padding_top));
		istream.read((char*)&padding_right, sizeof(padding_right));
		istream.read((char*)&padding_bottom, sizeof(padding_bottom));
		istream.read((char*)&padding_left, sizeof(padding_left));
		istream.read((char*)&spacing_horizontal, sizeof(spacing_horizontal));
		istream.read((char*)&spacing_vertical, sizeof(spacing_vertical));
		istream.read((char*)&outline, sizeof(outline));

		//name
		std::string name;
		std::getline(istream, name, '\0');

		//common
		istream.seekg(5, std::ios_base::cur);
		istream.read((char*)&line_height, sizeof(line_height));
		istream.read((char*)&base, sizeof(base));
		istream.read((char*)&width, sizeof(width));
		istream.read((char*)&height, sizeof(height));
		istream.read((char*)&page_count, sizeof(page_count));
		istream.read((char*)&flags_1, sizeof(flags_1));
		istream.read((char*)&alpha_channel, sizeof(alpha_channel));
		istream.read((char*)&red_channel, sizeof(red_channel));
		istream.read((char*)&green_channel, sizeof(green_channel));
		istream.read((char*)&blue_channel, sizeof(blue_channel));
		istream.seekg(1, std::ios_base::cur);

		//textures
		uint32_t page_texture_names_length;
		istream.read((char*)&page_texture_names_length, sizeof(page_texture_names_length));

		while (page_texture_names_length > 0)
		{
			std::string page_texture_name;
			std::getline(istream, page_texture_name, '\0');

            page_texture_names_length -= static_cast<uint32_t>(page_texture_name.length() + 1);

			auto page_texture = app.resources.get<texture_t>(hash_t(page_texture_name));

			page_textures.push_back(page_texture);
		}

		//TODO: abstract and this value in some sort of opengl capabilities struct
		int32_t pages_max = 0;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &pages_max);

		if (page_textures.size() > static_cast<size_t>(pages_max))
		{
			throw std::exception();
		}

		//characters
		istream.seekg(1, std::ios_base::cur);
		uint32_t characters_length;
		istream.read((char*)&characters_length, sizeof(characters_length));

		auto character_count = characters_length / sizeof(character_t);

		for (size_t i = 0; i < character_count; ++i)
		{
			character_t character;

			istream.read((char*)&character.id, sizeof(character.id));
			istream.read((char*)&character.x, sizeof(character.x));
			istream.read((char*)&character.y, sizeof(character.y));
			istream.read((char*)&character.width, sizeof(character.width));
			istream.read((char*)&character.height, sizeof(character.height));
			istream.read((char*)&character.offset_x, sizeof(character.offset_x));
			istream.read((char*)&character.offset_y, sizeof(character.offset_y));
			istream.read((char*)&character.advance_x, sizeof(character.advance_x));
			istream.read((char*)&character.page, sizeof(character.page));
			istream.read((char*)&character.channel, sizeof(character.channel));

			characters.insert(std::make_pair(character.id, character));
			character_ids.push_back(character.id);
			character_indices.insert(std::make_pair(character.id, i));
		}

		//kerning pairs
		istream.get();

		if (istream.good())
		{
			uint32_t kerning_pairs_length;
			istream.read((char*)&kerning_pairs_length, sizeof(kerning_pairs_length));

			auto kerning_pair_count = kerning_pairs_length / sizeof(kerning_pair_t);
			kerning_pairs.resize(kerning_pair_count);

			for (auto& kerning_pair : kerning_pairs)
			{
				istream.read((char*)&kerning_pair.first, sizeof(kerning_pair.first));
				istream.read((char*)&kerning_pair.second, sizeof(kerning_pair.second));
				istream.read((char*)&kerning_pair.amount, sizeof(kerning_pair.amount));
			}
		}

		//vertex buffer
		auto vertex_count = characters.size() * BITMAP_FONT_VERTICES_PER_CHARACTER;

		std::vector<vertex_t> vertices;
		vertices.resize(vertex_count);

		auto j = 0;

		for (size_t i = 0; i < character_ids.size(); ++i)
		{
			auto character_id = character_ids[i];

			auto character = characters.at(character_id);

			//location 0
			vertices[j + 0].position.x = static_cast<float32_t>(character.offset_x);
			vertices[j + 0].position.y = -static_cast<float32_t>(character.offset_y + character.height - base);

			//location 1
			vertices[j + 1].position.x = static_cast<float32_t>(character.offset_x + character.width);
			vertices[j + 1].position.y = -static_cast<float32_t>(character.offset_y + character.height - base);

			//location 2
			vertices[j + 2].position.x = static_cast<float32_t>(character.offset_x + character.width);
			vertices[j + 2].position.y = -static_cast<float32_t>(character.offset_y - base);

			//location 3
			vertices[j + 3].position.x = static_cast<float32_t>(character.offset_x);
			vertices[j + 3].position.y = -static_cast<float32_t>(character.offset_y - base);

			//texcoord 0
			vertices[j + 0].texcoord.x = static_cast<float32_t>(character.x) / width;
			vertices[j + 0].texcoord.y = 1.0f - static_cast<float32_t>(character.y + character.height) / height;

			//texcoord 1
			vertices[j + 1].texcoord.x = static_cast<float32_t>(character.x + character.width) / width;
			vertices[j + 1].texcoord.y = 1.0f - static_cast<float32_t>(character.y + character.height) / height;

			//texcoord 2
			vertices[j + 2].texcoord.x = static_cast<float32_t>(character.x + character.width) / width;
			vertices[j + 2].texcoord.y = 1.0f - static_cast<float32_t>(character.y) / height;

			//texcoord 3
			vertices[j + 3].texcoord.x = static_cast<float32_t>(character.x) / width;
			vertices[j + 3].texcoord.y = 1.0f - static_cast<float32_t>(character.y) / height;

			j += 4;
		}

		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), static_cast<GLvoid*>(vertices.data()), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//index buffer
		std::vector<uint16_t> indices;
		indices.resize(characters.size() * BITMAP_FONT_INDICES_PER_CHARACTER);

		const uint16_t character_index_offsets[BITMAP_FONT_INDICES_PER_CHARACTER] = { 0, 1, 2, 0, 2, 3 };

		for (size_t i = 0; i < characters.size(); ++i)
		{
			for (size_t j = 0; j < BITMAP_FONT_INDICES_PER_CHARACTER; ++j)
			{
				indices[(i * BITMAP_FONT_INDICES_PER_CHARACTER) + j] = static_cast<uint16_t>(i) * BITMAP_FONT_VERTICES_PER_CHARACTER + character_index_offsets[j];
			}
		}

		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), static_cast<GLvoid*>(indices.data()), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	bitmap_font_t::~bitmap_font_t()
	{
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &index_buffer);
	}

	int16_t bitmap_font_t::get_kerning_amount(const wchar_t lhs, const wchar_t rhs) const
	{
		int16_t kerning_amount = 0;

        for (auto kerning_pair : kerning_pairs)
		{
			if (kerning_pair.first == lhs && kerning_pair.second == rhs)
			{
				kerning_amount += kerning_pair.amount;

				break;
			}
		}

		return kerning_amount;
	}

	void bitmap_font_t::render_string(const std::wstring& string, const vec4_t& color_top, const vec4_t& color_bottom, mat4_t world, const mat4_t& view_projection) const
	{
		auto gpu_program = app.resources.get<gpu_program_t>(hash_t("bitmap_font.gpu"));
		auto program = gpu_program->program;

		static const GLint position_location = glGetAttribLocation(program, "position");
		static const GLint texcoord_location = glGetAttribLocation(program, "texcoord");
		static const GLint world_location = glGetUniformLocation(program, "world");
		static const GLint view_projection_location = glGetUniformLocation(program, "view_projection");
		static const GLint line_height_location = glGetUniformLocation(program, "line_height");
		static const GLint base_location = glGetUniformLocation(program, "base");
		static const GLint diffuse_map_location = glGetUniformLocation(program, "diffuse_map");
		static const GLint color_top_location = glGetUniformLocation(program, "color_top");
		static const GLint color_bottom_location = glGetUniformLocation(program, "color_bottom");
		static const GLuint vertex_size = sizeof(bitmap_font_t::vertex_t);
		static const GLvoid* position_offset = reinterpret_cast<void*>(offsetof(bitmap_font_t::vertex_t, position));
		static const GLvoid* texcoord_offset = reinterpret_cast<void*>(offsetof(bitmap_font_t::vertex_t, texcoord));

		//program
		glUseProgram(program);

		//blend
		glEnable(GL_BLEND);

		//blend function
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//depth mask
		GLboolean depth_mask;
		glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_mask);
		glDepthMask(GL_FALSE);

		//vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

		//index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

		//position
		glEnableVertexAttribArray(position_location);
		glVertexAttribPointer(position_location, 2, GL_FLOAT, false, vertex_size, position_offset);

		//texcoord
		glEnableVertexAttribArray(texcoord_location);
		glVertexAttribPointer(texcoord_location, 2, GL_FLOAT, false, vertex_size, texcoord_offset);

		//view
		glUniformMatrix4fv(view_projection_location, 1, GL_FALSE, glm::value_ptr(view_projection));

		//line_height
		glUniform1f(line_height_location, line_height);

		//base
		glUniform1f(base_location, base);

		//diffuse map
		std::vector<uint8_t> pages;
		get_string_pages(pages, string);

		for (auto page : pages)
		{
			auto page_texture = page_textures[page];

			glActiveTexture(GL_TEXTURE0 + page);
            glBindTexture(GL_TEXTURE_2D, page_texture->id);
		}

		//color top
		glUniform4fv(color_top_location, 1, glm::value_ptr(color_top));

		//color bottom
		glUniform4fv(color_bottom_location, 1, glm::value_ptr(color_bottom));

		//TODO: this is kind of sloppy, find cleaner way to do this
		uint8_t character_page = -1;

		for (auto c = string.begin(); c != string.end(); ++c)
		{
			//model
			glUniformMatrix4fv(world_location, 1, GL_FALSE, glm::value_ptr(world));

			const auto character = characters.at(*c);

			if (character_page != character.page)
			{
				character_page = character.page;

				glUniform1i(diffuse_map_location, character_page);
			}

			auto x = static_cast<float32_t>(character.advance_x);

			auto character_index = character_indices.at(character.id);

			glDrawElements(GL_TRIANGLES,
				BITMAP_FONT_INDICES_PER_CHARACTER,
				GL_UNSIGNED_SHORT,
				reinterpret_cast<GLvoid*>(character_index * BITMAP_FONT_CHARACTER_INDEX_STRIDE));

			auto next = (c + 1);

			if (kerning_pairs.size() > 0 && next != string.end())
			{
				x += get_kerning_amount(*c, *next);
			}

			world *= glm::translate(x, 0.0f, 0.0f);
		}

		glDisableVertexAttribArray(position_location);
		glDisableVertexAttribArray(texcoord_location);

		for (auto page : pages)
		{
			glActiveTexture(GL_TEXTURE0 + page);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDepthMask(depth_mask);

		glDisable(GL_BLEND);

		glUseProgram(0);
	}

	void bitmap_font_t::get_string_pages(std::vector<uint8_t>& pages, const std::wstring& string) const
	{
		std::set<uint8_t> pages_set;

		for (const auto c : string)
		{
			auto characters_itr = characters.at(c);

			pages_set.insert(characters_itr.page);
		}

		std::copy(pages_set.begin(), pages_set.end(), std::back_inserter(pages));
	}

	int16_t bitmap_font_t::get_string_width(const std::wstring& string) const
	{
		int16_t width = 0;

		for (const auto c : string)
		{
			width += characters.at(c).advance_x;
		}

		return width;
	}
};