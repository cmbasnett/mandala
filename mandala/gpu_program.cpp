//boost
#include <boost\property_tree\json_parser.hpp>

//mandala
#include "gpu_program.hpp"
#include "app.hpp"
#include "shader.hpp"

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
        id = glCreateProgram(); glCheckError();

        std::vector<std::shared_ptr<shader_t>> shaders;

		for (auto& shader_ptree : shaders_ptree)
		{
            auto shader = app.resources.get<shader_t>(hash_t(shader_ptree.second.data()));

			//attach shader
			glAttachShader(id, shader->handle);

			if ((error = glGetError()) != GL_NO_ERROR)
			{
                glDeleteProgram(id);

				throw std::exception();
			}

            shaders.push_back(shader);
		}

		//link program
		glLinkProgram(id);

        //link status
		GLint link_status;
        glGetProgramiv(id, GL_LINK_STATUS, &link_status);

		if (link_status == GL_FALSE)
		{
			GLsizei program_info_log_length = 0;
			GLchar program_info_log[1024] = { 0 };
            glGetProgramInfoLog(id, 1024, &program_info_log_length, &program_info_log[0]);

			if (program_info_log_length > 0)
			{
				std::cout << program_info_log << std::endl;
			}

            glDeleteProgram(id);

			throw std::exception();
		}

		for (auto& shader : shaders)
		{
			//detach vertex shader
            glDetachShader(id, shader->handle);

			if ((error = glGetError()) != GL_NO_ERROR)
			{
                glDeleteProgram(id);

				throw std::exception();
			}
		}
	}

	gpu_program_t::~gpu_program_t()
	{
        glDeleteProgram(id);
	}
};