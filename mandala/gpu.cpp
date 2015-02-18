//std
#include <iostream>
#include <fstream>

//mandala
#include "opengl.hpp"
#include "gpu.hpp"
#include "gpu_program.hpp"
#include "frame_buffer.hpp"
#include "texture.hpp"
#include "gpu_buffer.hpp"
#include "image.hpp"

//glm
#include <glm\gtc\type_ptr.hpp>

namespace mandala
{
	gpu_t gpu;

    inline GLenum get_buffer_target(gpu_t::buffer_target_e buffer_target)
    {
        switch (buffer_target)
        {
        case gpu_t::buffer_target_e::array:
            return GL_ARRAY_BUFFER;
        case gpu_t::buffer_target_e::atomic_counter:
            return GL_ATOMIC_COUNTER_BUFFER;
        case gpu_t::buffer_target_e::copy_read:
            return GL_COPY_READ_BUFFER;
        case gpu_t::buffer_target_e::copy_write:
            return GL_COPY_WRITE_BUFFER;
        case gpu_t::buffer_target_e::dispatch_indirect:
            return GL_DISPATCH_INDIRECT_BUFFER;
        case gpu_t::buffer_target_e::draw_indrect:
            return GL_DRAW_INDIRECT_BUFFER;
        case gpu_t::buffer_target_e::element_array:
            return GL_ELEMENT_ARRAY_BUFFER;
        case gpu_t::buffer_target_e::pixel_pack:
            return GL_PIXEL_PACK_BUFFER;
        case gpu_t::buffer_target_e::pixel_unpack:
            return GL_PIXEL_UNPACK_BUFFER;
        case gpu_t::buffer_target_e::query:
            return GL_QUERY_BUFFER;
        case gpu_t::buffer_target_e::shader_storage:
            return GL_SHADER_STORAGE_BUFFER;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_buffer_usage(gpu_t::buffer_usage_e buffer_usage)
    {
        switch (buffer_usage)
        {
        case gpu_t::buffer_usage_e::stream_draw:
            return GL_STREAM_DRAW;
        case gpu_t::buffer_usage_e::stream_read:
            return GL_STREAM_READ;
        case gpu_t::buffer_usage_e::stream_copy:
            return GL_STREAM_COPY;
        case gpu_t::buffer_usage_e::static_draw:
            return GL_STATIC_DRAW;
        case gpu_t::buffer_usage_e::static_read:
            return GL_STATIC_READ;
        case gpu_t::buffer_usage_e::static_copy:
            return GL_STATIC_COPY;
        case gpu_t::buffer_usage_e::dynamic_draw:
            return GL_DYNAMIC_DRAW;
        case gpu_t::buffer_usage_e::dynamic_read:
            return GL_DYNAMIC_READ;
        case gpu_t::buffer_usage_e::dynamic_copy:
            return GL_DYNAMIC_COPY;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_primitive_type(gpu_t::primitive_type_e primitive_type)
    {
        switch (primitive_type)
        {
        case gpu_t::primitive_type_e::points:
            return GL_POINTS;
        case gpu_t::primitive_type_e::lines:
            return GL_LINES;
        case gpu_t::primitive_type_e::line_loop:
            return GL_LINE_LOOP;
        case gpu_t::primitive_type_e::line_strip:
            return GL_LINE_STRIP;
        case gpu_t::primitive_type_e::triangles:
            return GL_TRIANGLES;
        case gpu_t::primitive_type_e::triangle_strip:
            return GL_TRIANGLE_STRIP;
        case gpu_t::primitive_type_e::triangle_fan:
            return GL_TRIANGLE_FAN;
        case gpu_t::primitive_type_e::quads:
            return GL_QUADS;
        case gpu_t::primitive_type_e::quad_strip:
            return GL_QUAD_STRIP;
        case gpu_t::primitive_type_e::polygon:
            return GL_POLYGON;
        default:
            throw std::invalid_argument("");
        }
    }

	inline GLenum get_blend_factor(gpu_t::blend_factor_e blend_factor)
	{
		switch (blend_factor)
		{
		case gpu_t::blend_factor_e::zero:
			return GL_ZERO;
		case gpu_t::blend_factor_e::one:
			return GL_ONE;
		case gpu_t::blend_factor_e::src_color:
			return GL_SRC_COLOR;
		case gpu_t::blend_factor_e::one_minus_src_color:
			return GL_ONE_MINUS_SRC_COLOR;
		case gpu_t::blend_factor_e::dst_color:
			return GL_DST_COLOR;
		case gpu_t::blend_factor_e::one_minus_dst_color:
			return GL_ONE_MINUS_DST_COLOR;
		case gpu_t::blend_factor_e::src_alpha:
			return GL_SRC_ALPHA;
		case gpu_t::blend_factor_e::one_minus_src_alpha:
			return GL_ONE_MINUS_SRC_ALPHA;
		case gpu_t::blend_factor_e::dst_alpha:
			return GL_DST_ALPHA;
		case gpu_t::blend_factor_e::one_minus_dst_alpha:
			return GL_ONE_MINUS_DST_ALPHA;
		case gpu_t::blend_factor_e::constant_color:
			return GL_CONSTANT_COLOR;
		case gpu_t::blend_factor_e::one_minus_constant_color:
			return GL_ONE_MINUS_CONSTANT_COLOR;
		case gpu_t::blend_factor_e::constant_alpha:
			return GL_CONSTANT_ALPHA;
		case gpu_t::blend_factor_e::one_minus_constant_alpha:
			return GL_ONE_MINUS_CONSTANT_ALPHA;
		case gpu_t::blend_factor_e::src_alpha_saturate:
			return GL_SRC_ALPHA_SATURATE;
		default:
			throw std::invalid_argument("");
		}
	}

	inline GLenum get_blend_equation(gpu_t::blend_equation_e blend_equation)
	{
		switch (blend_equation)
		{
		case gpu_t::blend_equation_e::add:
			return GL_FUNC_ADD;
		case gpu_t::blend_equation_e::subtract:
			return GL_FUNC_SUBTRACT;
		case gpu_t::blend_equation_e::subtract_reverse:
			return GL_FUNC_REVERSE_SUBTRACT;
		case gpu_t::blend_equation_e::min:
			return GL_MIN;
		case gpu_t::blend_equation_e::max:
			return GL_MAX;
		default:
			throw std::invalid_argument("");
		}
	}

	inline GLenum get_depth_function(gpu_t::depth_function_e depth_function)
	{
		switch (depth_function)
		{
		case gpu_t::depth_function_e::never:
			return GL_NEVER;
		case gpu_t::depth_function_e::less:
			return GL_LESS;
		case gpu_t::depth_function_e::equal:
			return GL_EQUAL;
		case gpu_t::depth_function_e::lequal:
			return GL_LEQUAL;
		case gpu_t::depth_function_e::greater:
			return GL_GREATER;
		case gpu_t::depth_function_e::notequal:
			return GL_NOTEQUAL;
		case gpu_t::depth_function_e::gequal:
			return GL_GEQUAL;
		case gpu_t::depth_function_e::always:
			return GL_ALWAYS;
		default:
			return GL_LESS;
		}
    }

    inline GLbitfield get_clear_flag_mask(gpu_clear_flag_type clear_flag)
    {
        switch (clear_flag)
        {
        case gpu_t::clear_flag_color:
            return GL_COLOR_BUFFER_BIT;
        case gpu_t::clear_flag_depth:
            return GL_DEPTH_BUFFER_BIT;
        case gpu_t::clear_flag_accum:
            return GL_ACCUM_BUFFER_BIT;
        case gpu_t::clear_flag_stencil:
            return GL_STENCIL_BUFFER_BIT;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_stencil_function(gpu_t::stencil_function_e stencil_function)
    {
        switch (stencil_function)
        {
        case gpu_t::stencil_function_e::always:
            return GL_ALWAYS;
        case gpu_t::stencil_function_e::gequal:
            return GL_GEQUAL;
        case gpu_t::stencil_function_e::greater:
            return GL_GREATER;
        case gpu_t::stencil_function_e::lequal:
            return GL_LEQUAL;
        case gpu_t::stencil_function_e::less:
            return GL_LESS;
        case gpu_t::stencil_function_e::never:
            return GL_NEVER;
        case gpu_t::stencil_function_e::notequal:
            return GL_NOTEQUAL;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_stencil_operation(gpu_t::stencil_operation_e stencil_operation)
    {
        switch (stencil_operation)
        {
        case gpu_t::stencil_operation_e::decr:
            return GL_DECR;
        case gpu_t::stencil_operation_e::decr_wrap:
            return GL_DECR_WRAP;
        case gpu_t::stencil_operation_e::incr:
            return GL_INCR;
        case gpu_t::stencil_operation_e::incr_wrap:
            return GL_INCR_WRAP;
        case gpu_t::stencil_operation_e::invert:
            return GL_INVERT;
        case gpu_t::stencil_operation_e::keep:
            return GL_KEEP;
        case gpu_t::stencil_operation_e::replace:
            return GL_REPLACE;
        case gpu_t::stencil_operation_e::zero:
            return GL_ZERO;
        default:
            throw std::invalid_argument("");
        }
    }

	inline GLenum get_data_type(gpu_data_type_e data_type)
	{
		switch (data_type)
		{
		case gpu_data_type_e::byte:
			return GL_BYTE;
		case gpu_data_type_e::unsigned_byte:
			return GL_UNSIGNED_BYTE;
		case gpu_data_type_e::short_:
			return GL_SHORT;
		case gpu_data_type_e::unsigned_short:
			return GL_UNSIGNED_SHORT;
		case gpu_data_type_e::int_:
			return GL_INT;
		case gpu_data_type_e::unsigned_int:
			return GL_UNSIGNED_INT;
		case gpu_data_type_e::float_:
			return GL_FLOAT;
		case gpu_data_type_e::double_:
			return GL_DOUBLE;
		default:
			throw std::invalid_argument("");
		}
	}

	inline GLenum get_shader_type(gpu_t::shader_type_e shader_type)
	{
		switch (shader_type)
		{
		case gpu_t::shader_type_e::fragment:
			return GL_FRAGMENT_SHADER;
		case gpu_t::shader_type_e::vertex:
			return GL_VERTEX_SHADER;
		default:
			throw std::invalid_argument("");
		}
	}

    void get_texture_formats(color_type_e color_type, texture_t::format_type& internal_format, texture_t::format_type& format, texture_t::type_type& type)
    {
        switch (color_type)
        {
        case color_type_e::g:
            format = GL_LUMINANCE;
            internal_format = 1;
            type = GL_UNSIGNED_BYTE;
            break;
        case color_type_e::rgb:
            format = GL_RGB;
            internal_format = GL_RGB;
            type = GL_UNSIGNED_BYTE;
            break;
        case color_type_e::rgba:
            format = GL_RGBA;
            internal_format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
        case color_type_e::ga:
            format = GL_LUMINANCE_ALPHA;
            internal_format = 2;
            type = GL_UNSIGNED_BYTE;
            break;
        case color_type_e::depth:
            format = GL_DEPTH_COMPONENT;
            internal_format = GL_DEPTH_COMPONENT;
            type = GL_UNSIGNED_BYTE;
            break;
        case color_type_e::depth_stencil:
            format = GL_DEPTH_STENCIL;
            internal_format = GL_DEPTH24_STENCIL8;
            type = GL_UNSIGNED_INT_24_8;
            break;
        default:
            throw std::exception();
        }
    }

    size_t get_bytes_per_pixel(color_type_e color_type)
    {
        switch (color_type)
        {
        case color_type_e::g:
            return 1;
        case color_type_e::rgb:
            return 3;
        case color_type_e::rgba:
            return 4;
        case color_type_e::ga:
            return 2;
        case color_type_e::depth:
            return 1;
        case color_type_e::depth_stencil:
            return 4;
        default:
            throw std::exception();
        }
    }

    void gpu_t::clear(const gpu_clear_flag_type clear_flags) const
    {
        GLbitfield clear_mask = 0;

        auto build_clear_mask = [&](const gpu_clear_flag_type clear_flag)
        {
            if ((clear_flags & clear_flag) == clear_flag)
            {
                clear_mask |= get_clear_flag_mask(clear_flag);
            }
        };

        build_clear_mask(clear_flag_color);
        build_clear_mask(clear_flag_depth);
        build_clear_mask(clear_flag_accum);
        build_clear_mask(clear_flag_stencil);

        glClear(clear_mask); glCheckError();
    }

	gpu_id_t gpu_t::create_buffer()
	{
		gpu_id_t id;

		glGenBuffers(1, &id); glCheckError();

		return id;
	}

	void gpu_t::destroy_buffer(gpu_id_t id)
	{
		glDeleteBuffers(1, &id); glCheckError();
	}

	gpu_id_t gpu_t::create_frame_buffer(gpu_frame_buffer_type_e type, const gpu_frame_buffer_size_type& size, std::shared_ptr<texture_t>& color_texture, std::shared_ptr<texture_t>& depth_stencil_texture, std::shared_ptr<texture_t>& depth_texture)
	{
		gpu_id_t id;

		glGenFramebuffers(1, &id); glCheckError();
		glBindFramebuffer(GL_FRAMEBUFFER, id); glCheckError();

		auto type_flags = static_cast<gpu_frame_buffer_type_flags_type>(type);

		//color
		if ((type_flags & gpu_frame_buffer_type_flag_color) == gpu_frame_buffer_type_flag_color)
		{
			color_texture = std::make_shared<texture_t>(color_type_e::rgb, size, nullptr);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture->get_id(), 0); glCheckError();
		}
		else
		{
			glDrawBuffer(GL_NONE); glCheckError();
			glReadBuffer(GL_NONE); glCheckError();
		}

		//depth & stencil
		if ((type_flags & (gpu_frame_buffer_type_flag_depth | gpu_frame_buffer_type_flag_stencil)) == (gpu_frame_buffer_type_flag_depth | gpu_frame_buffer_type_flag_stencil))
		{
			depth_stencil_texture = std::make_shared<texture_t>(color_type_e::depth_stencil, size, nullptr);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_texture->get_id(), 0); glCheckError();
		}
		//depth
		else if ((type_flags & gpu_frame_buffer_type_flag_depth) == gpu_frame_buffer_type_flag_depth)
		{
			depth_texture = std::make_shared<texture_t>(color_type_e::depth, size, nullptr);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture->get_id(), 0); glCheckError();
		}

		//restore previously bound frame buffer
		auto frame_buffer = gpu.frame_buffers.top();

		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer ? frame_buffer->lock()->get_id() : 0); glCheckError();

		return id;
	}

	void gpu_t::destroy_frame_buffer(gpu_id_t id)
	{
		glDeleteFramebuffers(1, &id);
	}

    gpu_id_t gpu_t::create_texture(color_type_e color_type, const vec2_u32_t& size, const void* data)
    {
        gpu_id_t id;

        glGenTextures(1, &id); glCheckError();
        glBindTexture(GL_TEXTURE_2D, id); glCheckError();

        GLint unpack_alignment;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment); glCheckError();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); glCheckError();

        texture_t::format_type internal_format, format;
        texture_t::type_type type;

        get_texture_formats(color_type, internal_format, format, type);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size.x, size.y, 0, format, type, data); glCheckError();
        glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment); glCheckError();
        glBindTexture(GL_TEXTURE_2D, 0); glCheckError();

        return id;
    }

    void gpu_t::resize_texture(const std::shared_ptr<texture_t>& texture, const vec2_u32_t& size)
    {
        texture_t::format_type internal_format, format;
        texture_t::type_type type;

        get_texture_formats(texture->get_color_type(), internal_format, format, type);

        glBindTexture(GL_TEXTURE_2D, texture->get_id()); glCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size.x, size.y, 0, format, type, nullptr); glCheckError();
        glBindTexture(GL_TEXTURE_2D, 0); glCheckError();
    }

    void gpu_t::destroy_texture(gpu_id_t id)
    {
        glDeleteTextures(1, &id); glCheckError();
    }

	gpu_location_t gpu_t::get_uniform_location(gpu_id_t program_id, const std::string & name)
	{
		auto uniform_location = glGetUniformLocation(program_id, name.c_str()); glCheckError();

		return uniform_location;
	}

	gpu_location_t gpu_t::get_attribute_location(gpu_id_t program_id, const std::string & name)
	{
		auto attribute_location = glGetAttribLocation(program_id, name.c_str()); glCheckError();

		return attribute_location;
	}

	void gpu_t::enable_vertex_attribute_array(gpu_location_t location)
	{
		glEnableVertexAttribArray(location); glCheckError();
	}

	void gpu_t::disable_vertex_attribute_array(gpu_location_t location)
	{
		glDisableVertexAttribArray(location); glCheckError();
	}

	void gpu_t::set_vertex_attrib_pointer(gpu_location_t location, int32_t size, gpu_data_type_e data_type, bool is_normalized, int32_t stride, const void* pointer)
	{
		glVertexAttribPointer(location, size, get_data_type(data_type), is_normalized, stride, pointer); glCheckError();
	}

	void gpu_t::set_vertex_attrib_pointer(gpu_location_t location, int32_t size, gpu_data_type_e data_type, int32_t stride, const void * pointer)
	{
		glVertexAttribIPointer(location, size, get_data_type(data_type), stride, pointer); glCheckError();
	}

	void gpu_t::set_uniform(gpu_location_t location, const mat3_t& value, bool should_transpose)
	{
		glUniformMatrix3fv(location, 1, should_transpose, glm::value_ptr(value)); glCheckError();
	}

	void gpu_t::set_uniform(gpu_location_t location, const mat4_t& value, bool should_transpose)
	{
		glUniformMatrix4fv(location, 1, should_transpose, glm::value_ptr(value)); glCheckError();
	}

	void gpu_t::set_uniform(gpu_location_t location, int32_t value)
	{
		glUniform1i(location, value); glCheckError();
	}

	void gpu_t::set_uniform(gpu_location_t location, uint32_t value)
	{
		glUniform1i(location, value); glCheckError();
	}

	void gpu_t::set_uniform(gpu_location_t location, float32_t value)
	{
		glUniform1f(location, value); glCheckError();
	}

	void gpu_t::set_uniform(gpu_location_t location, const vec2_t& value)
	{
		glUniform2fv(location, 1, glm::value_ptr(value)); glCheckError();
	}

	void gpu_t::set_uniform(gpu_location_t location, const vec3_t& value)
	{
		glUniform3fv(location, 1, glm::value_ptr(value)); glCheckError();
	}

	void gpu_t::set_uniform(gpu_location_t location, const vec4_t& value)
	{
		glUniform4fv(location, 1, glm::value_ptr(value)); glCheckError();
	}

	void gpu_t::set_uniform(gpu_location_t location, const std::vector<mat4_t>& matrices, bool should_transpose)
	{
		glUniformMatrix4fv(location, static_cast<GLsizei>(matrices.size()), false, reinterpret_cast<const float*>(matrices.data())); glCheckError();
	}

	void gpu_t::set_uniform_subroutine(shader_type_e shader_type, gpu_index_t index)
	{
		glUniformSubroutinesuiv(get_shader_type(shader_type), 1, &index); glCheckError();
	}

	void gpu_t::set_clear_color(rgba_type & color)
	{
		glClearColor(color.r, color.g, color.b, color.a); glCheckError();
	}

	rgba_type gpu_t::get_clear_color()
	{
		rgba_type clear_color;

		glGetFloatv(GL_COLOR_CLEAR_VALUE, glm::value_ptr(clear_color)); glCheckError();

		return clear_color;
	}

	gpu_location_t gpu_t::get_subroutine_uniform_location(gpu_id_t program_id, shader_type_e shader_type, const std::string& name)
	{
		auto location = glGetSubroutineUniformLocation(program_id, get_shader_type(shader_type), name.c_str()); glCheckError();

		return location;
	}

	gpu_index_t gpu_t::get_subroutine_index(gpu_id_t program_id, shader_type_e shader_type, const std::string& name)
	{
		auto index = glGetSubroutineIndex(program_id, get_shader_type(shader_type), name.c_str()); glCheckError();

		return index;
	}

    boost::optional<gpu_t::program_mgr_t::weak_type> gpu_t::program_mgr_t::top() const
	{
		boost::optional<weak_type> gpu_program;

        if (!programs.empty())
		{
			gpu_program = programs.top();
		}

		return gpu_program;
	}

    void gpu_t::program_mgr_t::push(const gpu_t::program_mgr_t::weak_type& program)
    {
        if (programs.empty() || program.lock() != programs.top().lock())
		{
            glUseProgram(program.lock()->get_id()); glCheckError();
		}

        programs.push(program);

		program.lock()->on_bind();
	}

    gpu_t::program_mgr_t::weak_type gpu_t::program_mgr_t::pop()
	{
		if (programs.empty())
		{
			throw std::exception();
		}

		auto previous_program = programs.top();

		programs.pop();

		previous_program.lock()->on_unbind();

        if (programs.empty())
		{
            glUseProgram(0); glCheckError();

            return weak_type();
		}
		else
		{
            glUseProgram(programs.top().lock()->get_id()); glCheckError();
            
            return programs.top();
		}
	}

	boost::optional<gpu_t::frame_buffer_mgr_t::weak_type> gpu_t::frame_buffer_mgr_t::top() const
    {
		boost::optional<weak_type> frame_buffer;

        if (!frame_buffers.empty())
        {
			frame_buffer = frame_buffers.top();
        }

		return frame_buffer;
    }

    void gpu_t::frame_buffer_mgr_t::push(const gpu_t::frame_buffer_mgr_t::shared_type& frame_buffer)
    {
        if (frame_buffers.size() == 0 || frame_buffer != frame_buffers.top())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->get_id()); glCheckError();

            frame_buffer->on_bind();
        }

        frame_buffers.push(frame_buffer);
    }

    gpu_t::frame_buffer_mgr_t::weak_type gpu_t::frame_buffer_mgr_t::pop()
    {
		if (frame_buffers.empty())
        {
			throw std::out_of_range("");
        }

        frame_buffers.pop();

        if (frame_buffers.empty())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0); glCheckError();
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); glCheckError();
            glDepthMask(GL_TRUE); glCheckError();
            //TODO: stencil mask

            return weak_type();
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffers.top()->get_id()); glCheckError();

            frame_buffers.top()->on_bind();

            return frame_buffers.top();
        }
    }

    gpu_t::texture_mgr_t::weak_type gpu_t::texture_mgr_t::get(index_type index) const
    {
        if (index < 0 || index >= texture_count)
        {
            throw std::out_of_range("");
        }

        return textures[index];
    }

    gpu_t::texture_mgr_t::weak_type gpu_t::texture_mgr_t::bind(index_type index, const shared_type& texture)
    {
        const auto& previous_texture = textures[index];

        if (previous_texture == texture)
        {
            return previous_texture;
        }

        glActiveTexture(GL_TEXTURE0 + index); glCheckError();
        glBindTexture(GL_TEXTURE_2D, texture != nullptr ? texture->get_id() : 0); glCheckError();

        textures[index] = texture;

        return previous_texture;
    }

    gpu_t::texture_mgr_t::weak_type gpu_t::texture_mgr_t::unbind(index_type index)
    {
        const auto& previous_texture = textures[index];

        if (previous_texture == nullptr)
        {
            return previous_texture;
        }

        glActiveTexture(GL_TEXTURE0 + index); glCheckError();
        glBindTexture(GL_TEXTURE_2D, 0); glCheckError();

        textures[index] = nullptr;

        return previous_texture;
    }

	gpu_viewport_type gpu_t::viewport_mgr_t::top() const
    {
        if (viewports.empty())
        {
			vec4_i32_t viewport;
            glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport)); glCheckError();

			return gpu_viewport_type(viewport.x, viewport.y, viewport.z, viewport.w);
        }

        return viewports.top();
    }

    void gpu_t::viewport_mgr_t::push(const gpu_viewport_type& viewport)
    {
        viewports.push(viewport);

        glViewport(viewport.x, viewport.y, viewport.width, viewport.height); glCheckError();
    }

    gpu_viewport_type gpu_t::viewport_mgr_t::pop()
    {
        if (viewports.empty())
        {
            throw std::exception();
        }

		const auto& previous_viewport = viewports.top();

		viewports.pop();

		const auto top_viewport = top();

		glViewport(top_viewport.x, top_viewport.y, top_viewport.width, top_viewport.height); glCheckError();

		return previous_viewport;
    }

    void gpu_t::buffer_mgr_t::put(buffer_type & buffer)
    {
        buffers.insert(buffer);
    }

    void gpu_t::buffer_mgr_t::erase(buffer_type & buffer)
    {
        buffers.erase(buffer);
    }
    
    void gpu_t::buffer_mgr_t::push(buffer_target_e target, buffer_type buffer)
    {
        if (target_buffers.find(target) == target_buffers.end())
        {
            target_buffers.emplace(std::make_pair(target, std::stack<buffer_type>()));
        }

        target_buffers[target].push(buffer);

        glBindBuffer(get_buffer_target(target), buffer->get_id()); glCheckError();
    }

    gpu_t::buffer_mgr_t::buffer_type gpu_t::buffer_mgr_t::pop(buffer_target_e target)
    {
        auto target_buffers_itr = target_buffers.find(target);

        if (target_buffers_itr == target_buffers.end())
        {
            throw std::out_of_range("");
        }

        auto& buffers = target_buffers_itr->second;

        if (buffers.empty())
        {
            throw std::out_of_range("");
        }

        buffers.pop();

        glBindBuffer(get_buffer_target(target), buffers.empty() ? 0 : buffers.top()->get_id()); glCheckError();

        return buffers.empty() ? buffer_type() : buffers.top();
    }


    gpu_t::buffer_mgr_t::buffer_type gpu_t::buffer_mgr_t::top(buffer_target_e target) const
    {
        auto target_buffers_itr = target_buffers.find(target);

        if (target_buffers_itr == target_buffers.end())
        {
            throw std::out_of_range("");
        }

        const auto& buffers = target_buffers_itr->second;

        if (buffers.empty())
        {
            throw std::out_of_range("");
        }

        return buffers.top();
    }

    void gpu_t::buffer_mgr_t::data(buffer_target_e target, const void* data, size_t size, buffer_usage_e usage)
    {
        glBufferData(get_buffer_target(target), size, data, get_buffer_usage(usage)); glCheckError();
    }
    
    //TODO: infer index_data_type from bound index buffer
    void gpu_t::draw_elements(primitive_type_e primitive_type, size_t count, gpu_data_type_e index_data_type, size_t offset) const
    {
        glDrawElements(get_primitive_type(primitive_type), count, get_data_type(index_data_type), reinterpret_cast<GLvoid*>(offset)); glCheckError();
    }

	gpu_id_t gpu_t::create_program(const std::string& vertex_shader_source, const std::string& fragment_shader_source) const
	{
		auto create_shader = [&](GLenum type, const std::string& source) -> GLint
		{
			//create shader
			gpu_id_t id = glCreateShader(type); glCheckError();

			auto strings = source.c_str();
			GLint lengths[1] = { static_cast<GLint>(source.length()) };

			//set shader source
			glShaderSource(id, 1, &strings, &lengths[0]); glCheckError();

			//compile shader
			glCompileShader(id); glCheckError();

			GLsizei compile_status;
			glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status); glCheckError();

			if (compile_status == GL_FALSE)
			{
#if defined(DEBUG)
				GLsizei shader_info_log_length = 0;
				GLchar shader_info_log[GL_INFO_LOG_LENGTH] = { '\0' };

				glGetShaderInfoLog(id, GL_INFO_LOG_LENGTH, &shader_info_log_length, &shader_info_log[0]); glCheckError();

				std::cerr << shader_info_log << std::endl;
#endif
				glDeleteShader(id); glCheckError();

				throw std::exception();
			}

			return id;
		};

		const auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
		const auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

		//create program
		const auto id = glCreateProgram(); glCheckError();
		
		//attach shaders
		glAttachShader(id, vertex_shader); glCheckError();
		glAttachShader(id, fragment_shader); glCheckError();

		//link program
		glLinkProgram(id); glCheckError();

		//link status
		GLint link_status;
		glGetProgramiv(id, GL_LINK_STATUS, &link_status); glCheckError();

		if (link_status == GL_FALSE)
		{
			GLint program_info_log_length = 0;

			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &program_info_log_length); glCheckError();

			if (program_info_log_length > 0)
			{
				std::string program_info_log;
				program_info_log.resize(program_info_log_length);

				glGetProgramInfoLog(id, program_info_log_length, nullptr, &program_info_log[0]); glCheckError();

				std::cout << program_info_log << std::endl;
			}

			glDeleteProgram(id); glCheckError();

			throw std::exception();
		}

		glDetachShader(id, vertex_shader); glCheckError();
		glDetachShader(id, fragment_shader); glCheckError();

		glDeleteShader(vertex_shader); glCheckError();
		glDeleteShader(fragment_shader); glCheckError();

        //save compiled program binary
        GLsizei binary_length = 0;

        glGetProgramiv(id, GL_PROGRAM_BINARY_LENGTH, &binary_length); glCheckError();

        GLenum binary_format = 0;
        std::vector<uint8_t> program_binary_data(binary_length);

        glGetProgramBinary(id, binary_length, &binary_length, &binary_format, static_cast<GLvoid*>(program_binary_data.data())); glCheckError();

        auto s = std::ofstream("program.cgp", std::ios::binary);
        s.write(reinterpret_cast<char*>(program_binary_data.data()), program_binary_data.size());
        s.close();

		return id;
	}

	void gpu_t::destroy_program(gpu_id_t id)
	{
        glDeleteProgram(id); glCheckError();
	}

    //blend
    gpu_t::blend_t::state_t gpu_t::blend_t::get_state() const
	{
		if (!states.empty())
		{
			return states.top();
		}

		return state_t();
    }

    void gpu_t::blend_t::push_state(const gpu_t::blend_t::state_t& state)
    {
        apply_state(state);

        states.push(state);
    }

    void gpu_t::blend_t::pop_state()
	{
		states.pop();

        apply_state(get_state());
	}

    void gpu_t::blend_t::apply_state(const gpu_t::blend_t::state_t& state)
	{
		if (state.is_enabled)
		{
			glEnable(GL_BLEND); glCheckError();
		}
		else
		{
			glDisable(GL_BLEND); glCheckError();
		}

		glBlendFunc(get_blend_factor(state.src_factor), get_blend_factor(state.dst_factor)); glCheckError();
		glBlendEquation(get_blend_equation(state.equation)); glCheckError();
	}

    //depth
    gpu_t::depth_t::state_t gpu_t::depth_t::get_state() const
    {
        if (!states.empty())
        {
            return states.top();
        }

        return state_t();
    }

    void gpu_t::depth_t::push_state(const state_t& state)
	{
        apply_state(state);

		states.push(state);
    }

    void gpu_t::depth_t::pop_state()
    {
        states.pop();

        apply_state(get_state());
    }

    void gpu_t::depth_t::apply_state(const state_t& state)
    {
        if (state.should_test)
        {
            glEnable(GL_DEPTH_TEST); glCheckError();
        }
        else
        {
            glDisable(GL_DEPTH_TEST); glCheckError();
        }

        glDepthMask(state.should_write_mask ? GL_TRUE : GL_FALSE); glCheckError();
        glDepthFunc(get_depth_function(state.function)); glCheckError();
    }

    //culling
    gpu_t::culling_t::state_t gpu_t::culling_t::get_state() const
    {
        if (!states.empty())
        {
            return states.top();
        }

        return state_t();
    }

    void gpu_t::culling_t::push_state(const state_t& state)
    {
        apply_state(state);

        states.push(state);
    }

    void gpu_t::culling_t::pop_state()
    {
        states.pop();

        apply_state(get_state());
    }

    void gpu_t::culling_t::apply_state(const state_t& state)
    {
        if (state.is_enabled)
        {
            glEnable(GL_CULL_FACE); glCheckError();
        }
        else
        {
            glDisable(GL_CULL_FACE); glCheckError();
        }

        switch (state.front_face)
        {
        case culling_front_face_e::ccw:
            glFrontFace(GL_CCW); glCheckError();
            break;
        case culling_front_face_e::cw:
            glFrontFace(GL_CW); glCheckError();
            break;
        }
        
        switch (state.mode)
        {
        case culling_mode_e::back:
            glCullFace(GL_BACK); glCheckError();
            break;
        case culling_mode_e::front:
            glCullFace(GL_FRONT); glCheckError();
            break;
        case culling_mode_e::front_and_back:
            glCullFace(GL_FRONT_AND_BACK); glCheckError();
            break;
        }
    }

    //stencil
    gpu_t::stencil_t::state_t gpu_t::stencil_t::get_state() const
    {
        if (!states.empty())
        {
            return states.top();
        }

        return state_t();
    }

    void gpu_t::stencil_t::push_state(const state_t& state)
    {
        apply_state(state);

        states.push(state);
    }

    void gpu_t::stencil_t::pop_state()
    {
        states.pop();

        apply_state(get_state());
    }

    void gpu_t::stencil_t::apply_state(const state_t& state)
    {
        if (state.is_enabled)
        {
            glEnable(GL_STENCIL_TEST); glCheckError();
        }
        else
        {
            glDisable(GL_STENCIL_TEST); glCheckError();
        }

        glStencilFunc(get_stencil_function(state.function.func), state.function.mask, state.function.ref); glCheckError();
        glStencilOp(get_stencil_operation(state.operations.fail), get_stencil_operation(state.operations.zfail), get_stencil_operation(state.operations.zpass)); glCheckError();
        glStencilMask(state.mask); glCheckError();
    }

    //color
    gpu_t::color_t::state_t gpu_t::color_t::get_state() const
    {
        if (!states.empty())
        {
            return states.top();
        }

        return state_t();
    }

    void gpu_t::color_t::push_state(const state_t& state)
    {
        apply_state(state);

        states.push(state);
    }

    void gpu_t::color_t::pop_state()
    {
        states.pop();

        apply_state(get_state());
    }

    void gpu_t::color_t::apply_state(const state_t& state)
    {
        glColorMask(state.mask.r ? GL_TRUE : GL_FALSE,
                    state.mask.g ? GL_TRUE : GL_FALSE, 
                    state.mask.b ? GL_TRUE : GL_FALSE, 
                    state.mask.a ? GL_TRUE : GL_FALSE); glCheckError();
    }

    const std::string& gpu_t::get_vendor() const
    {
        static const std::string vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

        return vendor;
    }

    const std::string& gpu_t::get_renderer() const
    {
        static const std::string renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

        return renderer;
    }

    const std::string& gpu_t::get_version() const
    {
        static const std::string version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

        return version;
    }

    const std::string& gpu_t::get_shading_language_version() const
    {
        static const std::string shading_language_verison = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

        return shading_language_verison;
    }

    const std::string& gpu_t::get_extensions() const
    {
        static const std::string extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

        return extensions;
    }

    void gpu_t::get_texture_data(const std::shared_ptr<texture_t>& texture, std::vector<uint8_t>& data, int32_t level)
    {
        int32_t internal_format, format, type;

        get_texture_formats(texture->get_color_type(), internal_format, format, type);

        const auto bytes_per_pixel = get_bytes_per_pixel(texture->get_color_type());

        data.resize(texture->get_size().x * texture->get_size().y * bytes_per_pixel);

        textures.bind(0, texture);

        glGetTexImage(GL_TEXTURE_2D, level, format, type, data.data());

        textures.unbind(0);
    }
}