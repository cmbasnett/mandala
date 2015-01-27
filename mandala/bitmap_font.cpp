//std
#include <set>

//glm
#include <glm\ext.hpp>

//mandala
#include "bitmap_font.hpp"
#include "resource_mgr.hpp"
#include "hash.hpp"
#include "texture.hpp"
#include "gpu_program.hpp"
#include "gpu_program_mgr.hpp"
#include "gpu.hpp"
#include "bitmap_font_gpu_program.hpp"
#include "io.hpp"

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
        read(istream, size);

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

        read(istream, flags);

        is_smooth = (flags & flag_smooth) == flag_smooth;
        is_unicode = (flags & flag_unicode) == flag_unicode;
        is_italic = (flags & flag_italic) == flag_italic;
        is_bold = (flags & flag_bold) == flag_bold;
        is_fixed_height = (flags & flag_fixed_height) == flag_fixed_height;

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
        uint32_t page_texture_names_length;
        read(istream, page_texture_names_length);

        while (page_texture_names_length > 0)
        {
            std::string page_texture_name;
            std::getline(istream, page_texture_name, '\0');

            page_texture_names_length -= static_cast<uint32_t>(page_texture_name.length() + 1);

            auto page_texture = resources.get<texture_t>(hash_t(page_texture_name));

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
        read(istream, characters_length);

        auto character_count = characters_length / sizeof(character_t);

        for (size_t i = 0; i < character_count; ++i)
        {
            character_t character;

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
            uint32_t kerning_pairs_length;
            read(istream, kerning_pairs_length);

            auto kerning_pair_count = kerning_pairs_length / sizeof(kerning_pair_t);
            kerning_pairs.resize(kerning_pair_count);

            for (auto& kerning_pair : kerning_pairs)
            {
                read(istream, kerning_pair.first);
                read(istream, kerning_pair.second);
                read(istream, kerning_pair.amount);
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
            vertices[j + 0].position.x = static_cast<float32_t>(character.offset.x);
            vertices[j + 0].position.y = -static_cast<float32_t>(character.offset.y + character.rectangle.height - base);

            //location 1
            vertices[j + 1].position.x = static_cast<float32_t>(character.offset.x + character.rectangle.width);
            vertices[j + 1].position.y = -static_cast<float32_t>(character.offset.y + character.rectangle.height - base);

            //location 2
            vertices[j + 2].position.x = static_cast<float32_t>(character.offset.x + character.rectangle.width);
            vertices[j + 2].position.y = -static_cast<float32_t>(character.offset.y - base);

            //location 3
            vertices[j + 3].position.x = static_cast<float32_t>(character.offset.x);
            vertices[j + 3].position.y = -static_cast<float32_t>(character.offset.y - base);

            //texcoord 0
            vertices[j + 0].texcoord.x = static_cast<float32_t>(character.rectangle.x) / width;
            vertices[j + 0].texcoord.y = 1.0f - static_cast<float32_t>(character.rectangle.y + character.rectangle.height) / height;

            //texcoord 1
            vertices[j + 1].texcoord.x = static_cast<float32_t>(character.rectangle.x + character.rectangle.width) / width;
            vertices[j + 1].texcoord.y = 1.0f - static_cast<float32_t>(character.rectangle.y + character.rectangle.height) / height;

            //texcoord 2
            vertices[j + 2].texcoord.x = static_cast<float32_t>(character.rectangle.x + character.rectangle.width) / width;
            vertices[j + 2].texcoord.y = 1.0f - static_cast<float32_t>(character.rectangle.y) / height;

            //texcoord 3
            vertices[j + 3].texcoord.x = static_cast<float32_t>(character.rectangle.x) / width;
            vertices[j + 3].texcoord.y = 1.0f - static_cast<float32_t>(character.rectangle.y) / height;

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

    int16_t bitmap_font_t::get_kerning_amount(char_type lhs, char_type rhs) const
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

    void bitmap_font_t::render_string(const string_type& string, mat4_t world_matrix, mat4_t view_projection_matrix, const rgba_type& base_color, std::stack<rgba_type>& color_stack, const std::vector<std::pair<size_t, rgba_type>>& color_pushes, const std::vector<size_t>& color_pop_indices) const
    {
        static const auto character_index_stride = sizeof(index_type) * indices_per_character;

        //buffers
        gpu.buffers.push(gpu_t::buffer_target_e::array, vertex_buffer);
        gpu.buffers.push(gpu_t::buffer_target_e::element_array, index_buffer);

        const auto gpu_program = gpu_programs.get<bitmap_font_gpu_program_t>();

        //program
        gpu.programs.push(gpu_program);

        //states
        auto blend_state = gpu.blend.get_state();
        blend_state.is_enabled = true;
        blend_state.src_factor = gpu_t::blend_factor_e::src_alpha;
        blend_state.dst_factor = gpu_t::blend_factor_e::one_minus_src_alpha;

        gpu.blend.push_state(blend_state);

        auto depth_state = gpu.depth.get_state();
        depth_state.should_write_mask = false;

        gpu.depth.push_state(depth_state);

        //view projection matrix
        gpu_program->view_projection_matrix(view_projection_matrix);
        gpu_program->font_line_height(line_height);
        gpu_program->font_base(base);

        auto start_color = color_stack.empty() ? base_color : color_stack.top();

        gpu_program->font_color_top(start_color);
        gpu_program->font_color_bottom(start_color);

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
            gpu_program->world_matrix(world_matrix);

            const auto character = characters.at(*c);

            if (character_texture_index != character.texture_index)
            {
                character_texture_index = character.texture_index;

                gpu_program->font_diffuse_texture_index(character_texture_index);
            }

            auto x = static_cast<float32_t>(character.advance_x);
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

                gpu_program->font_color_top(color);
                gpu_program->font_color_bottom(color);
            }

            gpu.draw_elements(gpu_t::primitive_type_e::triangles, indices_per_character, index_buffer_type::data_type, character_index * character_index_stride);

            const auto next = (c + 1);

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


        //states
        gpu.depth.pop_state();
        gpu.blend.pop_state();

        //program
        gpu.programs.pop();

        //buffers
        gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
        gpu.buffers.pop(gpu_t::buffer_target_e::array);
    }

    void bitmap_font_t::get_string_pages(std::vector<uint8_t>& pages, const string_type& string) const
    {
        std::set<uint8_t> pages_set;

        for (const auto c : string)
        {
            auto characters_itr = characters.at(c);

            pages_set.insert(characters_itr.texture_index);
        }

        std::copy(pages_set.begin(), pages_set.end(), std::back_inserter(pages));
    }

    int16_t bitmap_font_t::get_string_width(const string_type& string) const
    {
        int16_t width = 0;

        for (const auto c : string)
        {
            auto characters_itr = characters.find(c);

            if (characters_itr == characters.end())
            {
                characters_itr = characters.find(L'?'); //TODO: get this from somewhere else
            }

            width += characters_itr->second.advance_x;
        }

        return width;
    }
};