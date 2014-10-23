//mandala
#include "opengl.hpp"
#include "gpu.hpp"
#include "gpu_program.hpp"
#include "frame_buffer.hpp"
#include "texture.hpp"
#include "gpu_buffer.hpp"

//glm
#include <glm\gtc\type_ptr.hpp>

//std
#include <iostream>

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

    inline GLenum get_index_type(gpu_t::index_type_e index_type)
    {
        switch (index_type)
        {
        case gpu_t::index_type_e::unsigned_byte:
            return GL_UNSIGNED_BYTE;
        case gpu_t::index_type_e::unsigned_short:
            return GL_UNSIGNED_SHORT;
        case gpu_t::index_type_e::unsigned_int:
            return GL_UNSIGNED_INT;
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

    inline GLbitfield get_clear_flag_mask(gpu_t::clear_flag_type clear_flag)
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

    void gpu_t::clear(const clear_flag_type clear_flags) const
    {
        GLbitfield clear_mask = 0;

        auto build_clear_mask = [&](const clear_flag_type clear_flag)
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
            glUseProgram(program.lock()->id); glCheckError();
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
            glUseProgram(programs.top().lock()->id); glCheckError();
            
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
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->id); glCheckError();

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
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffers.top()->id); glCheckError();

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
        glBindTexture(GL_TEXTURE_2D, texture != nullptr ? texture->id : 0); glCheckError();

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

    gpu_t::viewport_type gpu_t::viewport_mgr_t::top() const
    {
        if (viewports.empty())
        {
			vec4_i32_t viewport;
            glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport)); glCheckError();

			return viewport_type(viewport.x, viewport.y, viewport.z, viewport.w);
        }

        return viewports.top();
    }

    void gpu_t::viewport_mgr_t::push(const viewport_type& viewport)
    {
        viewports.push(viewport);

        glViewport(viewport.x, viewport.y, viewport.width, viewport.height); glCheckError();
    }

    gpu_t::viewport_type gpu_t::viewport_mgr_t::pop()
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

    void gpu_t::buffer_mgr_t::push(buffer_target_e target, buffer_type buffer)
    {
        if (target_buffers.find(target) == target_buffers.end())
        {
            target_buffers.emplace(std::make_pair(target, std::stack<buffer_type>()));
        }

        target_buffers[target].push(buffer);

        glBindBuffer(get_buffer_target(target), buffer->id); glCheckError();
    }

    gpu_t::buffer_mgr_t::buffer_type gpu_t::buffer_mgr_t::pop(buffer_target_e target)
    {
        if (target_buffers.find(target) == target_buffers.end())
        {
            throw std::out_of_range("");
        }

        auto& buffers = target_buffers[target];

        if (buffers.empty())
        {
            throw std::out_of_range("");
        }

        buffers.pop();

        glBindBuffer(get_buffer_target(target), buffers.empty() ? 0 : buffers.top()->id); glCheckError();

        return buffers.empty() ? buffer_type() : buffers.top();
    }

    void gpu_t::buffer_mgr_t::data(buffer_target_e target, void* data, size_t size, buffer_usage_e usage)
    {
        glBufferData(get_buffer_target(target), size, data, get_buffer_usage(usage)); glCheckError();
    }

    void gpu_t::draw_elements(primitive_type_e primitive_type, size_t count, index_type_e index_type, size_t offset) const
    {
        glDrawElements(get_primitive_type(primitive_type), count, get_index_type(index_type), reinterpret_cast<GLvoid*>(offset)); glCheckError();
    }

	uint32_t gpu_t::create_program(const std::string& vertex_shader_source, const std::string& fragment_shader_source) const
	{
		auto create_shader = [&](GLenum type, const std::string& source) -> GLint
		{
			//create shader
			auto id = glCreateShader(type); glCheckError();

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
#ifdef _DEBUG
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

		return id;
	}

	void gpu_t::destroy_program(uint32_t id)
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
}
