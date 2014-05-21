//std
#include <fstream>
#include <sstream>

//boost
#include <boost\filesystem.hpp>
#include <boost\crc.hpp>

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
    //usage: pack <input_directory> <output> [options]

    if (argc < 3)
    {
        std::cerr << "too few arguments" << std::endl;

        return 1;
    }

    boost::filesystem::path input_directory = argv[1];

    if (!boost::filesystem::is_directory(input_directory))
    {
        std::cerr << "input directory is invalid" << std::endl;

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

    boost::filesystem::path output_path = argv[2];

    boost::filesystem::remove(output_path);

    boost::filesystem::create_directories(output_path.parent_path());

    std::ofstream ofstream;
    ofstream.open(output_path.string(), std::ios_base::binary);

    if (!ofstream.is_open())
    {
        std::cerr << "input directory is invalid" << std::endl;

        return 1;
    }

	auto version = 1;

	ofstream.write(PACK_MAGIC, sizeof(PACK_MAGIC) - 1);
	ofstream.write((char*)&version, sizeof(version));

    auto file_count = paths.size();

	ofstream.write((char*)&file_count, sizeof(file_count));

	//determine length of fat table ahead of time
    auto file_offset = static_cast<uint32_t>(ofstream.tellp());

	for (auto& path : paths)
	{
		auto filename = path.filename().string();

		file_offset += filename.size() + 1;	//+1 for null character
		file_offset += 12;					//offset, length, crc
	}

	for (auto& path : paths)
	{
        auto file_size = static_cast<uint32_t>(boost::filesystem::file_size(path));
		auto file_name = path.filename().string();

		ofstream.write(file_name.data(), file_name.size() + 1);
		ofstream.write((char*)&file_offset, sizeof(file_offset));
		ofstream.write((char*)&file_size, sizeof(file_size));

		//open file and compute crc
		std::ifstream ifstream;
		ifstream.open(path.string(), std::ios_base::binary);

		auto buffer = std::auto_ptr<char>(new char[file_size]);

		ifstream.read(buffer.get(), file_size);

		boost::crc_32_type crc_32;
		crc_32.process_bytes(buffer.get(), file_size);
		auto checksum = crc_32.checksum();

		ofstream.write((char*)&checksum, sizeof(checksum));

		file_offset += file_size;

		ifstream.close();
	}

	for (auto& path : paths)
	{
		auto file_size = boost::filesystem::file_size(path);

		std::ifstream ifstream;
		ifstream.open(path.string(), std::ios_base::binary);

        std::vector<uint8_t> buffer;
        buffer.resize(static_cast<uint32_t>(file_size));

        ifstream.read(reinterpret_cast<char*>(buffer.data()), file_size);
        ofstream.write(reinterpret_cast<char*>(buffer.data()), file_size);
	}

	ofstream.close();

    std::cout << "success" << std::endl;
}
