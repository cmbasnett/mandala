#pragma once

//mandala
#include "types.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "gpu_program_mgr.hpp"
#include "basic_gpu_program.hpp"
#include "rectangle.hpp"
#include "line.hpp"
#include "sphere.hpp"

#include "gpu_buffer_mgr.hpp"

namespace mandala
{
    inline void render_line_loop(const mat4& world_matrix, const mat4& view_projection_matrix, const std::vector<vec3>& points, const vec4& color)
    {
        static const size_t MAX_LINES = 1024;
        static const size_t VERTEX_COUNT = MAX_LINES + 1;
        static const size_t INDEX_COUNT = MAX_LINES;

        assert(points.size() <= MAX_LINES);

        typedef vertex_buffer_t<basic_gpu_program_t::vertex_type> vertex_buffer_type;
        typedef index_buffer_t<index_type<INDEX_COUNT>::type> index_buffer_type;

        static boost::weak_ptr<vertex_buffer_type> vertex_buffer;
        static boost::weak_ptr<index_buffer_type> index_buffer;

        static std::array<basic_gpu_program_t::vertex_type, VERTEX_COUNT> vertices;

        if (vertex_buffer.expired())
        {
            vertex_buffer = gpu_buffers.make<vertex_buffer_type>();

            for (auto& vertex : vertices)
            {
                vertex.color = vec4(1);
            }
        }

        for (size_t i = 0; i < points.size(); ++i)
        {
            vertices[i].location = points[i];
        }

        vertex_buffer.lock()->data(vertices.data(), points.size(), gpu_t::buffer_usage_e::STREAM_DRAW);

        if (index_buffer.expired())
        {
            std::array<index_buffer_type::index_type, INDEX_COUNT> indices;

            for (size_t i = 0; i < INDEX_COUNT; ++i)
            {
                indices[i] = i;
            }

            index_buffer = gpu_buffers.make<index_buffer_type>();
            index_buffer.lock()->data(indices.data(), indices.size(), gpu_t::buffer_usage_e::STATIC_DRAW);
        }

        gpu.buffers.push(gpu_t::buffer_target_e::ARRAY, vertex_buffer.lock());
        gpu.buffers.push(gpu_t::buffer_target_e::ELEMENT_ARRAY, index_buffer.lock());

        const auto gpu_program = gpu_programs.get<basic_gpu_program_t>();

        gpu.programs.push(gpu_program);

        gpu.set_uniform("world_matrix", world_matrix);
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);
        gpu.set_uniform("color", color);

        gpu.draw_elements(gpu_t::primitive_type_e::LINE_STRIP, points.size(), index_buffer_type::DATA_TYPE, 0);

        gpu.programs.pop();

        gpu.buffers.pop(gpu_t::buffer_target_e::ARRAY);
        gpu.buffers.pop(gpu_t::buffer_target_e::ELEMENT_ARRAY);
    }

    template<typename T>
    void render_rectangle(const mat4& world_matrix, const mat4& view_projection_matrix, const details::rectangle<T>& rectangle, const vec4& color, bool is_filled = false)
    {
        typedef vertex_buffer_t<basic_gpu_program_t::vertex_type> vertex_buffer_type;
        typedef index_buffer_t<u8> index_buffer_type;

        static boost::weak_ptr<vertex_buffer_type> vertex_buffer;
        static boost::weak_ptr<index_buffer_type> index_buffer;

        if (vertex_buffer.expired())
        {
            vertex_buffer = gpu_buffers.make<vertex_buffer_type>();
            auto vertices = {
                basic_gpu_program_t::vertex_type(vec3(0, 0, 0), vec4(1)),
                basic_gpu_program_t::vertex_type(vec3(1, 0, 0), vec4(1)),
                basic_gpu_program_t::vertex_type(vec3(1, 1, 0), vec4(1)),
                basic_gpu_program_t::vertex_type(vec3(0, 1, 0), vec4(1))
            };
            vertex_buffer.lock()->data(vertices, gpu_t::buffer_usage_e::STATIC_DRAW);
        }

        if (index_buffer.expired())
        {
            index_buffer = gpu_buffers.make<index_buffer_type>();
            index_buffer.lock()->data({ 0, 1, 2, 3 }, gpu_t::buffer_usage_e::STATIC_DRAW);
        }

        gpu.buffers.push(gpu_t::buffer_target_e::ARRAY, vertex_buffer.lock());
        gpu.buffers.push(gpu_t::buffer_target_e::ELEMENT_ARRAY, index_buffer.lock());

        const auto gpu_program = gpu_programs.get<basic_gpu_program_t>();

        gpu.programs.push(gpu_program);

        gpu.set_uniform("world_matrix", world_matrix * glm::translate(rectangle.x, rectangle.y, T(0)) * glm::scale(rectangle.width, rectangle.height, T(0)));
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);
        gpu.set_uniform("color", color);

        gpu.draw_elements(is_filled ? gpu_t::primitive_type_e::TRIANGLE_FAN : gpu_t::primitive_type_e::LINE_LOOP, 4, index_buffer_type::DATA_TYPE, 0);

        gpu.programs.pop();

        gpu.buffers.pop(gpu_t::buffer_target_e::ELEMENT_ARRAY);
        gpu.buffers.pop(gpu_t::buffer_target_e::ARRAY);
    }

    template<typename T>
    void render_aabb(const mat4& world_matrix, const mat4& view_projection_matrix, const details::aabb3<T>& aabb, const vec4& color)
    {
        typedef vertex_buffer_t<basic_gpu_program_t::vertex_type> vertex_buffer_type;
        typedef index_buffer_t<u8> index_buffer_type;

        static boost::weak_ptr<vertex_buffer_type> vertex_buffer;
        static boost::weak_ptr<index_buffer_type> index_buffer;

        if (vertex_buffer.expired())
        {
            vertex_buffer = gpu_buffers.make<vertex_buffer_type>();
            auto vertices = {
                basic_gpu_program_t::vertex_type(vec3(0, 0, 0), vec4(1)),
                basic_gpu_program_t::vertex_type(vec3(1, 0, 0), vec4(1)),
                basic_gpu_program_t::vertex_type(vec3(1, 1, 0), vec4(1)),
                basic_gpu_program_t::vertex_type(vec3(0, 1, 0), vec4(1)),
                basic_gpu_program_t::vertex_type(vec3(0, 0, 1), vec4(1)),
                basic_gpu_program_t::vertex_type(vec3(1, 0, 1), vec4(1)),
                basic_gpu_program_t::vertex_type(vec3(1, 1, 1), vec4(1)),
                basic_gpu_program_t::vertex_type(vec3(0, 1, 1), vec4(1))
            };
            vertex_buffer.lock()->data(vertices, gpu_t::buffer_usage_e::DYNAMIC_DRAW);
        }

        if (index_buffer.expired())
        {
            index_buffer = gpu_buffers.make<index_buffer_type>();
            index_buffer.lock()->data({ 0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 1, 5, 2, 6, 3, 7, 4, 5, 5, 6, 6, 7, 7, 4 }, gpu_t::buffer_usage_e::STATIC_DRAW);
        }

        gpu.buffers.push(gpu_t::buffer_target_e::ARRAY, vertex_buffer.lock());
        gpu.buffers.push(gpu_t::buffer_target_e::ELEMENT_ARRAY, index_buffer.lock());

        const auto gpu_program = gpu_programs.get<basic_gpu_program_t>();

        gpu.programs.push(gpu_program);

        gpu.set_uniform("world_matrix", world_matrix * glm::translate(aabb.min) * glm::scale(aabb.size()));
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);
        gpu.set_uniform("color", color);

        gpu.draw_elements(gpu_t::primitive_type_e::LINES, 24, index_buffer_type::DATA_TYPE, 0);

        gpu.programs.pop();

        gpu.buffers.pop(gpu_t::buffer_target_e::ELEMENT_ARRAY);
        gpu.buffers.pop(gpu_t::buffer_target_e::ARRAY);
    }

    template<typename T>
    void render_sphere(const mat4& world_matrix, const mat4& view_projection_matrix, const details::sphere<T>& sphere)
    {
        typedef vertex_buffer_t<basic_gpu_program_t::vertex_type> vertex_buffer_type;
        typedef index_buffer_t<u8> index_buffer_type;

        static boost::weak_ptr<vertex_buffer_type> vertex_buffer;
        static boost::weak_ptr<index_buffer_type> index_buffer;

        const size_t SPHERE_SIDES = 32;

        if (vertex_buffer.expired())
        {
            vertex_buffer = gpu_buffers.make<vertex_buffer_type>();

            std::vector<basic_gpu_program_t::vertex_type> vertices;
            vertices.reserve(SPHERE_SIDES);

            //TODO: this is lame, write this better
            for (auto i = 0; i < SPHERE_SIDES; ++i)
            {
                auto sigma = static_cast<f32>(i) / SPHERE_SIDES * glm::pi<f32>() * 2;

                vertices.emplace_back(vec3(glm::sin(sigma), glm::cos(sigma), 0), vec4(0, 0, 1, 1));
            }

            for (auto i = 0; i < SPHERE_SIDES; ++i)
            {
                auto sigma = static_cast<f32>(i) / SPHERE_SIDES * glm::pi<f32>() * 2;

                vertices.emplace_back(vec3(0, glm::sin(sigma), glm::cos(sigma)), vec4(1, 0, 0, 1));
            }

            for (auto i = 0; i < SPHERE_SIDES; ++i)
            {
                auto sigma = static_cast<f32>(i) / SPHERE_SIDES * glm::pi<f32>() * 2;

                vertices.emplace_back(vec3(glm::cos(sigma), 0, glm::sin(sigma)), vec4(0, 1, 0, 1));
            }

            vertex_buffer.lock()->data(vertices, gpu_t::buffer_usage_e::DYNAMIC_DRAW);
        }

        if (index_buffer.expired())
        {
            index_buffer = gpu_buffers.make<index_buffer_type>();

            std::vector<u8> indices;
            indices.reserve(SPHERE_SIDES * 3);

            for (auto i = 0; i < SPHERE_SIDES * 3; ++i)
            {
                indices.push_back(i);
            }

            index_buffer.lock()->data(indices, gpu_t::buffer_usage_e::STATIC_DRAW);
        }

        gpu.buffers.push(gpu_t::buffer_target_e::ARRAY, vertex_buffer.lock());
        gpu.buffers.push(gpu_t::buffer_target_e::ELEMENT_ARRAY, index_buffer.lock());

        const auto gpu_program = gpu_programs.get<basic_gpu_program_t>();

        gpu.programs.push(gpu_program);

        gpu.set_uniform("world_matrix", world_matrix * glm::translate(sphere.origin) * glm::scale(vec3(sphere.radius)));
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);

        gpu.draw_elements(gpu_t::primitive_type_e::LINE_LOOP, SPHERE_SIDES, index_buffer_type::DATA_TYPE, SPHERE_SIDES * 0);
        gpu.draw_elements(gpu_t::primitive_type_e::LINE_LOOP, SPHERE_SIDES, index_buffer_type::DATA_TYPE, SPHERE_SIDES * 1);
        gpu.draw_elements(gpu_t::primitive_type_e::LINE_LOOP, SPHERE_SIDES, index_buffer_type::DATA_TYPE, SPHERE_SIDES * 2);

        gpu.programs.pop();

        gpu.buffers.pop(gpu_t::buffer_target_e::ELEMENT_ARRAY);
        gpu.buffers.pop(gpu_t::buffer_target_e::ARRAY);
    }
}