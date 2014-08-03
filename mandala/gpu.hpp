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

//boost
#include <boost\optional.hpp>

namespace mandala
{
	struct gpu_program_t;
	struct frame_buffer_t;
	struct texture_t;
    struct gpu_buffer_t;

    struct gpu_t
	{
		typedef uint32_t program_id_type;
		typedef int32_t attribute_location_type;
		typedef int32_t uniform_location_type;

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

        enum class primitive_type_e
        {
            points,
            lines,
            line_strip,
            line_loop,
            triangles,
            triangle_strip,
            triangle_fan,
            quads,
            quad_strip,
            polygon
        };

        enum class index_type_e
        {
            unsigned_byte,
            unsigned_short,
            unsigned_int
		};

		enum class blend_factor_e
		{
			zero,
			one,
			src_color,
			one_minus_src_color,
			dst_color,
			one_minus_dst_color,
			src_alpha,
			one_minus_src_alpha,
			dst_alpha,
			one_minus_dst_alpha,
			constant_color,
			one_minus_constant_color,
			constant_alpha,
			one_minus_constant_alpha,
			src_alpha_saturate,
			default = one
		};

		enum class blend_equation_e
		{
			add,
			subtract,
			subtract_reverse,
			min,
			max,
			default = add
		};

		enum class cull_face_e
		{
			front,
			back,
			front_and_back,
			default = back
		};

		enum class depth_function_e
		{
			never,
			less,
			equal,
			lequal,
			greater,
			notequal,
			gequal,
			always,
			default = less
		};

		//programs
        struct program_mgr_t
        {
            typedef std::weak_ptr<gpu_program_t> weak_type;
            typedef std::shared_ptr<gpu_program_t> shared_type;

            boost::optional<weak_type> top() const;
            void push(const shared_type& data);
            weak_type pop();

        private:
            std::stack<shared_type> programs;
        } programs;

		//frame buffers
        struct frame_buffer_mgr_t
        {
            typedef std::weak_ptr<frame_buffer_t> weak_type;
            typedef std::shared_ptr<frame_buffer_t> shared_type;

            boost::optional<weak_type> top() const;
            void push(const shared_type& frame_buffer);
            weak_type pop();

        private:
            std::stack<shared_type> frame_buffers;
        } frame_buffers;

		//textures
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

		//viewports
        struct viewport_mgr_t
        {
            typedef rectangle_i32_t viewport_type;

			viewport_type top() const;
            void push(const viewport_type& viewport);
            viewport_type pop();

        private:
            std::stack<viewport_type> viewports;
        } viewports;

		//buffers
        struct buffer_mgr_t
        {
            typedef std::shared_ptr<gpu_buffer_t> buffer_type;

            void push(buffer_target_e target, buffer_type buffer);
			buffer_type top(buffer_target_e target);
            buffer_type pop(buffer_target_e target);
            void data(buffer_target_e target, void* data, size_t size, buffer_usage_e usage);

        private:
            std::map<buffer_target_e, std::stack<buffer_type>> target_buffers;
        } buffers;

		//blend
		struct blend_t
		{
			struct state_t
			{
				bool is_enabled = false;
				blend_factor_e src_factor = blend_factor_e::one;
				blend_factor_e dst_factor = blend_factor_e::zero;
				blend_equation_e equation = blend_equation_e::add;
			};

			state_t top() const;
			void push(const state_t& state);
			void pop();
		private:
			std::stack<state_t> states;

			void apply(const state_t& state);
		} blend;

		//depth
		struct depth_t
		{
			struct state_t
			{
				bool should_test = false;
				bool should_write_mask = true;
				depth_function_e function = depth_function_e::default;
			};

			state_t top() const;
			void push(const state_t& state);
			void pop();

		private:
			std::stack<state_t> states;

			void apply(const state_t& state);
		} depth;

		void draw_elements(primitive_type_e primitive_type, size_t count, index_type_e index_type, size_t offset) const;

		program_id_type create_program(const std::string& vertex_shader_source, const std::string& fragment_shader_source) const;
		void destroy_program(program_id_type id);
	};

    extern gpu_t gpu;
};
