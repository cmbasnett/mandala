//std
#include <fstream>
#include <sstream>

//boost
#include <boost\filesystem.hpp>
#include <boost\crc.hpp>
#include <boost\algorithm\string.hpp>

#define PACK_MAGIC		("PACK")

struct recursive_directory_range
{
    typedef boost::filesystem::recursive_directory_iterator iterator;
    recursive_directory_range(boost::filesystem::path path) :
        path(path)
    {
    }

    iterator begin() { return boost::filesystem::recursive_directory_iterator(path); }
    iterator end() { return boost::filesystem::recursive_directory_iterator(); }

    boost::filesystem::path path;
};

int main(int argc, char* argv[])
{
    static auto version_major = 1;
    static auto version_minor = 0;

    std::cout <<
        "pack [version " << version_major << "." << version_minor << "]" << std::endl <<
        "(c) 2014 colin basnett. all rights reserved." << std::endl <<
        std::endl;

    if (argc < 3)
    {
        std::cout <<
            "pack <input> <output> [/verbose]" << std::endl <<
            std::endl <<
            "  <input>       input directory." << std::endl <<
            "  <output>      output path." << std::endl <<
            std::endl <<
            "  [/verbose]    verbose console output.";

        return 1;
    }

    const boost::filesystem::path input_directory = argv[1];

    if (!boost::filesystem::is_directory(input_directory))
    {
        std::cerr << "error: input directory is invalid" << std::endl;

        return 1;
    }

    std::vector<boost::filesystem::path> paths;

    for (const auto& path : recursive_directory_range(input_directory))
    {
        if (boost::filesystem::is_regular_file(path.path()))
        {
            paths.push_back(path);
        }
    }

    const boost::filesystem::path output_path = argv[2];

    boost::filesystem::remove(output_path);
    boost::filesystem::create_directories(output_path.parent_path());

    std::ofstream ofstream;
    ofstream.open(output_path.string(), std::ios_base::binary);

    if (!ofstream.is_open())
    {
        std::cerr << "error: output directory is invalid" << std::endl;

        return 1;
    }

    bool is_verbose = false;

    for (auto i = 3; i < argc; ++i)
    {
        if (boost::iequals(argv[3], "/verbose"))
        {
            is_verbose = true;
        }
        else
        {
            std::cerr << "invalid argument '" << argv[i] << "'" << std::endl;

            return 1;
        }
    }

    if (is_verbose)
    {
        std::cout << "verbose output on";
    }

	ofstream.write(PACK_MAGIC, sizeof(PACK_MAGIC) - 1);
    ofstream.write(reinterpret_cast<char*>(&version_major), sizeof(version_major));

    auto file_count = paths.size();

    if (is_verbose)
    {
        std::cout << file_count << " files" << std::endl;
    }

    ofstream.write(reinterpret_cast<char*>(&file_count), sizeof(file_count));

	//determine length of fat table ahead of time
    auto file_offset = static_cast<uint32_t>(ofstream.tellp());

	for (auto& path : paths)
	{
		const auto filename = path.filename().string();

		file_offset += filename.size() + 1;	//+1 for null character
		file_offset += 12;					//offset, length, crc
	}

	for (const auto& path : paths)
	{
        auto file_size = static_cast<uint32_t>(boost::filesystem::file_size(path));
		const auto file_name = path.filename().string();

		ofstream.write(file_name.data(), file_name.size() + 1);
        ofstream.write(reinterpret_cast<char*>(&file_offset), sizeof(file_offset));
        ofstream.write(reinterpret_cast<char*>(&file_size), sizeof(file_size));

		//open file and compute crc
		std::ifstream ifstream;
		ifstream.open(path.string(), std::ios_base::binary);

        if (!ifstream.is_open())
        {
            std::cerr << "error: file \"" << path << "\"is invalid" << std::endl;

            return 1;
        }

		auto buffer = std::auto_ptr<char>(new char[file_size]);

		ifstream.read(buffer.get(), file_size);

		boost::crc_32_type crc_32;
		crc_32.process_bytes(buffer.get(), file_size);
		auto checksum = crc_32.checksum();

        ofstream.write(reinterpret_cast<char*>(&checksum), sizeof(checksum));

		file_offset += file_size;

        ifstream.close();

        if (is_verbose)
        {
            std::cout << "wrote " << path.filename() << " (" << boost::filesystem::file_size(path) << " bytes)" << std::endl;
        }
	}

	for (auto& path : paths)
	{
		const auto file_size = boost::filesystem::file_size(path);

		std::ifstream ifstream;
		ifstream.open(path.string(), std::ios_base::binary);

        std::vector<uint8_t> buffer;
        buffer.resize(static_cast<uint32_t>(file_size));

        ifstream.read(reinterpret_cast<char*>(buffer.data()), file_size);
        ofstream.write(reinterpret_cast<char*>(buffer.data()), file_size);
	}

	ofstream.close();

    std::cout << "success: wrote " << boost::filesystem::file_size(output_path) << " bytes (" << boost::filesystem::file_size(output_path) / 1048576 << " MB)" << std::endl;
}
