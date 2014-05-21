#pragma once

//std
#include <string>

//boost
#include <boost\property_tree\json_parser.hpp>

//mandala
#include "mandala.h"
#include "shader.h"

namespace mandala
{
	shader_t::shader_t(std::istream& istream)
	{
		using namespace boost::property_tree;

		ptree ptree_;

		read_json(istream, ptree_);

		//type
		auto type = ptree_.get<std::string>("type");

		GLenum shader_type;

		if (type == "vertex")
		{
			shader_type = GL_VERTEX_SHADER;
		}
		else if (type == "fragment")
		{
			shader_type = GL_FRAGMENT_SHADER;
		}
		else
		{
			throw std::exception();
		}

		//source
		auto source = ptree_.get<std::string>("source");

		GLenum error;

		//create shader
		handle = glCreateShader(shader_type);

		if ((error = glGetError()) != GL_NO_ERROR)
		{
			throw std::exception();
		}

		auto strings = source.c_str();
        GLint lengths[1] = { static_cast<GLint>(source.length()) };

		//set shader source
		glShaderSource(handle, 1, &strings, &lengths[0]);

		if ((error = glGetError()) != GL_NO_ERROR)
		{
			throw std::exception();
		}

		//compile shader
		glCompileShader(handle);

		GLsizei compile_status;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &compile_status);

		if (compile_status == GL_FALSE)
		{
#ifdef _DEBUG
			GLsizei shader_info_log_length = 0;
			GLchar shader_info_log[1024] = { '\0' };

			glGetShaderInfoLog(handle, 1024, &shader_info_log_length, &shader_info_log[0]);

			std::cerr << shader_info_log << std::endl;
#endif
			glDeleteShader(handle);

			throw std::exception();
		}
	}

	shader_t::~shader_t()
	{
		GLenum error;

		glDeleteShader(handle);

		if ((error = glGetError()) != GL_NO_ERROR)
		{
			throw std::exception();
		}
	}
};
