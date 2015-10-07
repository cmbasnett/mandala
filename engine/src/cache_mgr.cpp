//std
#include <fstream>
#include <memory>
#include <sstream>

//boost
#include <boost\filesystem.hpp>
#include <boost\crc.hpp>
#include <boost\property_tree\json_parser.hpp>

//mandala
#include "cache_mgr.hpp"
#include "io.hpp"

#define CACHE_DIRECTORY "cache"
#define CACHE_FILENAME  ".cache"
#define CACHE_PATH      CACHE_DIRECTORY "/" CACHE_FILENAME

namespace mandala
{
    cache_mgr cache;

    cache_mgr::cache_mgr()
    {
        read();
    }

    cache_mgr::~cache_mgr()
    {
        write();
    }

    std::unique_ptr<std::ifstream> cache_mgr::get(const std::string& file_name) const
    {
        boost::filesystem::path file_path(CACHE_DIRECTORY);
        file_path /= file_name;

        return std::make_unique<std::ifstream>(file_path.string());
    }

    i32 cache_mgr::put(const std::string& file_name, const void* data, size_t count)
    {
        boost::crc_32_type crc32;

        crc32.process_bytes(data, count);

        boost::filesystem::path file_path(CACHE_DIRECTORY);
        file_path /= file_name;

        std::ofstream ofstream(file_path.c_str(), std::ios::out);
        ::write(ofstream, data, count);

        auto checksum = crc32.checksum();

        file_checksums.insert(std::make_pair(file_name, checksum));

        return checksum;
    }

    void cache_mgr::erase(const std::string& file_name)
    {
        boost::filesystem::path file_path(CACHE_DIRECTORY);
        file_path /= file_name;

        try
        {
            boost::filesystem::remove(file_path);
        }
        catch (boost::filesystem::filesystem_error e)
        {
            std::cerr << e.what() << std::endl;
        }

        file_checksums.erase(file_name);
    }

    void cache_mgr::purge()
    {
        try
        {
            boost::filesystem::remove_all(CACHE_DIRECTORY);
        }
        catch (boost::filesystem::filesystem_error e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    i32 cache_mgr::checksum(const std::string& file_name) const
    {
        auto file_checksums_itr = file_checksums.find(file_name);

        if (file_checksums_itr == file_checksums.end())
        {
            throw std::invalid_argument("file does not exist");
        }

        return file_checksums_itr->second;
    }

    void cache_mgr::write()
    {
        using namespace boost::property_tree;

        std::lock_guard<std::mutex> lock(mutex);

        auto ofstream = std::ofstream(CACHE_PATH, std::ios::trunc);

        if (!ofstream.is_open())
        {
            return;
        }

        ptree ptree_;

        for (const auto& file_checksum : file_checksums)
        {
            ptree_.put(file_checksum.first, file_checksum.second);
        }

        try
        {
            write_json(ofstream, ptree_);
        }
        catch (json_parser_error e)
        {
            std::cerr << e.what() << std::endl; 
        }
    }

    void cache_mgr::read()
    {
        using namespace boost::property_tree;

        std::lock_guard<std::mutex> lock(mutex);

        auto ifstream = std::ifstream(CACHE_PATH);

        if (!ifstream.is_open())
        {
            return;
        }

        ptree ptree_;

        try
        {
            read_json(ifstream, ptree_);
        }
        catch (json_parser_error& e)
        {
            std::cerr << e.what() << std::endl;

            return;
        }
        
        file_checksums.clear();

        for (auto& ptree_itr : ptree_)
        {
            file_checksums.insert(std::make_pair(ptree_itr.first, std::atoi(ptree_itr.second.data().c_str())));
        }
    }
}