//std
#include <iostream>
#include <fstream>
#include <sstream>

//boost
#include <boost\make_shared.hpp>
#include <boost\crc.hpp>
#include <boost\filesystem.hpp>

//naga
#include "opengl.hpp"
#include "gpu.hpp"
#include "gpu_program.hpp"
#include "frame_buffer.hpp"
#include "texture.hpp"
#include "gpu_buffer.hpp"
#include "image.hpp"
#include "cache.hpp"
#include "io.hpp"

//glm
#include <glm\gtc\type_ptr.hpp>

namespace naga
{
    Gpu gpu;

    inline GLenum get_buffer_target(Gpu::BufferTarget buffer_target)
    {
        switch (buffer_target)
        {
        case Gpu::BufferTarget::ARRAY:
            return GL_ARRAY_BUFFER;
        case Gpu::BufferTarget::ATOMIC_COUNTER:
            return GL_ATOMIC_COUNTER_BUFFER;
        case Gpu::BufferTarget::COPY_READ:
            return GL_COPY_READ_BUFFER;
        case Gpu::BufferTarget::COPY_WRITE:
            return GL_COPY_WRITE_BUFFER;
        case Gpu::BufferTarget::DISPATCH_INDIRECT:
            return GL_DISPATCH_INDIRECT_BUFFER;
        case Gpu::BufferTarget::DRAW_INDRECT:
            return GL_DRAW_INDIRECT_BUFFER;
        case Gpu::BufferTarget::ELEMENT_ARRAY:
            return GL_ELEMENT_ARRAY_BUFFER;
        case Gpu::BufferTarget::PIXEL_PACK:
            return GL_PIXEL_PACK_BUFFER;
        case Gpu::BufferTarget::PIXEL_UNPACK:
            return GL_PIXEL_UNPACK_BUFFER;
        case Gpu::BufferTarget::QUERY:
            return GL_QUERY_BUFFER;
        case Gpu::BufferTarget::SHADER_STORAGE:
            return GL_SHADER_STORAGE_BUFFER;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_buffer_usage(Gpu::BufferUsage buffer_usage)
    {
        switch (buffer_usage)
        {
        case Gpu::BufferUsage::STREAM_DRAW:
            return GL_STREAM_DRAW;
        case Gpu::BufferUsage::STREAM_READ:
            return GL_STREAM_READ;
        case Gpu::BufferUsage::STREAM_COPY:
            return GL_STREAM_COPY;
        case Gpu::BufferUsage::STATIC_DRAW:
            return GL_STATIC_DRAW;
        case Gpu::BufferUsage::STATIC_READ:
            return GL_STATIC_READ;
        case Gpu::BufferUsage::STATIC_COPY:
            return GL_STATIC_COPY;
        case Gpu::BufferUsage::DYNAMIC_DRAW:
            return GL_DYNAMIC_DRAW;
        case Gpu::BufferUsage::DYNAMIC_READ:
            return GL_DYNAMIC_READ;
        case Gpu::BufferUsage::DYNAMIC_COPY:
            return GL_DYNAMIC_COPY;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_primitive_type(Gpu::PrimitiveType primitive_type)
    {
        switch (primitive_type)
        {
        case Gpu::PrimitiveType::POINTS:
            return GL_POINTS;
        case Gpu::PrimitiveType::LINES:
            return GL_LINES;
        case Gpu::PrimitiveType::LINE_LOOP:
            return GL_LINE_LOOP;
        case Gpu::PrimitiveType::LINE_STRIP:
            return GL_LINE_STRIP;
        case Gpu::PrimitiveType::TRIANGLES:
            return GL_TRIANGLES;
        case Gpu::PrimitiveType::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case Gpu::PrimitiveType::TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        case Gpu::PrimitiveType::QUADS:
            return GL_QUADS;
        case Gpu::PrimitiveType::QUAD_STRIP:
            return GL_QUAD_STRIP;
        case Gpu::PrimitiveType::POLYGON:
            return GL_POLYGON;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_blend_factor(Gpu::BlendFactor blend_factor)
    {
        switch (blend_factor)
        {
        case Gpu::BlendFactor::ZERO:
            return GL_ZERO;
        case Gpu::BlendFactor::ONE:
            return GL_ONE;
        case Gpu::BlendFactor::SRC_COLOR:
            return GL_SRC_COLOR;
        case Gpu::BlendFactor::ONE_MINUS_SRC_COLOR:
            return GL_ONE_MINUS_SRC_COLOR;
        case Gpu::BlendFactor::DST_COLOR:
            return GL_DST_COLOR;
        case Gpu::BlendFactor::ONE_MINUS_DST_COLOR:
            return GL_ONE_MINUS_DST_COLOR;
        case Gpu::BlendFactor::SRC_ALPHA:
            return GL_SRC_ALPHA;
        case Gpu::BlendFactor::ONE_MINUS_SRC_ALPHA:
            return GL_ONE_MINUS_SRC_ALPHA;
        case Gpu::BlendFactor::DST_ALPHA:
            return GL_DST_ALPHA;
        case Gpu::BlendFactor::ONE_MINUS_DST_ALPHA:
            return GL_ONE_MINUS_DST_ALPHA;
        case Gpu::BlendFactor::CONSTANT_COLOR:
            return GL_CONSTANT_COLOR;
        case Gpu::BlendFactor::ONE_MINUS_CONSTANT_COLOR:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case Gpu::BlendFactor::CONSTANT_ALPHA:
            return GL_CONSTANT_ALPHA;
        case Gpu::BlendFactor::ONE_MINUS_CONSTANT_ALPHA:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        case Gpu::BlendFactor::SRC_ALPHA_SATURATE:
            return GL_SRC_ALPHA_SATURATE;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_blend_equation(Gpu::BlendEquation blend_equation)
    {
        switch (blend_equation)
        {
        case Gpu::BlendEquation::ADD:
            return GL_FUNC_ADD;
        case Gpu::BlendEquation::SUBTRACT:
            return GL_FUNC_SUBTRACT;
        case Gpu::BlendEquation::SUBTRACT_REVERSE:
            return GL_FUNC_REVERSE_SUBTRACT;
        case Gpu::BlendEquation::MIN:
            return GL_MIN;
        case Gpu::BlendEquation::MAX:
            return GL_MAX;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_depth_function(Gpu::DepthFunction depth_function)
    {
        switch (depth_function)
        {
        case Gpu::DepthFunction::NEVER:
            return GL_NEVER;
        case Gpu::DepthFunction::LESS:
            return GL_LESS;
        case Gpu::DepthFunction::EQUAL:
            return GL_EQUAL;
        case Gpu::DepthFunction::LEQUAL:
            return GL_LEQUAL;
        case Gpu::DepthFunction::GREATER:
            return GL_GREATER;
        case Gpu::DepthFunction::NOTEQUAL:
            return GL_NOTEQUAL;
        case Gpu::DepthFunction::GEQUAL:
            return GL_GEQUAL;
        case Gpu::DepthFunction::ALWAYS:
            return GL_ALWAYS;
        default:
            return GL_LESS;
        }
    }

    inline GLbitfield get_clear_flag_mask(GpuClearFlagType clear_flag)
    {
        switch (clear_flag)
        {
        case Gpu::CLEAR_FLAG_COLOR:
            return GL_COLOR_BUFFER_BIT;
        case Gpu::CLEAR_FLAG_DEPTH:
            return GL_DEPTH_BUFFER_BIT;
        case Gpu::CLEAR_FLAG_ACCUM:
            return GL_ACCUM_BUFFER_BIT;
        case Gpu::CLEAR_FLAG_STENCIL:
            return GL_STENCIL_BUFFER_BIT;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_stencil_function(Gpu::StencilFunction stencil_function)
    {
        switch (stencil_function)
        {
        case Gpu::StencilFunction::ALWAYS:
            return GL_ALWAYS;
        case Gpu::StencilFunction::EQUAL:
            return GL_EQUAL;
        case Gpu::StencilFunction::GEQUAL:
            return GL_GEQUAL;
        case Gpu::StencilFunction::GREATER:
            return GL_GREATER;
        case Gpu::StencilFunction::LEQUAL:
            return GL_LEQUAL;
        case Gpu::StencilFunction::LESS:
            return GL_LESS;
        case Gpu::StencilFunction::NEVER:
            return GL_NEVER;
        case Gpu::StencilFunction::NOTEQUAL:
            return GL_NOTEQUAL;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_stencil_operation(Gpu::StencilOperation stencil_operation)
    {
        switch (stencil_operation)
        {
        case Gpu::StencilOperation::DECR:
            return GL_DECR;
        case Gpu::StencilOperation::DECR_WRAP:
            return GL_DECR_WRAP;
        case Gpu::StencilOperation::INCR:
            return GL_INCR;
        case Gpu::StencilOperation::INCR_WRAP:
            return GL_INCR_WRAP;
        case Gpu::StencilOperation::INVERT:
            return GL_INVERT;
        case Gpu::StencilOperation::KEEP:
            return GL_KEEP;
        case Gpu::StencilOperation::REPLACE:
            return GL_REPLACE;
        case Gpu::StencilOperation::ZERO:
            return GL_ZERO;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_data_type(GpuDataTypes data_type)
    {
        switch (data_type)
        {
		case GpuDataTypes::BYTE:
            return GL_BYTE;
		case GpuDataTypes::UNSIGNED_BYTE:
            return GL_UNSIGNED_BYTE;
		case GpuDataTypes::SHORT:
            return GL_SHORT;
		case GpuDataTypes::UNSIGNED_SHORT:
            return GL_UNSIGNED_SHORT;
		case GpuDataTypes::INT:
            return GL_INT;
		case GpuDataTypes::UNSIGNED_INT:
            return GL_UNSIGNED_INT;
		case GpuDataTypes::FLOAT:
            return GL_FLOAT;
		case GpuDataTypes::DOUBLE:
            return GL_DOUBLE;
        default:
            throw std::invalid_argument("");
        }
    }

    inline GLenum get_shader_type(Gpu::ShaderType shader_type)
    {
        switch (shader_type)
        {
		case Gpu::ShaderType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
		case Gpu::ShaderType::VERTEX:
            return GL_VERTEX_SHADER;
        default:
            throw std::invalid_argument("");
        }
    }

	void get_texture_formats(ColorType color_type, Texture::FormatType& internal_format, Texture::FormatType& format, Texture::TypeType& type)
    {
        switch (color_type)
        {
		case ColorType::G:
            format = GL_LUMINANCE;
            internal_format = 1;
            type = GL_UNSIGNED_BYTE;
            break;
		case ColorType::RGB:
            format = GL_RGB;
            internal_format = GL_RGB;
            type = GL_UNSIGNED_BYTE;
            break;
		case ColorType::RGBA:
            format = GL_RGBA;
            internal_format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            break;
		case ColorType::GA:
            format = GL_LUMINANCE_ALPHA;
            internal_format = 2;
            type = GL_UNSIGNED_BYTE;
            break;
		case ColorType::DEPTH:
            format = GL_DEPTH_COMPONENT;
            internal_format = GL_DEPTH_COMPONENT;
            type = GL_UNSIGNED_BYTE;
            break;
		case ColorType::DEPTH_STENCIL:
            format = GL_DEPTH_STENCIL;
            internal_format = GL_DEPTH24_STENCIL8;
            type = GL_UNSIGNED_INT_24_8;
            break;
        default:
            throw std::exception();
        }
    }

    size_t get_bytes_per_pixel(ColorType color_type)
    {
        switch (color_type)
        {
		case ColorType::G:
            return 1;
		case ColorType::RGB:
            return 3;
		case ColorType::RGBA:
            return 4;
		case ColorType::GA:
            return 2;
		case ColorType::DEPTH:
            return 1;
		case ColorType::DEPTH_STENCIL:
            return 4;
        default:
            throw std::exception();
        }
    }

    void Gpu::clear(const GpuClearFlagType clear_flags) const
    {
        GLbitfield clear_mask = 0;

        auto build_clear_mask = [&](const GpuClearFlagType clear_flag)
        {
            if ((clear_flags & clear_flag) == clear_flag)
            {
                clear_mask |= get_clear_flag_mask(clear_flag);
            }
        };

        build_clear_mask(CLEAR_FLAG_COLOR);
        build_clear_mask(CLEAR_FLAG_DEPTH);
        build_clear_mask(CLEAR_FLAG_ACCUM);
        build_clear_mask(CLEAR_FLAG_STENCIL);

        glClear(clear_mask); glCheckError();
    }

    GpuId Gpu::create_buffer()
    {
        GpuId id;

        glGenBuffers(1, &id); glCheckError();

        return id;
    }

    void Gpu::destroy_buffer(GpuId id)
    {
        glDeleteBuffers(1, &id); glCheckError();
    }

	GpuId Gpu::create_frame_buffer(GpuFrameBufferType type, const GpuFrameBufferSizeType& size, boost::shared_ptr<Texture>& color_texture, boost::shared_ptr<Texture>& depth_stencil_texture, boost::shared_ptr<Texture>& depth_texture)
    {
        GpuId id;

        glGenFramebuffers(1, &id); glCheckError();
        glBindFramebuffer(GL_FRAMEBUFFER, id); glCheckError();

        auto type_flags = static_cast<GpuFrameBufferTypeFlagsType>(type);

        //color
		if ((type_flags & GPU_FRAME_BUFFER_TYPE_FLAG_COLOR) == GPU_FRAME_BUFFER_TYPE_FLAG_COLOR)
        {
			color_texture = boost::make_shared<Texture>(ColorType::RGB, size, nullptr);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture->get_id(), 0); glCheckError();
        }
        else
        {
            glDrawBuffer(GL_NONE); glCheckError();
            glReadBuffer(GL_NONE); glCheckError();
        }

        //depth & stencil
		if ((type_flags & (GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH | GPU_FRAME_BUFFER_TYPE_FLAG_STENCIL)) == (GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH | GPU_FRAME_BUFFER_TYPE_FLAG_STENCIL))
        {
            depth_stencil_texture = boost::make_shared<Texture>(ColorType::DEPTH_STENCIL, size, nullptr);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_texture->get_id(), 0); glCheckError();
        }
        //depth
		else if ((type_flags & GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH) == GPU_FRAME_BUFFER_TYPE_FLAG_DEPTH)
        {
			depth_texture = boost::make_shared<Texture>(ColorType::DEPTH, size, nullptr);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture->get_id(), 0); glCheckError();
        }

        //restore previously bound frame buffer
        auto frame_buffer = gpu.frame_buffers.top();

        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer ? frame_buffer->lock()->get_id() : 0); glCheckError();

        return id;
    }

    void Gpu::destroy_frame_buffer(GpuId id)
    {
        glDeleteFramebuffers(1, &id);
    }

    GpuId Gpu::create_texture(ColorType color_type, vec2_u32 size, const void* data)
    {
        size = glm::max(vec2_u32(1), size);

        GpuId id;

        glGenTextures(1, &id); glCheckError();
        glBindTexture(GL_TEXTURE_2D, id); glCheckError();

        GLint unpack_alignment;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment); glCheckError();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); glCheckError();

        Texture::FormatType internal_format, format;
		Texture::TypeType type;

        get_texture_formats(color_type, internal_format, format, type);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size.x, size.y, 0, format, type, data); glCheckError();
        glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment); glCheckError();
        glBindTexture(GL_TEXTURE_2D, 0); glCheckError();

        return id;
    }

    void Gpu::resize_texture(const boost::shared_ptr<Texture>& texture, vec2_u32 size)
    {
        Texture::FormatType internal_format, format;
		Texture::TypeType type;

        size = glm::max(vec2_u32(1), size);

        get_texture_formats(texture->get_color_type(), internal_format, format, type);

        glBindTexture(GL_TEXTURE_2D, texture->get_id()); glCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, size.x, size.y, 0, format, type, nullptr); glCheckError();
        glBindTexture(GL_TEXTURE_2D, 0); glCheckError();
    }

    void Gpu::destroy_texture(GpuId id)
    {
        glDeleteTextures(1, &id); glCheckError();
    }

    GpuLocation Gpu::get_uniform_location(GpuId program_id, const char* name) const
    {
        auto uniform_location = glGetUniformLocation(program_id, name); glCheckError();

        return uniform_location;
    }

    GpuLocation Gpu::get_attribute_location(GpuId program_id, const char* name) const
    {
        auto attribute_location = glGetAttribLocation(program_id, name); glCheckError();

        return attribute_location;
    }

    void Gpu::enable_vertex_attribute_array(GpuLocation location)
    {
        glEnableVertexAttribArray(location); glCheckError();
    }

    void Gpu::disable_vertex_attribute_array(GpuLocation location)
    {
        glDisableVertexAttribArray(location); glCheckError();
    }

    void Gpu::set_vertex_attrib_pointer(GpuLocation location, i32 size, GpuDataTypes data_type, bool is_normalized, i32 stride, const void* pointer)
    {
        glVertexAttribPointer(location, size, get_data_type(data_type), is_normalized, stride, pointer); glCheckError();
    }

	void Gpu::set_vertex_attrib_pointer(GpuLocation location, i32 size, GpuDataTypes data_type, i32 stride, const void * pointer)
    {
        glVertexAttribIPointer(location, size, get_data_type(data_type), stride, pointer); glCheckError();
    }

    void Gpu::set_uniform(const char* name, const mat3& value, bool should_transpose) const
    {
        glUniformMatrix3fv(get_uniform_location(programs.top()->lock()->get_id(), name), 1, should_transpose, glm::value_ptr(value)); glCheckError();
    }

    void Gpu::set_uniform(const char* name, const mat4& value, bool should_transpose) const
    {
        glUniformMatrix4fv(get_uniform_location(programs.top()->lock()->get_id(), name), 1, should_transpose, glm::value_ptr(value)); glCheckError();
    }

    void Gpu::set_uniform(const char* name, i32 value) const
    {
        glUniform1i(get_uniform_location(programs.top()->lock()->get_id(), name), value); glCheckError();
    }

    void Gpu::set_uniform(const char* name, u32 value) const
    {
        glUniform1i(get_uniform_location(programs.top()->lock()->get_id(), name), value); glCheckError();
    }

    void Gpu::set_uniform(const char* name, f32 value) const
    {
        glUniform1f(get_uniform_location(programs.top()->lock()->get_id(), name), value); glCheckError();
    }

    void Gpu::set_uniform(const char* name, const vec2& value) const
    {
        glUniform2fv(get_uniform_location(programs.top()->lock()->get_id(), name), 1, glm::value_ptr(value)); glCheckError();
    }

    void Gpu::set_uniform(const char* name, const vec3& value) const
    {
        glUniform3fv(get_uniform_location(programs.top()->lock()->get_id(), name), 1, glm::value_ptr(value)); glCheckError();
    }

    void Gpu::set_uniform(const char* name, const vec4& value) const
    {
        glUniform4fv(get_uniform_location(programs.top()->lock()->get_id(), name), 1, glm::value_ptr(value)); glCheckError();
    }

    void Gpu::set_uniform(const char* name, const std::vector<mat4>& matrices, bool should_transpose) const
    {
		const GLfloat* f = reinterpret_cast<const GLfloat*>(matrices.data());
		glUniformMatrix4fv(get_uniform_location(programs.top()->lock()->get_id(), name), static_cast<GLsizei>(matrices.size()), should_transpose ? GL_TRUE : GL_FALSE, f); glCheckError();
    }

    void Gpu::set_uniform_subroutine(ShaderType shader_type, GpuIndex index)
    {
        glUniformSubroutinesuiv(get_shader_type(shader_type), 1, &index); glCheckError();
    }

    void Gpu::set_clear_color(vec4 & color)
    {
        glClearColor(color.r, color.g, color.b, color.a); glCheckError();
    }

    vec4 Gpu::get_clear_color()
    {
        vec4 clear_color;

        glGetFloatv(GL_COLOR_CLEAR_VALUE, glm::value_ptr(clear_color)); glCheckError();

        return clear_color;
    }

    GpuLocation Gpu::get_subroutine_uniform_location(GpuId program_id, ShaderType shader_type, const std::string& name)
    {
        auto location = glGetSubroutineUniformLocation(program_id, get_shader_type(shader_type), name.c_str()); glCheckError();

        return location;
    }

	GpuIndex Gpu::get_subroutine_index(GpuId program_id, ShaderType shader_type, const std::string& name)
    {
        auto index = glGetSubroutineIndex(program_id, get_shader_type(shader_type), name.c_str()); glCheckError();

        return index;
    }

    boost::optional<Gpu::ProgramManager::WeakType> Gpu::ProgramManager::top() const
    {
        boost::optional<WeakType> gpu_program;

        if (!programs.empty())
        {
            gpu_program = programs.top();
        }

        return gpu_program;
    }

    void Gpu::ProgramManager::push(const Gpu::ProgramManager::WeakType& program)
    {
        if (programs.empty() || program.lock() != programs.top().lock())
        {
            glUseProgram(program.lock()->get_id()); glCheckError();
        }

        programs.push(program);

        program.lock()->on_bind();
    }

    Gpu::ProgramManager::WeakType Gpu::ProgramManager::pop()
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

            return WeakType();
        }
        else
        {
            glUseProgram(programs.top().lock()->get_id()); glCheckError();
            
            return programs.top();
        }
    }

    boost::optional<Gpu::FrameBufferManager::WeakType> Gpu::FrameBufferManager::top() const
    {
        boost::optional<WeakType> frame_buffer;

        if (!frame_buffers.empty())
        {
            frame_buffer = frame_buffers.top();
        }

        return frame_buffer;
    }

    void Gpu::FrameBufferManager::push(const Gpu::FrameBufferManager::SharedType& frame_buffer)
    {
        if (frame_buffers.size() == 0 || frame_buffer != frame_buffers.top())
        {
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->get_id()); glCheckError();

            frame_buffer->on_bind();
        }

        frame_buffers.push(frame_buffer);
    }

    Gpu::FrameBufferManager::WeakType Gpu::FrameBufferManager::pop()
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

            return WeakType();
        }
        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffers.top()->get_id()); glCheckError();

            frame_buffers.top()->on_bind();

            return frame_buffers.top();
        }
    }

    Gpu::TextureManager::WeakType Gpu::TextureManager::get(IndexType index) const
    {
        if (index < 0 || index >= texture_count)
        {
            throw std::out_of_range("");
        }

        return textures[index];
    }

	Gpu::TextureManager::WeakType Gpu::TextureManager::bind(IndexType index, const SharedType& texture)
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

	Gpu::TextureManager::WeakType Gpu::TextureManager::unbind(IndexType index)
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

    GpuViewportType Gpu::viewport_mgr::top() const
    {
        if (viewports.empty())
        {
            vec4_i32 viewport;
            glGetIntegerv(GL_VIEWPORT, glm::value_ptr(viewport)); glCheckError();

            return GpuViewportType(viewport.x, viewport.y, viewport.z, viewport.w);
        }

        return viewports.top();
    }

    void Gpu::viewport_mgr::push(const GpuViewportType& viewport)
    {
        viewports.push(viewport);

        glViewport(static_cast<GLint>(viewport.x),
				   static_cast<GLint>(viewport.y),
				   static_cast<GLint>(viewport.width),
				   static_cast<GLint>(viewport.height)); glCheckError();
    }

    GpuViewportType Gpu::viewport_mgr::pop()
    {
        if (viewports.empty())
        {
            throw std::exception();
        }

        const auto& previous_viewport = viewports.top();

        viewports.pop();

		const auto top_viewport = static_cast<details::Rectangle<i32>>(top());

        glViewport(top_viewport.x, top_viewport.y, top_viewport.width, top_viewport.height); glCheckError();

        return previous_viewport;
    }

	void Gpu::BufferManager::put(BufferType & buffer)
    {
        buffers.insert(buffer);
    }

	void Gpu::BufferManager::erase(BufferType & buffer)
    {
        buffers.erase(buffer);
    }
    
	void Gpu::BufferManager::push(BufferTarget target, BufferType buffer)
    {
        if (target_buffers.find(target) == target_buffers.end())
        {
			target_buffers.emplace(std::make_pair(target, std::stack<BufferType>()));
        }

        target_buffers[target].push(buffer);

        glBindBuffer(get_buffer_target(target), buffer->get_id()); glCheckError();
    }

	Gpu::BufferManager::BufferType Gpu::BufferManager::pop(BufferTarget target)
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

        return buffers.empty() ? BufferType() : buffers.top();
    }


	Gpu::BufferManager::BufferType Gpu::BufferManager::top(BufferTarget target) const
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

	void Gpu::BufferManager::data(BufferTarget target, const void* data, size_t size, BufferUsage usage)
    {
        glBufferData(get_buffer_target(target), size, data, get_buffer_usage(usage)); glCheckError();
    }
    
    //TODO: infer index_data_type from bound index buffer
    void Gpu::draw_elements(PrimitiveType primitive_type, size_t count, GpuDataTypes index_data_type, size_t offset) const
    {
        glDrawElements(get_primitive_type(primitive_type), count, get_data_type(index_data_type), reinterpret_cast<GLvoid*>(offset)); glCheckError();
    }

    GpuId Gpu::create_program(const std::string& vertex_shader_source, const std::string& fragment_shader_source) const
    {
        auto create_shader = [&](GLenum type, const std::string& source) -> GLint
        {
            //create shader
            GpuId id = glCreateShader(type); glCheckError();

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

        //create program
        const auto id = glCreateProgram(); glCheckError();

        boost::crc_32_type crc32;
        crc32.process_bytes(vertex_shader_source.data(), vertex_shader_source.size());
        crc32.process_bytes(fragment_shader_source.data(), fragment_shader_source.size());
        auto source_checksum = crc32.checksum();

        //TODO: give these programs proper names
        auto ifstream = cache.get(std::to_string(source_checksum));

        if (ifstream->is_open())
        {
            GLenum binary_format;
            GLsizei binary_length;
            std::vector<char> binary;

            read(*ifstream, binary_format);
            read(*ifstream, binary_length);
            read(*ifstream, binary, binary_length);

            glProgramBinary(id, binary_format, binary.data(), binary_length); glCheckError();

            //link status
            GLint link_status;
            glGetProgramiv(id, GL_LINK_STATUS, &link_status); glCheckError();

            if (link_status == GL_TRUE)
            {
                return id;
            }

            GLint program_info_log_length = 0;

            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &program_info_log_length); glCheckError();

            if (program_info_log_length > 0)
            {
                std::string program_info_log;
                program_info_log.resize(program_info_log_length);

                glGetProgramInfoLog(id, program_info_log_length, nullptr, &program_info_log[0]); glCheckError();

                std::cout << program_info_log << std::endl;
            }
        }

        const auto vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_source);
        const auto fragment_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

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

        //save compiled program binary
        GLsizei binary_length = 0;

        glGetProgramiv(id, GL_PROGRAM_BINARY_LENGTH, &binary_length); glCheckError();

        GLenum binary_format = 0;
        std::vector<u8> program_binary_data(binary_length);

        glGetProgramBinary(id, program_binary_data.size(), &binary_length, &binary_format, static_cast<GLvoid*>(program_binary_data.data())); glCheckError();

        std::stringstream stringstream;
        write(stringstream, binary_format);
        write(stringstream, binary_length);
        write(stringstream, program_binary_data);
        auto data = stringstream.str();

        cache.put_buffer(std::to_string(source_checksum), data.data(), data.size());

        return id;
    }

    void Gpu::destroy_program(GpuId id)
    {
        glDeleteProgram(id); glCheckError();
    }

    //blend
    Gpu::BlendStateManager::BlendState Gpu::BlendStateManager::get_state() const
    {
        if (!states.empty())
        {
            return states.top();
        }

		return BlendState();
    }

	void Gpu::BlendStateManager::push_state(const Gpu::BlendStateManager::BlendState& state)
    {
        apply_state(state);

        states.push(state);
    }

    void Gpu::BlendStateManager::pop_state()
    {
        states.pop();

        apply_state(get_state());
    }

	void Gpu::BlendStateManager::apply_state(const Gpu::BlendStateManager::BlendState& state)
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
    Gpu::depth::state Gpu::depth::get_state() const
    {
        if (!states.empty())
        {
            return states.top();
        }

        return state();
    }

    void Gpu::depth::push_state(const state& state)
    {
        apply_state(state);

        states.push(state);
    }

    void Gpu::depth::pop_state()
    {
        states.pop();

        apply_state(get_state());
    }

    void Gpu::depth::apply_state(const state& state)
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
	Gpu::CullingStateManager::CullingState Gpu::CullingStateManager::get_state() const
    {
        if (!states.empty())
        {
            return states.top();
        }

		return CullingState();
    }

	void Gpu::CullingStateManager::push_state(const CullingState& state)
    {
        apply_state(state);

        states.push(state);
    }

	void Gpu::CullingStateManager::pop_state()
    {
        states.pop();

        apply_state(get_state());
    }

	void Gpu::CullingStateManager::apply_state(const CullingState& state)
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
        case CullingFrontFace::CCW:
            glFrontFace(GL_CCW); glCheckError();
            break;
		case CullingFrontFace::CW:
            glFrontFace(GL_CW); glCheckError();
            break;
        }
        
        switch (state.mode)
        {
        case CullingMode::BACK:
            glCullFace(GL_BACK); glCheckError();
            break;
		case CullingMode::FRONT:
            glCullFace(GL_FRONT); glCheckError();
            break;
		case CullingMode::FRONT_AND_BACK:
            glCullFace(GL_FRONT_AND_BACK); glCheckError();
            break;
        }
    }

    //stencil
	Gpu::StencilStateManager::StencilState Gpu::StencilStateManager::get_state() const
    {
        if (!states.empty())
        {
            return states.top();
        }

		return StencilState();
    }

	void Gpu::StencilStateManager::push_state(const StencilState& state)
    {
        apply_state(state);

        states.push(state);
    }

	void Gpu::StencilStateManager::pop_state()
    {
        states.pop();

        apply_state(get_state());
    }

	void Gpu::StencilStateManager::apply_state(const StencilState& state)
    {
        if (state.is_enabled)
        {
            glEnable(GL_STENCIL_TEST); glCheckError();
        }
        else
        {
            glDisable(GL_STENCIL_TEST); glCheckError();
        }

        glStencilFunc(get_stencil_function(state.function.func), state.function.ref, state.function.mask); glCheckError();
        glStencilOp(get_stencil_operation(state.operations.fail), get_stencil_operation(state.operations.zfail), get_stencil_operation(state.operations.zpass)); glCheckError();
        glStencilMask(state.mask); glCheckError();
    }

    //color
	Gpu::ColorStateManager::ColorState Gpu::ColorStateManager::get_state() const
    {
        if (!states.empty())
        {
            return states.top();
        }

		return ColorState();
    }

	void Gpu::ColorStateManager::push_state(const ColorState& state)
    {
        apply_state(state);

        states.push(state);
    }

	void Gpu::ColorStateManager::pop_state()
    {
        states.pop();

        apply_state(get_state());
    }

	void Gpu::ColorStateManager::apply_state(const ColorState& state)
    {
        glColorMask(state.mask.r ? GL_TRUE : GL_FALSE,
                    state.mask.g ? GL_TRUE : GL_FALSE, 
                    state.mask.b ? GL_TRUE : GL_FALSE, 
                    state.mask.a ? GL_TRUE : GL_FALSE); glCheckError();
    }

    const std::string& Gpu::get_vendor() const
    {
        static const std::string VENDOR = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

        return VENDOR;
    }

    const std::string& Gpu::get_renderer() const
    {
        static const std::string RENDERER = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

        return RENDERER;
    }

    const std::string& Gpu::get_version() const
    {
        static const std::string VERSION = reinterpret_cast<const char*>(glGetString(GL_VERSION));

        return VERSION;
    }

    const std::string& Gpu::get_shading_language_version() const
    {
        static const std::string SHADING_LANGUAGE_VERISON = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

        return SHADING_LANGUAGE_VERISON;
    }

    const std::string& Gpu::get_extensions() const
    {
        static const std::string EXTENSIONS = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

        return EXTENSIONS;
    }

    void Gpu::get_texture_data(const boost::shared_ptr<Texture>& texture, std::vector<u8>& data, i32 level)
    {
        i32 internal_format, format, type;

        get_texture_formats(texture->get_color_type(), internal_format, format, type);

        const auto bytes_per_pixel = get_bytes_per_pixel(texture->get_color_type());

        data.resize(texture->get_width() * texture->get_height() * bytes_per_pixel);

        textures.bind(0, texture);

        glGetTexImage(GL_TEXTURE_2D, level, format, type, data.data());

        textures.unbind(0);
    }

	std::unique_ptr<u8[]> Gpu::get_backbuffer_pixels(i32& width, i32& height)
	{
		auto viewport = viewports.top();
		width = static_cast<i32>(viewport.width);
		height = static_cast<i32>(viewport.height);
		std::unique_ptr<u8[]> pixels(new u8[width * height * 4]);
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLvoid*>(pixels.get()));
		return pixels;
	}
}