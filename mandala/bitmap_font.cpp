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
#include "gpu.hpp"
#include "bitmap_font_gpu_program.hpp"

#define BMF_MAGIC			("BMF")
#define BMF_MAGIC_LENGTH	(3)
#define BMF_VERSION			(3)

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
		istream.read(reinterpret_cast<char*>(&version), sizeof(version));

		if (version != BMF_VERSION)
		{
			throw std::exception();
		}

		//info
		istream.seekg(5, std::ios_base::cur);
        istream.read(reinterpret_cast<char*>(&size), sizeof(size));

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

        istream.read(reinterpret_cast<char*>(&flags), sizeof(flags));

		is_smooth = (flags & flag_smooth) == flag_smooth;
		is_unicode = (flags & flag_unicode) == flag_unicode;
		is_italic = (flags & flag_italic) == flag_italic;
		is_bold = (flags & flag_bold) == flag_bold;
		is_fixed_height = (flags & flag_fixed_height) == flag_fixed_height;

        istream.read(reinterpret_cast<char*>(&char_set), sizeof(char_set));
		istream.read(reinterpret_cast<char*>(&stretch_height), sizeof(stretch_height));
		istream.read(reinterpret_cast<char*>(&antialiasing), sizeof(antialiasing));
		istream.read(reinterpret_cast<char*>(&padding_top), sizeof(padding_top));
		istream.read(reinterpret_cast<char*>(&padding_right), sizeof(padding_right));
		istream.read(reinterpret_cast<char*>(&padding_bottom), sizeof(padding_bottom));
		istream.read(reinterpret_cast<char*>(&padding_left), sizeof(padding_left));
		istream.read(reinterpret_cast<char*>(&spacing_horizontal), sizeof(spacing_horizontal));
		istream.read(reinterpret_cast<char*>(&spacing_vertical), sizeof(spacing_vertical));
		istream.read(reinterpret_cast<char*>(&outline), sizeof(outline));

		//name
		std::string name;
		std::getline(istream, name, '\0');

		//common
		istream.seekg(5, std::ios_base::cur);
		istream.read(reinterpret_cast<char*>(&line_height), sizeof(line_height));
        istream.read(reinterpret_cast<char*>(&base), sizeof(base));
        istream.read(reinterpret_cast<char*>(&width), sizeof(width));
        istream.read(reinterpret_cast<char*>(&height), sizeof(height));
        istream.read(reinterpret_cast<char*>(&page_count), sizeof(page_count));
        istream.read(reinterpret_cast<char*>(&flags_1), sizeof(flags_1));
        istream.read(reinterpret_cast<char*>(&alpha_channel), sizeof(alpha_channel));
        istream.read(reinterpret_cast<char*>(&red_channel), sizeof(red_channel));
        istream.read(reinterpret_cast<char*>(&green_channel), sizeof(green_channel));
        istream.read(reinterpret_cast<char*>(&blue_channel), sizeof(blue_channel));
		istream.seekg(1, std::ios_base::cur);

		//textures
		uint32_t page_texture_names_length;
        istream.read(reinterpret_cast<char*>(&page_texture_names_length), sizeof(page_texture_names_length));

		while (page_texture_names_length > 0)
		{
			std::string page_texture_name;
			std::getline(istream, page_texture_name, '\0');

            page_texture_names_length -= static_cast<uint32_t>(page_texture_name.length() + 1);

			auto page_texture = app.resources.get<texture_t>(hash_t(page_texture_name));

			page_textures.push_back(page_texture);
		}

		//TODO: abstract and this value in some sort of opengl capabilities struct
		int32_t pages_max = 32;
		//glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &pages_max);

		if (page_textures.size() > static_cast<size_t>(pages_max))
		{
			throw std::exception();
		}

		//characters
		istream.seekg(1, std::ios_base::cur);
		uint32_t characters_length;
		istream.read(reinterpret_cast<char*>(&characters_length), sizeof(characters_length));

		auto character_count = characters_length / sizeof(character_t);

		for (size_t i = 0; i < character_count; ++i)
		{
			character_t character;

			istream.read(reinterpret_cast<char*>(&character.id), sizeof(character.id));
			istream.read(reinterpret_cast<char*>(&character.x), sizeof(character.x));
			istream.read(reinterpret_cast<char*>(&character.y), sizeof(character.y));
			istream.read(reinterpret_cast<char*>(&character.width), sizeof(character.width));
			istream.read(reinterpret_cast<char*>(&character.height), sizeof(character.height));
			istream.read(reinterpret_cast<char*>(&character.offset_x), sizeof(character.offset_x));
			istream.read(reinterpret_cast<char*>(&character.offset_y), sizeof(character.offset_y));
			istream.read(reinterpret_cast<char*>(&character.advance_x), sizeof(character.advance_x));
			istream.read(reinterpret_cast<char*>(&character.texture_index), sizeof(character.texture_index));
			istream.read(reinterpret_cast<char*>(&character.channel), sizeof(character.channel));

			characters.insert(std::make_pair(character.id, character));
			character_ids.push_back(character.id);
			character_indices.insert(std::make_pair(character.id, i));
		}

		//kerning pairs
		istream.get();

		if (istream.good())
		{
			uint32_t kerning_pairs_length;
			istream.read(reinterpret_cast<char*>(&kerning_pairs_length), sizeof(kerning_pairs_length));

			auto kerning_pair_count = kerning_pairs_length / sizeof(kerning_pair_t);
			kerning_pairs.resize(kerning_pair_count);

			for (auto& kerning_pair : kerning_pairs)
			{
                istream.read(reinterpret_cast<char*>(&kerning_pair.first), sizeof(kerning_pair.first));
                istream.read(reinterpret_cast<char*>(&kerning_pair.second), sizeof(kerning_pair.second));
                istream.read(reinterpret_cast<char*>(&kerning_pair.amount), sizeof(kerning_pair.amount));
			}
		}

		//vertex buffer
		auto vertex_count = characters.size() * vertices_per_character;

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

        vertex_buffer = std::make_shared<vertex_buffer_type>();
        vertex_buffer->data(vertices, gpu_t::buffer_usage_e::static_draw);

		//index buffer
		std::vector<index_type> indices;
		indices.resize(characters.size() * indices_per_character);

        const index_type character_index_offsets[indices_per_character] = { 0, 1, 2, 0, 2, 3 };

		for (size_t i = 0; i < characters.size(); ++i)
		{
			for (size_t j = 0; j < indices_per_character; ++j)
			{
                indices[(i * indices_per_character) + j] = static_cast<index_type>(i) * vertices_per_character + character_index_offsets[j];
			}
		}

        index_buffer = std::make_shared<index_buffer_type>();
        index_buffer->data(indices, gpu_t::buffer_usage_e::static_draw);
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

	void bitmap_font_t::render_string(const std::wstring& string, mat4_t world_matrix, mat4_t view_projection_matrix, const vec4_t& base_color, std::stack<vec4_t>& color_stack, const std::vector<std::pair<size_t, vec4_t>>& color_pushes, const std::vector<size_t>& color_pop_indices) const
	{
		//buffers
		gpu.buffers.push(gpu_t::buffer_target_e::array, vertex_buffer);
		gpu.buffers.push(gpu_t::buffer_target_e::element_array, index_buffer);

		//program
		gpu.programs.push(bitmap_font_gpu_program);

		//blend
		auto gpu_blend_state = gpu.blend.top();
		gpu_blend_state.is_enabled = true;
		gpu_blend_state.src_factor = gpu_t::blend_factor_e::src_alpha;
		gpu_blend_state.dst_factor = gpu_t::blend_factor_e::one_minus_src_alpha;

		gpu.blend.push(gpu_blend_state);

		//depth mask
		auto gpu_depth_state = gpu.depth.top();
		gpu_depth_state.should_write_mask = false;

		gpu.depth.push(gpu_depth_state);

		//view projection matrix
		bitmap_font_gpu_program->view_projection_matrix(view_projection_matrix);
		bitmap_font_gpu_program->font_line_height(line_height);
		bitmap_font_gpu_program->font_base(base);

		auto start_color = color_stack.empty() ? base_color : color_stack.top();

		bitmap_font_gpu_program->font_color_top(start_color);
		bitmap_font_gpu_program->font_color_bottom(start_color);

		//textures
		std::vector<uint8_t> page_indices;
		get_string_pages(page_indices, string);

		for (auto page_index : page_indices)
		{
            gpu.textures.bind(page_index, page_textures[page_index]);
		}

		//TODO: this is kind of sloppy, find cleaner way to do this
		uint8_t character_texture_index = -1;
		bool did_color_stack_change = false;
		auto color_pushes_itr = color_pushes.begin();
		auto color_pop_indices_itr = color_pop_indices.begin();

		for (auto c = string.begin(); c != string.end(); ++c)
		{
			bitmap_font_gpu_program->world_matrix(world_matrix);

			const auto character = characters.at(*c);

			if (character_texture_index != character.texture_index)
			{
				character_texture_index = character.texture_index;

				bitmap_font_gpu_program->font_diffuse_texture_index(character_texture_index);
			}

			auto x = static_cast<float32_t>(character.advance_x);

            auto character_index = character_indices.at(character.id);

            static const auto character_index_stride = sizeof(index_type) * indices_per_character;

			auto string_index = std::distance(string.begin(), c);

			//color push
			while (color_pushes_itr != color_pushes.end() && color_pushes_itr->first == string_index)
			{
				color_stack.push(color_pushes_itr->second);

				did_color_stack_change = true;

				++color_pushes_itr;
			}

			//color pop
			while (color_pop_indices_itr != color_pop_indices.end() && *color_pop_indices_itr == string_index)
			{
				if (!color_stack.empty())
				{
					color_stack.pop();

					did_color_stack_change = true;
				}

				++color_pop_indices_itr;
			}

			if (did_color_stack_change)
			{
				auto color = color_stack.empty() ? base_color : color_stack.top();

				bitmap_font_gpu_program->font_color_top(color);
				bitmap_font_gpu_program->font_color_bottom(color);
			}

			gpu.draw_elements(gpu_t::primitive_type_e::triangles, indices_per_character, gpu_t::index_type_e::unsigned_int, character_index * character_index_stride);

			auto next = (c + 1);

			if (kerning_pairs.size() > 0 && next != string.end())
			{
				x += get_kerning_amount(*c, *next);
			}

			world_matrix *= glm::translate(x, 0.0f, 0.0f);
		}

		//color push
		while (color_pushes_itr != color_pushes.end() && color_pushes_itr->first == string.length())
		{
			color_stack.push(color_pushes_itr->second);

			++color_pushes_itr;
		}

		//color pop
		while (color_pop_indices_itr != color_pop_indices.end() && *color_pop_indices_itr == string.length())
		{
			if (!color_stack.empty())
			{
				color_stack.pop();
			}

			++color_pop_indices_itr;
		}

		for (auto page_index : page_indices)
		{
			gpu.textures.unbind(page_index);
		}

		//depth
		gpu.depth.pop();

		//blend
		gpu.blend.pop();

		//program
		gpu.programs.pop();

		//buffers
		gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
		gpu.buffers.pop(gpu_t::buffer_target_e::array);
	}

	void bitmap_font_t::get_string_pages(std::vector<uint8_t>& pages, const std::wstring& string) const
	{
		std::set<uint8_t> pages_set;

		for (const auto c : string)
		{
			auto characters_itr = characters.at(c);

			pages_set.insert(characters_itr.texture_index);
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