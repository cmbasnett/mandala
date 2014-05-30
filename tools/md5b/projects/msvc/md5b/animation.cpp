//md5b
#include "animation.h"
#include "exceptions.h"

//boost
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>

namespace md5b
{
    animation_t::animation_t(std::istream& istream)
    {
        std::string line;
        std::vector<std::string> tokens;

        while (istream.good())
        {
            std::getline(istream, line);
            boost::trim(line);
            boost::algorithm::split(tokens, line, boost::is_any_of(" ()\t"), boost::token_compress_on);

            if (tokens.size() == 0)
            {
                continue;
            }

            if (boost::iequals(tokens[0], "md5version"))
            {
                if (boost::iequals(tokens[0], "md5version"))
                {
                    auto version = boost::lexical_cast<size_t>(tokens[1]);

                    if (version != 10)
                    {
                        throw bad_version(version, 10);
                    }
                }
            }
            else if (boost::iequals(tokens[0], "numframes"))
            {
                auto frame_count = boost::lexical_cast<size_t>(tokens[1]);

                frames.resize(frame_count);
            }
            else if (boost::iequals(tokens[0], "numjoints"))
            {
                auto bone_count = boost::lexical_cast<size_t>(tokens[1]);

                bones.resize(bone_count);
                base_bone_frames.resize(bone_count);
            }
            else if (boost::iequals(tokens[0], "framerate"))
            {
                frames_per_second = boost::lexical_cast<size_t>(tokens[1]);
            }
            else if (boost::iequals(tokens[0], "numanimatedcomponents"))
            {
                frame_data_component_count = boost::lexical_cast<int>(tokens[1]);
            }
            else if (boost::iequals(tokens[0], "hierarchy"))
            {
                for (auto& bone : bones)
                {
                    std::getline(istream, line);
                    boost::trim(line);
                    boost::algorithm::split(tokens, line, boost::is_any_of(" ()\t"), boost::token_compress_on);

                    boost::replace_all(tokens[0], "\"", "");

                    bone.name = tokens[0];
                    bone.parent_index = boost::lexical_cast<int>(tokens[1]);
                    bone.flags = static_cast<unsigned char>(boost::lexical_cast<unsigned short>(tokens[2]));
                    bone.frame_data_start_index = boost::lexical_cast<int>(tokens[3]);
                }
            }
            else if (boost::iequals(tokens[0], "bounds"))
            {
                for (auto& frame : frames)
                {
                    std::getline(istream, line);
                    boost::trim(line);
                    boost::algorithm::split(tokens, line, boost::is_any_of(" ()\t"), boost::token_compress_on);

                    frame.min.x = boost::lexical_cast<float>(tokens[1]);
                    frame.min.y = boost::lexical_cast<float>(tokens[2]);
                    frame.min.z = boost::lexical_cast<float>(tokens[3]);
                    frame.max.x = boost::lexical_cast<float>(tokens[4]);
                    frame.max.y = boost::lexical_cast<float>(tokens[5]);
                    frame.max.z = boost::lexical_cast<float>(tokens[6]);
                }
            }
            else if (boost::iequals(tokens[0], "baseframe"))
            {
                for (auto& bone_frame : base_bone_frames)
                {
                    std::getline(istream, line);
                    boost::trim(line);
                    boost::algorithm::split(tokens, line, boost::is_any_of(" ()\t"), boost::token_compress_on);

                    bone_frame.position.x = boost::lexical_cast<float>(tokens[1]);
                    bone_frame.position.y = boost::lexical_cast<float>(tokens[2]);
                    bone_frame.position.z = boost::lexical_cast<float>(tokens[3]);
                    bone_frame.orientation.x = boost::lexical_cast<float>(tokens[4]);
                    bone_frame.orientation.y = boost::lexical_cast<float>(tokens[5]);
                    bone_frame.orientation.z = boost::lexical_cast<float>(tokens[6]);
                }
            }
            else if (boost::iequals(tokens[0], "frame"))
            {
                auto frame_index = boost::lexical_cast<int>(tokens[1]);
                auto& frame = frames[frame_index];
                
                frame.data.resize(frame_data_component_count);

                decltype(frame_data_component_count) frame_data_index = 0;

                while (frame_data_index < frame_data_component_count)
                {
                    std::getline(istream, line);
                    boost::trim(line);
                    boost::algorithm::split(tokens, line, boost::is_any_of(" ()\t"), boost::token_compress_on);

                    for (auto& token : tokens)
                    {
                        frame.data[frame_data_index++] = boost::lexical_cast<float>(token);
                    }
                }
            }
        }
    }

    std::ostream& operator<<(std::ostream& ostream, const animation_t& animation)
    {
        static const std::array<char, 4> magic = { 'M', 'D', '5', 'A' };
        static const int version = 1;

        //magic
        ostream.write(magic.data(), magic.size());

        //version
        ostream.write(reinterpret_cast<const char*>(&version), sizeof(version));

        //frames per second
        ostream.write(reinterpret_cast<const char*>(&animation.frames_per_second), sizeof(animation.frames_per_second));

        //bone count
        auto bone_count = static_cast<unsigned char>(animation.bones.size());
        ostream.write(reinterpret_cast<const char*>(&bone_count), sizeof(bone_count));

        //bones
        for (auto& bone : animation.bones)
        {
            ostream.write(bone.name.c_str(), bone.name.size() + 1);
            ostream.write(reinterpret_cast<const char*>(&bone.parent_index), sizeof(bone.parent_index));
            ostream.write(reinterpret_cast<const char*>(&bone.flags), sizeof(bone.parent_index));
            ostream.write(reinterpret_cast<const char*>(&bone.frame_data_start_index), sizeof(bone.frame_data_start_index));
        }

        //frame count
        auto frame_count = static_cast<unsigned short>(animation.frames.size());
        ostream.write(reinterpret_cast<const char*>(&frame_count), sizeof(frame_count));

        //frame bounds
        for (auto& frame : animation.frames)
        {
            ostream.write(reinterpret_cast<const char*>(&frame.min.x), sizeof(frame.min.x));
            ostream.write(reinterpret_cast<const char*>(&frame.min.y), sizeof(frame.min.y));
            ostream.write(reinterpret_cast<const char*>(&frame.min.z), sizeof(frame.min.z));
            ostream.write(reinterpret_cast<const char*>(&frame.max.x), sizeof(frame.max.x));
            ostream.write(reinterpret_cast<const char*>(&frame.max.y), sizeof(frame.max.y));
            ostream.write(reinterpret_cast<const char*>(&frame.max.z), sizeof(frame.max.z));
        }

        //base bone frames
        for (auto& bone_frame : animation.base_bone_frames)
        {
            ostream.write(reinterpret_cast<const char*>(&bone_frame.position.x), sizeof(bone_frame.position.x));
            ostream.write(reinterpret_cast<const char*>(&bone_frame.position.y), sizeof(bone_frame.position.y));
            ostream.write(reinterpret_cast<const char*>(&bone_frame.position.z), sizeof(bone_frame.position.z));
            ostream.write(reinterpret_cast<const char*>(&bone_frame.orientation.x), sizeof(bone_frame.orientation.x));
            ostream.write(reinterpret_cast<const char*>(&bone_frame.orientation.y), sizeof(bone_frame.orientation.y));
            ostream.write(reinterpret_cast<const char*>(&bone_frame.orientation.z), sizeof(bone_frame.orientation.z));
        }

        //frame data component count
        for (auto& frame : animation.frames)
        {
            ostream.write(reinterpret_cast<const char*>(frame.data.data()), frame.data.size() * sizeof(float));
        }

        return ostream;
    }
};
