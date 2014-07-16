#pragma once

//std
#include <stack>
#include <memory>
#include <array>
#include <vector>
#include <map>

//mandala
#include "rectangle.hpp"
#include "index_type.hpp"

namespace mandala
{
	struct gpu_program_t;
	struct frame_buffer_t;
	struct texture_t;
    struct gpu_buffer_t;

    struct gpu_mgr_t
    {
        enum class buffer_target_e
        {
            array,
            atomic_counter,
            copy_read,
            copy_write,
            draw_indrect,
            dispatch_indirect,
            element_array,
            pixel_pack,
            pixel_unpack,
            query,
            shader_storage
        };

        enum class buffer_usage_e
        {
            stream_draw,
            stream_read,
            stream_copy,
            static_draw,
            static_read,
            static_copy,
            dynamic_draw,
            dynamic_read,
            dynamic_copy
        };

        struct program_mgr_t
        {
            typedef std::weak_ptr<gpu_program_t> weak_type;
            typedef std::shared_ptr<gpu_program_t> shared_type;

            weak_type get() const;
            void push(const shared_type& data);
            weak_type pop();

        private:
            std::stack<shared_type> programs;
        } programs;

        struct frame_buffer_mgr_t
        {
            typedef std::weak_ptr<frame_buffer_t> weak_type;
            typedef std::shared_ptr<frame_buffer_t> shared_type;

            weak_type get() const;
            void push(const shared_type& frame_buffer);
            weak_type pop();

        private:
            std::stack<shared_type> frame_buffers;
        } frame_buffers;

        struct texture_mgr_t
        {
            const static auto texture_count = 32;

            typedef std::weak_ptr<texture_t> weak_type;
            typedef std::shared_ptr<texture_t> shared_type;
            typedef index_type<texture_count>::type index_type;

            weak_type get(index_type index) const;
            weak_type bind(index_type index, const shared_type& texture);
            weak_type unbind(index_type index);

        private:
            std::array<shared_type, texture_count> textures;
        } textures;

        struct viewport_mgr_t
        {
            typedef rectangle_i32_t viewport_type;

            viewport_type get() const;
            void push(const viewport_type& viewport);
            viewport_type pop();

        private:
            std::stack<viewport_type> viewports;
        } viewports;

        struct buffer_mgr_t
        {
            typedef std::shared_ptr<gpu_buffer_t> buffer_type;

            void push(buffer_target_e target, buffer_type buffer);
            buffer_type peek(buffer_target_e target);
            buffer_type pop(buffer_target_e target);
            void data(buffer_target_e target, void* data, size_t size, buffer_usage_e usage);

        private:
            std::map<buffer_target_e, std::stack<buffer_type>> target_buffers;
        } buffers;
	};

	extern gpu_mgr_t gpu;
};
