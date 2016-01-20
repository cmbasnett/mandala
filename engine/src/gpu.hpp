#pragma once

//std
#include <stack>
#include <array>
#include <vector>
#include <map>
#include <set>

//naga
#include "rectangle.hpp"
#include "index_type.hpp"
#include "gpu_defs.hpp"
#include "color_types.hpp"

//boost
#include <boost\optional.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\weak_ptr.hpp>

namespace naga
{
    struct frame_buffer;
    struct gpu_buffer;
    struct gpu_program;
    struct texture;
    struct image_t;

    struct gpu_t
    {
        enum class buffer_target
        {
            ARRAY,
            ATOMIC_COUNTER,
            COPY_READ,
            COPY_WRITE,
            DRAW_INDRECT,
            DISPATCH_INDIRECT,
            ELEMENT_ARRAY,
            PIXEL_PACK,
            PIXEL_UNPACK,
            QUERY,
            SHADER_STORAGE
        };

        enum class buffer_usage
        {
            STREAM_DRAW,
            STREAM_READ,
            STREAM_COPY,
            STATIC_DRAW,
            STATIC_READ,
            STATIC_COPY,
            DYNAMIC_DRAW,
            DYNAMIC_READ,
            DYNAMIC_COPY
        };

        enum class primitive_type
        {
            POINTS,
            LINES,
            LINE_LOOP,
            LINE_STRIP,
            TRIANGLES,
            TRIANGLE_STRIP,
            TRIANGLE_FAN,
            QUADS,
            QUAD_STRIP,
            POLYGON
        };

        enum class blend_factor
        {
            ZERO,
            ONE,
            SRC_COLOR,
            ONE_MINUS_SRC_COLOR,
            DST_COLOR,
            ONE_MINUS_DST_COLOR,
            SRC_ALPHA,
            ONE_MINUS_SRC_ALPHA,
            DST_ALPHA,
            ONE_MINUS_DST_ALPHA,
            CONSTANT_COLOR,
            ONE_MINUS_CONSTANT_COLOR,
            CONSTANT_ALPHA,
            ONE_MINUS_CONSTANT_ALPHA,
            SRC_ALPHA_SATURATE,
            DEFAULT = ONE
        };

        enum class blend_equation
        {
            ADD,
            SUBTRACT,
            SUBTRACT_REVERSE,
            MIN,
            MAX,
            DEFAULT = ADD
        };

        enum class cull_face
        {
            FRONT,
            BACK,
            FRONT_AND_BACK,
            DEFAULT = BACK
        };

        enum class depth_function
        {
            NEVER,
            LESS,
            EQUAL,
            LEQUAL,
            GREATER,
            NOTEQUAL,
            GEQUAL,
            ALWAYS,
            DEFAULT = LESS
        };

        enum class culling_front_face
        {
            CW,
            CCW
        };

        enum class culling_mode
        {
            FRONT,
            BACK,
            FRONT_AND_BACK
        };

        enum clear_flag : gpu_clear_flag_type
        {
            CLEAR_FLAG_COLOR = (1 << 0),
            CLEAR_FLAG_DEPTH = (1 << 1),
            CLEAR_FLAG_ACCUM = (1 << 2),
            CLEAR_FLAG_STENCIL = (1 << 3)
        };

        enum class stencil_function
        {
            NEVER,
            LESS,
            LEQUAL,
            GREATER,
            GEQUAL,
            EQUAL,
            NOTEQUAL,
            ALWAYS
        };

        enum class stencil_operation
        {
            KEEP,
            ZERO,
            REPLACE,
            INCR,
            INCR_WRAP,
            DECR,
            DECR_WRAP,
            INVERT
        };

        enum class shader_type
        {
            FRAGMENT,
            VERTEX
        };

        //programs
        struct program_mgr
        {
            typedef boost::weak_ptr<gpu_program> weak_type;

            boost::optional<weak_type> top() const;
            void push(const weak_type& data);
            weak_type pop();

        private:
            std::stack<weak_type> programs;
        } programs;

        //frame buffers
        struct frame_buffer_mgr
        {
            typedef boost::weak_ptr<frame_buffer> weak_type;
            typedef boost::shared_ptr<frame_buffer> shared_type;

            boost::optional<weak_type> top() const;
            void push(const shared_type& frame_buffer);
            weak_type pop();

        private:
            std::stack<shared_type> frame_buffers;
        } frame_buffers;

        //textures
        struct texture_mgr
        {
            const static auto texture_count = 32;

            typedef boost::weak_ptr<texture> weak_type;
            typedef boost::shared_ptr<texture> shared_type;
            typedef index_type<texture_count>::type index_type;

            weak_type get(index_type index) const;
            weak_type bind(index_type index, const shared_type& texture);
            weak_type unbind(index_type index);

        private:
            std::array<shared_type, texture_count> textures;
        } textures;

        //viewports
        struct viewport_mgr
        {
            gpu_viewport_type top() const;
            void push(const gpu_viewport_type& viewport);
            gpu_viewport_type pop();

        private:
            std::stack<gpu_viewport_type> viewports;
        } viewports;

        //buffers
        struct buffer_mgr
        {
            typedef boost::shared_ptr<gpu_buffer> buffer_type;

            void put(buffer_type& buffer);
            void erase(buffer_type& buffer);
            void push(buffer_target target, buffer_type buffer);
            buffer_type pop(buffer_target target);
            buffer_type top(buffer_target target) const;
            void data(buffer_target target, const void* data, size_t size, buffer_usage usage);

        private:
            std::map<buffer_target, std::stack<buffer_type>> target_buffers;
            std::set<buffer_type> buffers;
        } buffers;

        //blend
        struct blend
        {
            struct state
            {
                bool is_enabled = false;
                blend_factor src_factor = blend_factor::ONE;
                blend_factor dst_factor = blend_factor::ZERO;
                blend_equation equation = blend_equation::ADD;
            };

            state get_state() const;
            void push_state(const state& state);
            void pop_state();
        private:
            std::stack<state> states;

            void apply_state(const state& state);
        } blend;

        //depth
        struct depth
        {
            struct state
            {
                bool should_test = false;
                bool should_write_mask = true;
                depth_function function = depth_function::DEFAULT;
            };

            state get_state() const;
            void push_state(const state& state);
            void pop_state();

        private:
            std::stack<state> states;

            void apply_state(const state& state);
        } depth;

        struct culling
        {
            struct state
            {
                bool is_enabled = false;
                culling_front_face front_face = culling_front_face::CCW;
                culling_mode mode = culling_mode::BACK;
            };

            state get_state() const;
            void push_state(const state& state);
            void pop_state();

        private:
            std::stack<state> states;

            void apply_state(const state& state);
        } culling;

        //stencil
        struct stencil
        {
            struct state
            {
                struct function
                {
                    stencil_function func = stencil_function::ALWAYS;
                    i32 ref = 0;
                    u32 mask = 0xFFFFFFFF;
                };

                struct operations
                {
                    stencil_operation fail = stencil_operation::KEEP;
                    stencil_operation zfail = stencil_operation::KEEP;
                    stencil_operation zpass = stencil_operation::KEEP;
                };

                function function;
                operations operations;
                bool is_enabled = false;
                u32 mask = 0xFFFFFFFF;
            };

            state get_state() const;
            void push_state(const state& state);
            void pop_state();
        private:
            std::stack<state> states;

            void apply_state(const state& state);
        } stencil;

        //color
        struct color
        {
            struct state
            {
                struct mask
                {
                    bool r = true;
                    bool g = true;
                    bool b = true;
                    bool a = true;
                };

                mask mask;
            };

            state get_state() const;
            void push_state(const state& state);
            void pop_state();
        private:
            std::stack<state> states;

            void apply_state(const state& state);
        } color;

        void clear(const gpu_clear_flag_type clear_flags) const;
        void draw_elements(primitive_type primitive_type, size_t count, gpu_data_type index_data_type, size_t offset) const;

        gpu_id create_program(const std::string& vertex_shader_source, const std::string& fragment_shader_source) const;
        void destroy_program(gpu_id id);

        gpu_id create_buffer();
        void destroy_buffer(gpu_id id);

        gpu_id create_frame_buffer(gpu_frame_buffer_type type, const gpu_frame_buffer_size_type& size, boost::shared_ptr<texture>& color_texture, boost::shared_ptr<texture>& depth_stencil_texture, boost::shared_ptr<texture>& depth_texture);
        void destroy_frame_buffer(gpu_id id);

        gpu_id create_texture(color_type color_type, vec2_u32 size, const void* data);
        void resize_texture(const boost::shared_ptr<texture>& texture, vec2_u32 size);
        void destroy_texture(gpu_id id);

        gpu_location get_uniform_location(gpu_id program_id, const char* name) const;
        gpu_location get_attribute_location(gpu_id program_id, const char* name) const;

        void enable_vertex_attribute_array(gpu_location location);
        void disable_vertex_attribute_array(gpu_location location);
        void set_vertex_attrib_pointer(gpu_location location, i32 size, gpu_data_type data_type, bool is_normalized, i32 stride, const void* pointer);
        void set_vertex_attrib_pointer(gpu_location location, i32 size, gpu_data_type data_type, i32 stride, const void* pointer);
        void set_uniform(const char* name, const mat3& value, bool should_tranpose = false) const;
        void set_uniform(const char* name, const mat4& value, bool should_tranpose = false) const;
        void set_uniform(const char* name, i32 value) const;
        void set_uniform(const char* name, u32 value) const;
        void set_uniform(const char* name, f32 value) const;
        void set_uniform(const char* name, const vec2& value) const;
        void set_uniform(const char* name, const vec3& value) const;
        void set_uniform(const char* name, const vec4& value) const;
        void set_uniform(const char* name, const std::vector<mat4>& value, bool should_transpose = false) const;
        void set_uniform_subroutine(shader_type shader_type, gpu_index index);

        void set_clear_color(vec4& color);
        vec4 get_clear_color();

        gpu_location get_subroutine_uniform_location(gpu_id program_id, shader_type shader_type, const std::string& name);
        gpu_index get_subroutine_index(gpu_id program_id, shader_type shader_type, const std::string& name);

        const std::string& get_vendor() const;
        const std::string& get_renderer() const;
        const std::string& get_version() const;
        const std::string& get_shading_language_version() const;
        const std::string& get_extensions() const;

        void get_texture_data(const boost::shared_ptr<texture>& texture, std::vector<u8>& data, i32 level = 0);
    };

    extern gpu_t gpu;
}
