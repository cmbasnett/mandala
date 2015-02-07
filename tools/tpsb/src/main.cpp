//std
#include <sstream>

//boost
#include <boost\filesystem.hpp>
#include <boost\algorithm\string.hpp>

//tpb
#include "sprite_set.hpp"

using namespace tpb;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "too few arguments" << std::endl;

        return 1;
    }

    //input path
    boost::filesystem::path input_path(argv[1]);

    std::ifstream ifstream(input_path.string());

    if (!ifstream.is_open())
    {
        std::cerr << "failed to open input file \"" << input_path << "\"" << std::endl;

        return 1;
    }

    //output path
    boost::filesystem::path output_path;

    if (argc >= 3)
    {
        output_path = argv[2];
    }
    else
    {
        output_path = input_path.parent_path();
        output_path += "\\";
    }

    std::cout << output_path << std::endl;

    if (!boost::filesystem::is_directory(output_path))
    {
        std::cerr << "output path \"" << output_path << "\"is not a directory" << std::endl;

        return 1;
    }

    boost::filesystem::create_directories(output_path);

    auto filename = input_path.filename().replace_extension();
    auto extension = boost::filesystem::extension(input_path);

    std::ostringstream oss;

    if (boost::iequals(extension, ".json"))
    {
        filename.replace_extension("tpsb");
        output_path /= filename;

        try
        {
            sprite_set_t sprite_set(ifstream);

            oss << sprite_set;
        }
        catch (std::exception e)
        {
            std::cerr << "failed to export tpsb: " << e.what() << std::endl;

            return 1;
        }
    }
    else
    {
        std::cerr << "bad extension" << std::endl;

        return 1;
    }

    std::ofstream ofstream(output_path.string(), std::ios::binary);

    if (!ofstream.is_open())
    {
        std::cerr << "failed to open output file: " << output_path << std::endl;

        return 1;
    }

    ofstream << oss.str();

    std::cout << "successfully exported to " << output_path << std::endl;

    return 0;
}