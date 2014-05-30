//boost
#include <boost\algorithm\string.hpp>
#include <boost\lexical_cast.hpp>

//md5b
#include "model.h"
#include "exceptions.h"

namespace md5b
{
    model_t::model_t(std::istream& istream)
    {
        std::string line;
        std::vector<std::string> tokens;

        while (istream.good())
        {
            std::getline(istream, line);
            boost::trim(line);
            boost::algorithm::split(tokens, line, boost::is_any_of(" ()\t"), boost::token_compress_on);

            if (tokens.empty())
            {
                continue;
            }

            if (boost::iequals(tokens[0], "md5version"))
            {
                auto version = boost::lexical_cast<size_t>(tokens[1]);

                if (version != 10)
                {
                    throw bad_version(version, 10);
                }
            }
            else if (boost::iequals(tokens[0], "numjoints"))
            {
                auto bone_count = boost::lexical_cast<size_t>(tokens[1]);

                bones.resize(bone_count);
            }
            else if (boost::iequals(tokens[0], "nummeshes"))
            {
                auto mesh_count = boost::lexical_cast<size_t>(tokens[1]);
                
                meshes.reserve(mesh_count);
            }
            else if (boost::iequals(tokens[0], "joints"))
            {
                for (auto& bone : bones)
                {
                    std::getline(istream, line);
                    boost::trim(line);
                    boost::algorithm::split(tokens, line, boost::is_any_of(" ()\t"), boost::token_compress_on);

                    boost::replace_all(tokens[0], "\"", "");

                    bone.name = tokens[0];

                    auto bone_parent_index = boost::lexical_cast<short>(tokens[1]);
                    bone.parent_index = bone_parent_index == -1 ? 255 : boost::numeric_cast<unsigned char>(bone_parent_index);
                    bone.position.x = boost::lexical_cast<float>(tokens[2]);
                    bone.position.y = boost::lexical_cast<float>(tokens[3]);
                    bone.position.z = boost::lexical_cast<float>(tokens[4]);
                    bone.orientation.x = boost::lexical_cast<float>(tokens[5]);
                    bone.orientation.y = boost::lexical_cast<float>(tokens[6]);
                    bone.orientation.z = boost::lexical_cast<float>(tokens[7]);
                }
            }
            else if (boost::iequals(tokens[0], "mesh"))
            {
                mesh_t mesh;

                for (;;)
                {
                    std::getline(istream, line);
                    boost::trim(line);
                    boost::algorithm::split(tokens, line, boost::is_any_of(" ()\t"), boost::token_compress_on);

                    if (boost::iequals(tokens[0], "shader"))
                    {
                        boost::replace_all(tokens[1], "\"", "");

                        mesh.material = tokens[1];
                    }
                    else if (boost::iequals(tokens[0], "numverts"))
                    {
                        auto vertex_count = boost::lexical_cast<size_t>(tokens[1]);

                        mesh.vertices.resize(vertex_count);

                        for (auto& vertex : mesh.vertices)
                        {
                            std::getline(istream, line);
                            boost::trim(line);
                            boost::algorithm::split(tokens, line, boost::is_any_of(" ()\t"), boost::token_compress_on);

                            vertex.texcoord.x = boost::lexical_cast<float>(tokens[2]);
                            vertex.texcoord.y = boost::lexical_cast<float>(tokens[3]);
                            vertex.weight_index_start = boost::lexical_cast<unsigned short>(tokens[4]);
                            vertex.weight_count = boost::numeric_cast<unsigned char>(boost::lexical_cast<unsigned short>(tokens[5]));
                        }
                    }
                    else if (boost::iequals(tokens[0], "numtris"))
                    {
                        auto index_count = boost::lexical_cast<size_t>(tokens[1]) * 3;

                        mesh.indices.resize(index_count);

                        for (size_t i = 0; i < index_count; i += 3)
                        {
                            std::getline(istream, line);
                            boost::trim(line);
                            boost::algorithm::split(tokens, line, boost::is_any_of(" "), boost::token_compress_on);

                            mesh.indices[i + 0] = boost::lexical_cast<unsigned short>(tokens[2]);
                            mesh.indices[i + 1] = boost::lexical_cast<unsigned short>(tokens[3]);
                            mesh.indices[i + 2] = boost::lexical_cast<unsigned short>(tokens[4]);
                        }
                    }
                    else if (boost::iequals(tokens[0], "numweights"))
                    {
                        auto weight_count = boost::lexical_cast<size_t>(tokens[1]);

                        mesh.weights.resize(weight_count);

                        for (auto& weight : mesh.weights)
                        {
                            std::getline(istream, line);
                            boost::trim(line);
                            boost::algorithm::split(tokens, line, boost::is_any_of(" ()\t"), boost::token_compress_on);

                            weight.bone_index = boost::numeric_cast<unsigned char>(boost::lexical_cast<unsigned short>(tokens[2]));
                            weight.bias = boost::lexical_cast<float>(tokens[3]);
                            weight.position.x = boost::lexical_cast<float>(tokens[4]);
                            weight.position.y = boost::lexical_cast<float>(tokens[5]);
                            weight.position.z = boost::lexical_cast<float>(tokens[6]);
                        }
                    }
                    else if (boost::iequals(tokens[0], "}"))
                    {
                        break;
                    }
                }

                meshes.push_back(mesh);
            }
        }
    }

    std::ostream& operator<<(std::ostream& ostream, const model_t& model)
    {
        static const std::array<char, 4> magic = { 'M', 'D', '5', 'M' };
        static const int version = 1;

        //magic
        ostream.write(magic.data(), magic.size());

        //version
        ostream.write(reinterpret_cast<const char*>(&version), sizeof(version));

        //bone count
        auto bone_count = static_cast<unsigned int>(model.bones.size());
        ostream.write((char*)&bone_count, sizeof(bone_count));

        //bones
        for (const auto& bone : model.bones)
        {
            ostream.write(bone.name.c_str(), bone.name.size() + 1);
            ostream.write(reinterpret_cast<const char*>(&bone.parent_index), sizeof(bone.parent_index));
            ostream.write(reinterpret_cast<const char*>(&bone.position.x), sizeof(bone.position.x));
            ostream.write(reinterpret_cast<const char*>(&bone.position.y), sizeof(bone.position.y));
            ostream.write(reinterpret_cast<const char*>(&bone.position.z), sizeof(bone.position.z));
            ostream.write(reinterpret_cast<const char*>(&bone.orientation.x), sizeof(bone.orientation.x));
            ostream.write(reinterpret_cast<const char*>(&bone.orientation.y), sizeof(bone.orientation.y));
            ostream.write(reinterpret_cast<const char*>(&bone.orientation.z), sizeof(bone.orientation.z));
        }

        //mesh count
        auto mesh_count = static_cast<unsigned char>(model.meshes.size());
        ostream.write(reinterpret_cast<const char*>(&mesh_count), sizeof(mesh_count));

        //meshes
        for (const auto& mesh : model.meshes) 
        {
            ostream.write(mesh.material.c_str(), mesh.material.size() + 1);

            //vertex count
            auto vertex_count = static_cast<unsigned int>(mesh.vertices.size());

            ostream.write((char*)&vertex_count, sizeof(vertex_count));

            //vertices
            for (const auto& vertex : mesh.vertices)
            {
                ostream.write(reinterpret_cast<const char*>(&vertex.texcoord.x), sizeof(vertex.texcoord.x));
                ostream.write(reinterpret_cast<const char*>(&vertex.texcoord.y), sizeof(vertex.texcoord.y));
                ostream.write(reinterpret_cast<const char*>(&vertex.weight_index_start), sizeof(vertex.weight_index_start));
                ostream.write(reinterpret_cast<const char*>(&vertex.weight_count), sizeof(vertex.weight_count));
            }

            //index count
            auto index_count = static_cast<unsigned int>(mesh.indices.size());
            ostream.write((char*)&index_count, sizeof(index_count));

            //indices
            for (size_t i = 0; i < mesh.indices.size(); i += 3)
            {
                ostream.write(reinterpret_cast<const char*>(&mesh.indices[i + 0]), sizeof(mesh.indices[0]));
                ostream.write(reinterpret_cast<const char*>(&mesh.indices[i + 1]), sizeof(mesh.indices[0]));
                ostream.write(reinterpret_cast<const char*>(&mesh.indices[i + 2]), sizeof(mesh.indices[0]));
            }

            //weight count
            auto weight_count = static_cast<unsigned int>(mesh.weights.size());
            ostream.write((char*)&weight_count, sizeof(weight_count));

            //weights
            for (const auto& weight : mesh.weights)
            {
                ostream.write(reinterpret_cast<const char*>(&weight.bone_index), sizeof(weight.bone_index));
                ostream.write(reinterpret_cast<const char*>(&weight.bias), sizeof(weight.bias));
                ostream.write(reinterpret_cast<const char*>(&weight.position.x), sizeof(weight.position.x));
                ostream.write(reinterpret_cast<const char*>(&weight.position.y), sizeof(weight.position.y));
                ostream.write(reinterpret_cast<const char*>(&weight.position.z), sizeof(weight.position.z));
            }
        }

        return ostream;
    }
};
