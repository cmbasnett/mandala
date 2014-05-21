//boost
#include <boost\property_tree\json_parser.hpp>

//mandala
#include "gpu_program.h"
#include "app.h"
#include "shader.h"

namespace mandala
{
	gpu_program_t::gpu_program_t(std::istream& istream)
	{
		using namespace boost::property_tree;

		ptree ptree_;

		read_json(istream, ptree_);

		auto& shaders_ptree = ptree_.get_child("shaders");

		GLenum error;

		//create program
		program = glCreateProgram();

		if ((error = glGetError()) != GL_NO_ERROR)
		{
			throw std::exception();
		}

		for (auto& shader_ptree : shaders_ptree)
		{
			auto& resource_name = shader_ptree.second.data();

			auto shader = app.resources.get<shader_t>(hash_t(resource_name));

			//attach shader
			glAttachShader(program, shader->handle);

			if ((error = glGetError()) != GL_NO_ERROR)
			{
				glDeleteProgram(program);

				throw std::exception();
			}

			shaders.push_back(shader);
		}

		//link program
		glLinkProgram(program);
		GLint link_status;
		glGetProgramiv(program, GL_LINK_STATUS, &link_status);

		if (link_status == GL_FALSE)
		{
#ifdef _DEBUG
			GLsizei program_info_log_length = 0;
			GLchar program_info_log[1024] = { 0 };
			glGetProgramInfoLog(program, 1024, &program_info_log_length, &program_info_log[0]);

			if (program_info_log_length > 0)
			{
				std::cout << program_info_log << std::endl;
			}
#endif
			glDeleteProgram(program);

			throw std::exception();
		}

		for (auto& shader : shaders)
		{
			//detach vertex shader
			glDetachShader(program, shader->handle);

			if ((error = glGetError()) != GL_NO_ERROR)
			{
				glDeleteProgram(program);

				throw std::exception();
			}
		}
	}

	gpu_program_t::~gpu_program_t()
	{
		glDeleteProgram(program);
	}
};