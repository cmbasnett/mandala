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

		//create program
        id = glCreateProgram(); glCheckError();

        std::vector<std::shared_ptr<shader_t>> shaders;

		for (auto& shader_ptree : shaders_ptree)
		{
            auto shader = app.resources.get<shader_t>(hash_t(shader_ptree.second.data()));

			//attach shader
            glAttachShader(id, shader->handle); glCheckError();

            shaders.push_back(shader);
		}

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

		for (auto& shader : shaders)
		{
			//detach vertex shader
            glDetachShader(id, shader->handle); glCheckError();
		}
	}

	gpu_program_t::~gpu_program_t()
	{
        glDeleteProgram(id); glCheckError();
	}
};