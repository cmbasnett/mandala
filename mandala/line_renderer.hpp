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
    template<typename T>
    void render_rectangle(const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const details::rectangle_t<T>& rectangle, bool is_filled = false)
    {
        typedef vertex_buffer_t<basic_gpu_vertex_t> vertex_buffer_type;
        typedef index_buffer_t<uint8_t> index_buffer_type;

        static boost::weak_ptr<vertex_buffer_type> vertex_buffer;
        static boost::weak_ptr<index_buffer_type> index_buffer;

        if (vertex_buffer.expired())
        {
            vertex_buffer = gpu_buffers.make<vertex_buffer_type>();
            auto vertices = {
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(0, 0, 0), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type()),
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(1, 0, 0), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type()),
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(1, 1, 0), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type()),
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(0, 1, 0), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type())
            };
            vertex_buffer.lock()->data(vertices, gpu_t::buffer_usage_e::dynamic_draw);
        }

        if (index_buffer.expired())
        {
            index_buffer = gpu_buffers.make<index_buffer_type>();
            index_buffer.lock()->data({ 0, 1, 2, 3 }, gpu_t::buffer_usage_e::static_draw);
        }

        gpu.buffers.push(gpu_t::buffer_target_e::array, vertex_buffer.lock());
        gpu.buffers.push(gpu_t::buffer_target_e::element_array, index_buffer.lock());

        const auto gpu_program = gpu_programs.get<basic_gpu_program_t>();

        gpu.programs.push(gpu_program);

        gpu.set_uniform("world_matrix", world_matrix * glm::translate(rectangle.x, rectangle.y, T(0)) * glm::scale(rectangle.width, rectangle.height, T(0)));
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);

        gpu.draw_elements(is_filled ? gpu_t::primitive_type_e::triangle_fan : gpu_t::primitive_type_e::line_loop, 4, index_buffer_type::data_type, 0);

        gpu.programs.pop();

        gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
        gpu.buffers.pop(gpu_t::buffer_target_e::array);
    }

    template<typename T>
    void render_aabb(const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const details::aabb3_t<T>& aabb)
    {
        typedef vertex_buffer_t<basic_gpu_vertex_t> vertex_buffer_type;
        typedef index_buffer_t<uint8_t> index_buffer_type;

        static boost::weak_ptr<vertex_buffer_type> vertex_buffer;
        static boost::weak_ptr<index_buffer_type> index_buffer;

        if (vertex_buffer.expired())
        {
            vertex_buffer = gpu_buffers.make<vertex_buffer_type>();
            auto vertices = {
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(0, 0, 0), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type()),
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(1, 0, 0), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type()),
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(1, 1, 0), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type()),
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(0, 1, 0), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type()),
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(0, 0, 1), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type()),
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(1, 0, 1), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type()),
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(1, 1, 1), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type()),
                basic_gpu_vertex_t(basic_gpu_vertex_t::location_type(0, 1, 1), basic_gpu_vertex_t::color_type(1), basic_gpu_vertex_t::texcoord_type())
            };
            vertex_buffer.lock()->data(vertices, gpu_t::buffer_usage_e::dynamic_draw);
        }

        if (index_buffer.expired())
        {
            index_buffer = gpu_buffers.make<index_buffer_type>();
            index_buffer.lock()->data({ 0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 1, 5, 2, 6, 3, 7, 4, 5, 5, 6, 6, 7, 7, 4 }, gpu_t::buffer_usage_e::static_draw);
        }

        gpu.buffers.push(gpu_t::buffer_target_e::array, vertex_buffer.lock());
        gpu.buffers.push(gpu_t::buffer_target_e::element_array, index_buffer.lock());

        const auto gpu_program = gpu_programs.get<basic_gpu_program_t>();

        gpu.programs.push(gpu_program);

        gpu.set_uniform("world_matrix", world_matrix * glm::translate(aabb.min) * glm::scale(aabb.size()));
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);

        gpu.draw_elements(gpu_t::primitive_type_e::lines, 24, index_buffer_type::data_type, 0);

        gpu.programs.pop();

        gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
        gpu.buffers.pop(gpu_t::buffer_target_e::array);
    }

    template<typename T>
    void render_sphere(const mat4_t& world_matrix, const mat4_t& view_projection_matrix, const details::sphere_t<T>& sphere)
    {
        typedef vertex_buffer_t<basic_gpu_vertex_t> vertex_buffer_type;
        typedef index_buffer_t<uint8_t> index_buffer_type;

        static boost::weak_ptr<vertex_buffer_type> vertex_buffer;
        static boost::weak_ptr<index_buffer_type> index_buffer;

        const size_t SPHERE_SIDES = 32;

        if (vertex_buffer.expired())
        {
            vertex_buffer = gpu_buffers.make<vertex_buffer_type>();

            std::vector<basic_gpu_vertex_t> vertices;
            vertices.reserve(SPHERE_SIDES);

            //TODO: this is lame, write this better
            for (auto i = 0; i < SPHERE_SIDES; ++i)
            {
                auto sigma = static_cast<float32_t>(i) / SPHERE_SIDES * glm::pi<float32_t>() * 2;

                vertices.emplace_back(basic_gpu_vertex_t::location_type(glm::sin(sigma), glm::cos(sigma), 0), basic_gpu_vertex_t::color_type(0, 0, 1, 1), basic_gpu_vertex_t::texcoord_type());
            }

            for (auto i = 0; i < SPHERE_SIDES; ++i)
            {
                auto sigma = static_cast<float32_t>(i) / SPHERE_SIDES * glm::pi<float32_t>() * 2;

                vertices.emplace_back(basic_gpu_vertex_t::location_type(0, glm::sin(sigma), glm::cos(sigma)), basic_gpu_vertex_t::color_type(1, 0, 0, 1), basic_gpu_vertex_t::texcoord_type());
            }

            for (auto i = 0; i < SPHERE_SIDES; ++i)
            {
                auto sigma = static_cast<float32_t>(i) / SPHERE_SIDES * glm::pi<float32_t>() * 2;

                vertices.emplace_back(basic_gpu_vertex_t::location_type(glm::cos(sigma), 0, glm::sin(sigma)), basic_gpu_vertex_t::color_type(0, 1, 0, 1), basic_gpu_vertex_t::texcoord_type());
            }

            vertex_buffer.lock()->data(vertices, gpu_t::buffer_usage_e::dynamic_draw);
        }

        if (index_buffer.expired())
        {
            index_buffer = gpu_buffers.make<index_buffer_type>();

            std::vector<uint8_t> indices;
            indices.reserve(SPHERE_SIDES * 3);

            for (auto i = 0; i < SPHERE_SIDES * 3; ++i)
            {
                indices.push_back(i);
            }

            index_buffer.lock()->data(indices, gpu_t::buffer_usage_e::static_draw);
        }

        gpu.buffers.push(gpu_t::buffer_target_e::array, vertex_buffer.lock());
        gpu.buffers.push(gpu_t::buffer_target_e::element_array, index_buffer.lock());

        const auto gpu_program = gpu_programs.get<basic_gpu_program_t>();

        gpu.programs.push(gpu_program);

        gpu.set_uniform("world_matrix", world_matrix * glm::translate(sphere.origin) * glm::scale(vec3_t(sphere.radius)));
        gpu.set_uniform("view_projection_matrix", view_projection_matrix);

        gpu.draw_elements(gpu_t::primitive_type_e::line_loop, SPHERE_SIDES, index_buffer_type::data_type, SPHERE_SIDES * 0);
        gpu.draw_elements(gpu_t::primitive_type_e::line_loop, SPHERE_SIDES, index_buffer_type::data_type, SPHERE_SIDES * 1);
        gpu.draw_elements(gpu_t::primitive_type_e::line_loop, SPHERE_SIDES, index_buffer_type::data_type, SPHERE_SIDES * 2);

        gpu.programs.pop();

        gpu.buffers.pop(gpu_t::buffer_target_e::element_array);
        gpu.buffers.pop(gpu_t::buffer_target_e::array);
    }
}