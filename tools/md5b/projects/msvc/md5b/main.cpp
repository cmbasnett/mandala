//std
#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>

//boost
#include <boost\filesystem.hpp>
#include <boost\algorithm\string.hpp>

//glm
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//md5b
#include "model.h"
#include "animation.h"
#include "exceptions.h"

using namespace md5b;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "too few arguments" << std::endl;
        
        return 1;
    }

    //input path
    boost::filesystem::path input_path(argv[1]);

    std::ifstream ifstream(input_path.string());

    if (!ifstream.is_open())
    {
        std::cerr << "failed to open input file" << std::endl;

        return 1;
    }

    //output path
    boost::filesystem::path output_path(argv[2]);

    if (!boost::filesystem::is_directory(output_path))
    {
        std::cerr << "output path is not a directory" << std::endl;

        return 1;
    }

    boost::filesystem::create_directories(output_path);

    auto filename = input_path.filename().replace_extension();
    auto extension = boost::filesystem::extension(input_path);

    std::ostringstream oss;

    if (boost::iequals(extension, ".md5mesh"))
    {
        filename.replace_extension("md5m");
        output_path += filename;

        try
        {
            model_t model(ifstream);

            oss << model;
        }
        catch (std::exception e)
        {
            std::cerr << "failed to export md5m: " << e.what() << std::endl;

            return 1;
        }

        std::cout << "successfully exported md5m" << std::endl;
    }
    else if (boost::iequals(extension, ".md5anim"))
    {
        filename.replace_extension("md5a");
        output_path += filename;

        try
        {
            animation_t animation(ifstream);

            oss << animation;
        }
        catch (std::exception e)
        {
            std::cerr << "failed to export md5a: " << e.what() << std::endl;

            return 1;
        }

        std::cout << "successfully exported md5a" << std::endl;
    }
    else
    {
        std::cerr << "bad extension" << std::endl;

        return 1;
    }

    std::ofstream ofstream(output_path.string());

    if (!ofstream.is_open())
    {
        std::cerr << "failed to open output file: " << output_path << std::endl;

        return 1;
    }

    ofstream << oss.str();

    return 0;
}
