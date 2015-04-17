#pragma once

//std
#include <stack>
#include <memory>
#include <array>
#include <vector>
#include <map>
#include <set>

//mandala
#include "rectangle.hpp"
#include "index_type.hpp"
#include "gpu_defs.hpp"
#include "color_types.hpp"

//boost
#include <boost\optional.hpp>

namespace mandala
{
    struct frame_buffer_t;
    struct gpu_buffer_t;
	struct gpu_program_t;
	struct texture_t;
    struct image_t;

    struct gpu_t
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

        enum class primitive_type_e
        {
            points,
            lines,
            line_loop,
            line_strip,
            triangles,
            triangle_strip,
            triangle_fan,
            quads,
            quad_strip,
            polygon
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

        enum class culling_front_face_e
        {
            cw,
            ccw
        };

        enum class culling_mode_e
        {
            front,
            back,
            front_and_back
        };

        enum clear_flag_e : gpu_clear_flag_type
        {
            clear_flag_color = (1 << 0),
            clear_flag_depth = (1 << 1),
            clear_flag_accum = (1 << 2),
            clear_flag_stencil = (1 << 3)
        };

        enum class stencil_function_e
        {
            never,
            less,
            lequal,
            greater,
            gequal,
            equal,
            notequal,
            always
        };

        enum class stencil_operation_e
        {
            keep,
            zero,
            replace,
            incr,
            incr_wrap,
            decr,
            decr_wrap,
            invert
        };

        enum class shader_type_e
        {
            fragment,
            vertex
        };

		//programs
        struct program_mgr_t
        {
            typedef std::weak_ptr<gpu_program_t> weak_type;

            boost::optional<weak_type> top() const;
            void push(const weak_type& data);
            weak_type pop();

        private:
            std::stack<weak_type> programs;
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
			gpu_viewport_type top() const;
            void push(const gpu_viewport_type& viewport);
			gpu_viewport_type pop();

        private:
            std::stack<gpu_viewport_type> viewports;
        } viewports;

		//buffers
        struct buffer_mgr_t
        {
            typedef std::shared_ptr<gpu_buffer_t> buffer_type;

            void put(buffer_type& buffer);
            void erase(buffer_type& buffer);
            void push(buffer_target_e target, buffer_type buffer);
            buffer_type pop(buffer_target_e target);
            buffer_type top(buffer_target_e target) const;
            void data(buffer_target_e target, const void* data, size_t size, buffer_usage_e usage);

        private:
            std::map<buffer_target_e, std::stack<buffer_type>> target_buffers;
            std::set<buffer_type> buffers;
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

			state_t get_state() const;
			void push_state(const state_t& state);
			void pop_state();
		private:
			std::stack<state_t> states;

			void apply_state(const state_t& state);
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

            state_t get_state() const;
			void push_state(const state_t& state);
			void pop_state();

		private:
			std::stack<state_t> states;

			void apply_state(const state_t& state);
		} depth;

        struct culling_t
        {
            struct state_t
            {
                bool is_enabled = false;
                culling_front_face_e front_face = culling_front_face_e::ccw;
                culling_mode_e mode = culling_mode_e::back;
            };

            state_t get_state() const;
            void push_state(const state_t& state);
            void pop_state();

        private:
            std::stack<state_t> states;

            void apply_state(const state_t& state);
        } culling;

        //stencil
        struct stencil_t
        {
            struct state_t
            {
                struct function_t
                {
                    stencil_function_e func = stencil_function_e::always;
                    int32_t ref = 0;
                    uint32_t mask = 0xFFFFFFFF;
                };

				struct operations_t
                {
                    stencil_operation_e fail = stencil_operation_e::keep;
                    stencil_operation_e zfail = stencil_operation_e::keep;
                    stencil_operation_e zpass = stencil_operation_e::keep;
                };

				function_t function;
				operations_t operations;
                bool is_enabled = false;
                uint32_t mask = 0xFFFFFFFF;
            };

            state_t get_state() const;
            void push_state(const state_t& state);
            void pop_state();
        private:
            std::stack<state_t> states;

            void apply_state(const state_t& state);
        } stencil;

        //color
        struct color_t
        {
            struct state_t
            {
                struct mask_t
                {
                    bool r = true;
                    bool g = true;
                    bool b = true;
                    bool a = true;
                };

				mask_t mask;
            };

            state_t get_state() const;
            void push_state(const state_t& state);
            void pop_state();
        private:
            std::stack<state_t> states;

            void apply_state(const state_t& state);
        } color;

        void clear(const gpu_clear_flag_type clear_flags) const;
		void draw_elements(primitive_type_e primitive_type, size_t count, gpu_data_type_e index_data_type, size_t offset) const;

		gpu_id_t create_program(const std::string& vertex_shader_source, const std::string& fragment_shader_source) const;
		void destroy_program(gpu_id_t id);

		gpu_id_t create_buffer();
		void destroy_buffer(gpu_id_t id);

		gpu_id_t create_frame_buffer(gpu_frame_buffer_type_e type, const gpu_frame_buffer_size_type& size, std::shared_ptr<texture_t>& color_texture, std::shared_ptr<texture_t>& depth_stencil_texture, std::shared_ptr<texture_t>& depth_texture);
		void destroy_frame_buffer(gpu_id_t id);

        gpu_id_t create_texture(color_type_e color_type, vec2_u32_t size, const void* data);
        void resize_texture(const std::shared_ptr<texture_t>& texture, vec2_u32_t size);
        void destroy_texture(gpu_id_t id);

        gpu_location_t get_uniform_location(gpu_id_t program_id, const char* name) const;
        gpu_location_t get_attribute_location(gpu_id_t program_id, const char* name) const;

		void enable_vertex_attribute_array(gpu_location_t location);
		void disable_vertex_attribute_array(gpu_location_t location);
		void set_vertex_attrib_pointer(gpu_location_t location, int32_t size, gpu_data_type_e data_type, bool is_normalized, int32_t stride, const void* pointer);
		void set_vertex_attrib_pointer(gpu_location_t location, int32_t size, gpu_data_type_e data_type, int32_t stride, const void* pointer);
        void set_uniform(const char* name, const mat3_t& value, bool should_tranpose = false) const;
        void set_uniform(const char* name, const mat4_t& value, bool should_tranpose = false) const;
        void set_uniform(const char* name, int32_t value) const;
        void set_uniform(const char* name, uint32_t value) const;
        void set_uniform(const char* name, float32_t value) const;
        void set_uniform(const char* name, const vec2_t& value) const;
        void set_uniform(const char* name, const vec3_t& value) const;
        void set_uniform(const char* name, const vec4_t& value) const;
        void set_uniform(const char* name, const std::vector<mat4_t>& value, bool should_transpose = false) const;
		void set_uniform_subroutine(shader_type_e shader_type, gpu_index_t index);

		void set_clear_color(rgba_type& color);
		rgba_type get_clear_color();

		gpu_location_t get_subroutine_uniform_location(gpu_id_t program_id, shader_type_e shader_type, const std::string& name);
		gpu_index_t get_subroutine_index(gpu_id_t program_id, shader_type_e shader_type, const std::string& name);

        const std::string& get_vendor() const;
        const std::string& get_renderer() const;
        const std::string& get_version() const;
        const std::string& get_shading_language_version() const;
        const std::string& get_extensions() const;

        void get_texture_data(const std::shared_ptr<texture_t>& texture, std::vector<uint8_t>& data, int32_t level = 0);
	};

    extern gpu_t gpu;
}
