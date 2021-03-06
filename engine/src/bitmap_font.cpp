﻿//std
#include <set>

//glm
#include <glm\ext.hpp>

//naga
#include "bitmap_font.hpp"
#include "resource_manager.hpp"
#include "texture.hpp"
#include "gpu_program.hpp"
#include "gpu_program_mgr.hpp"
#include "gpu_buffer_mgr.hpp"
#include "gpu.hpp"
#include "bitmap_font_gpu_program.hpp"
#include "io.hpp"

#define BMF_MAGIC_LENGTH    (3)
#define BMF_MAGIC           (std::array<char, BMF_MAGIC_LENGTH> { { 'B', 'M', 'F' } })
#define BMF_VERSION         (3)

namespace naga
{
	BitmapFont::BitmapFont(std::istream& istream)
    {
        //magic
        std::array<char, BMF_MAGIC_LENGTH> magic;
        read(istream, magic);

        if (magic != BMF_MAGIC)
        {
            throw std::exception();
        }

        //version
        u8 version = 0;
        read(istream, version);

        if (version != BMF_VERSION)
        {
            throw std::exception();
        }

        //info
        istream.seekg(5, std::ios_base::cur);
        read(istream, size);

        enum : u8
        {
            FLAG_SMOOTH = (1 << 7),
            FLAG_UNICODE = (1 << 6),
            FLAG_ITALIC = (1 << 5),
            FLAG_BOLD = (1 << 4),
            FLAG_FIXED_HEIGHT = (1 << 3),
            FLAG_RESERVED0 = (1 << 2),
            FLAG_RESERVED1 = (1 << 1),
            FLAG_RESERVED2 = (1 << 0)
        };

        u8 flags;

        read(istream, flags);

		is_smooth = (flags & FLAG_SMOOTH) == FLAG_SMOOTH;
		is_unicode = (flags & FLAG_UNICODE) == FLAG_UNICODE;
		is_italic = (flags & FLAG_ITALIC) == FLAG_ITALIC;
		is_bold = (flags & FLAG_BOLD) == FLAG_BOLD;
		is_fixed_height = (flags & FLAG_FIXED_HEIGHT) == FLAG_FIXED_HEIGHT;

        read(istream, char_set);
        read(istream, stretch_height);
        read(istream, antialiasing);
        read(istream, padding.top);
        read(istream, padding.right);
        read(istream, padding.bottom);
        read(istream, padding.left);
        read(istream, spacing_horizontal);
        read(istream, spacing_vertical);
        read(istream, outline);

        //name
        std::string name;
        std::getline(istream, name, '\0');

        //common
        istream.seekg(5, std::ios_base::cur);
        read(istream, line_height);
        read(istream, base);
        read(istream, width);
        read(istream, height);
        read(istream, page_count);
        read(istream, flags_1);
        read(istream, alpha_channel);
        read(istream, red_channel);
        read(istream, green_channel);
        read(istream, blue_channel);
        istream.seekg(1, std::ios_base::cur);

        //textures
        u32 page_texture_names_length;
        read(istream, page_texture_names_length);

        while (page_texture_names_length > 0)
        {
            std::string page_texture_name;
            std::getline(istream, page_texture_name, '\0');

            page_texture_names_length -= static_cast<u32>(page_texture_name.length() + 1);

            auto page_texture = resources.get<Texture>(page_texture_name);

            page_textures.push_back(page_texture);
        }

        //TODO: abstract and this value in some sort of opengl capabilities struct
        i32 pages_max = 32;
        //glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &pages_max);

        if (page_textures.size() > static_cast<size_t>(pages_max))
        {
            throw std::exception("too many pages");
        }

        //characters
        istream.seekg(1, std::ios_base::cur);
        u32 characters_length;
        read(istream, characters_length);

        auto character_count = characters_length / sizeof(Character);

        for (size_t i = 0; i < character_count; ++i)
        {
            Character character;

            read(istream, character.id);
            read(istream, character.rectangle.x);
            read(istream, character.rectangle.y);
            read(istream, character.rectangle.width);
            read(istream, character.rectangle.height);
            read(istream, character.offset.x);
            read(istream, character.offset.y);
            read(istream, character.advance_x);
            read(istream, character.texture_index);
            read(istream, character.channel);

            characters.insert(std::make_pair(character.id, character));
            character_ids.push_back(character.id);
            character_indices.insert(std::make_pair(character.id, i));
        }

        //kerning pairs
        istream.get();

        if (istream.good())
        {
            u32 kerning_pairs_length;
            read(istream, kerning_pairs_length);

            auto kerning_pair_count = kerning_pairs_length / sizeof(KerningPair);
            kerning_pairs.resize(kerning_pair_count);

            for (auto& kerning_pair : kerning_pairs)
            {
                read(istream, kerning_pair.first);
                read(istream, kerning_pair.second);
                read(istream, kerning_pair.amount);
            }
        }

        //vertex buffer
        auto vertex_count = characters.size() * VERTICES_PER_CHARACTER;

        std::vector<VertexType> vertices;
        vertices.resize(vertex_count);

        auto j = 0;

        for (size_t i = 0; i < character_ids.size(); ++i)
        {
            auto character_id = character_ids[i];
            auto character = characters.at(character_id);

            //location 0
            vertices[j + 0].location.x = static_cast<f32>(character.offset.x);
            vertices[j + 0].location.y = -static_cast<f32>(character.offset.y + character.rectangle.height - base);

            //location 1
            vertices[j + 1].location.x = static_cast<f32>(character.offset.x + character.rectangle.width);
            vertices[j + 1].location.y = -static_cast<f32>(character.offset.y + character.rectangle.height - base);

            //location 2
            vertices[j + 2].location.x = static_cast<f32>(character.offset.x + character.rectangle.width);
            vertices[j + 2].location.y = -static_cast<f32>(character.offset.y - base);

            //location 3
            vertices[j + 3].location.x = static_cast<f32>(character.offset.x);
            vertices[j + 3].location.y = -static_cast<f32>(character.offset.y - base);

            //texcoord 0
            vertices[j + 0].texcoord.x = static_cast<f32>(character.rectangle.x) / width;
            vertices[j + 0].texcoord.y = 1.0f - static_cast<f32>(character.rectangle.y + character.rectangle.height) / height;

            //texcoord 1
            vertices[j + 1].texcoord.x = static_cast<f32>(character.rectangle.x + character.rectangle.width) / width;
            vertices[j + 1].texcoord.y = 1.0f - static_cast<f32>(character.rectangle.y + character.rectangle.height) / height;

            //texcoord 2
            vertices[j + 2].texcoord.x = static_cast<f32>(character.rectangle.x + character.rectangle.width) / width;
            vertices[j + 2].texcoord.y = 1.0f - static_cast<f32>(character.rectangle.y) / height;

            //texcoord 3
            vertices[j + 3].texcoord.x = static_cast<f32>(character.rectangle.x) / width;
            vertices[j + 3].texcoord.y = 1.0f - static_cast<f32>(character.rectangle.y) / height;

            j += 4;
        }

        vertex_buffer = gpu_buffers.make<VertexBufferType>().lock();
        vertex_buffer->data(vertices, Gpu::BufferUsage::STATIC_DRAW);

        //index buffer
        std::vector<IndexType> indices;
        indices.resize(characters.size() * INDICES_PER_CHARACTER);

		const IndexType character_index_offsets[INDICES_PER_CHARACTER] = { 0, 1, 2, 0, 2, 3 };

        for (size_t i = 0; i < characters.size(); ++i)
        {
            for (size_t j = 0; j < INDICES_PER_CHARACTER; ++j)
            {
				indices[(i * INDICES_PER_CHARACTER) + j] = static_cast<IndexType>(i)* VERTICES_PER_CHARACTER + character_index_offsets[j];
            }
        }

        index_buffer = gpu_buffers.make<IndexBufferType>().lock();
        index_buffer->data(indices, Gpu::BufferUsage::STATIC_DRAW);
    }

	i16 BitmapFont::get_kerning_amount(CharType lhs, CharType rhs) const
    {
        i16 kerning_amount = 0;

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

    void BitmapFont::render_string(const StringType& string, mat4 world_matrix, mat4 view_projection_matrix, const vec4& base_color, std::stack<vec4>& color_stack, const std::vector<std::pair<size_t, vec4>>& color_pushes, const std::vector<size_t>& color_pop_indices) const
    {
        static const auto CHARACTER_INDEX_STRIDE = sizeof(IndexType) * INDICES_PER_CHARACTER;

        //buffers
        gpu.buffers.push(Gpu::BufferTarget::ARRAY, vertex_buffer);
        gpu.buffers.push(Gpu::BufferTarget::ELEMENT_ARRAY, index_buffer);

        const auto gpu_program = gpu_programs.get<bitmap_font_gpu_program>();

        //program
        gpu.programs.push(gpu_program);

        //states
        auto blend_state = gpu.blend.get_state();
        blend_state.is_enabled = true;
        blend_state.src_factor = Gpu::BlendFactor::SRC_ALPHA;
        blend_state.dst_factor = Gpu::BlendFactor::ONE_MINUS_SRC_ALPHA;

        gpu.blend.push_state(blend_state);

        auto depth_state = gpu.depth.get_state();
        depth_state.should_write_mask = false;

        gpu.depth.push_state(depth_state);

        //view projection matrix
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);
        gpu.set_uniform("line_height", static_cast<f32>(line_height));
        gpu.set_uniform("base", static_cast<f32>(base));

        auto start_color = color_stack.empty() ? base_color : color_stack.top();

        gpu.set_uniform("color_top", start_color);
        gpu.set_uniform("color_bottom", start_color);

        //textures
        std::vector<u8> page_indices;
        get_string_pages(page_indices, string);

        for (auto page_index : page_indices)
        {
            gpu.textures.bind(page_index, page_textures[page_index]);
        }

        //TODO: this is kind of sloppy, find cleaner way to do this
        u8 character_texture_index = -1;
        bool did_color_stack_change = false;
        auto color_pushes_itr = color_pushes.begin();
        auto color_pop_indices_itr = color_pop_indices.begin();

        for (auto c = string.begin(); c != string.end(); ++c)
        {
            gpu.set_uniform("world_matrix", world_matrix);

            const auto character = characters.at(*c);

            if (character_texture_index != character.texture_index)
            {
                character_texture_index = character.texture_index;

                gpu.set_uniform("diffuse_texture", character_texture_index);
            }

            auto x = static_cast<f32>(character.advance_x);
            const auto character_index = character_indices.at(character.id);
            const auto string_index = std::distance(string.begin(), c);

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
                const auto& color = color_stack.empty() ? base_color : color_stack.top();

                gpu.set_uniform("color_top", color);
                gpu.set_uniform("color_bottom", color);
            }

            gpu.draw_elements(Gpu::PrimitiveType::TRIANGLES, INDICES_PER_CHARACTER, IndexBufferType::DATA_TYPE, character_index * CHARACTER_INDEX_STRIDE);

            const auto next = (c + 1);

            if (kerning_pairs.size() > 0 && next != string.end())
            {
                x += get_kerning_amount(*c, *next);
            }

            world_matrix *= glm::translate(vec3(x, 0.0f, 0.0f));
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


        //states
        gpu.depth.pop_state();
        gpu.blend.pop_state();

        //program
        gpu.programs.pop();

        //buffers
        gpu.buffers.pop(Gpu::BufferTarget::ELEMENT_ARRAY);
        gpu.buffers.pop(Gpu::BufferTarget::ARRAY);
    }

	void BitmapFont::get_string_pages(std::vector<PageIndexType>& pages, const StringType& string) const
    {
		std::set<PageIndexType> pages_set;

        for (const auto c : string)
        {
            auto characters_itr = characters.at(c);

            pages_set.insert(characters_itr.texture_index);
        }

        std::copy(pages_set.begin(), pages_set.end(), std::back_inserter(pages));
    }

	i16 BitmapFont::get_string_width(const StringType& string) const
    {
        i16 width = 0;

        for (const auto c : string)
        {
            auto characters_itr = characters.find(c);

            if (characters_itr == characters.end())
            {
                characters_itr = characters.find(L'?'); //TODO: get fallback character from somewhere else
            }

            width += characters_itr->second.advance_x;
        }

        return width;
    }
};