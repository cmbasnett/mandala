//std
#include <fstream>

//boost
#include <boost/iostreams/read.hpp>

//mandala
#include "animation.h"

namespace mandala
{
	animation_t::animation_t(std::istream& istream)
	{
		//magic
        char magic[md5b::magic_length + 1] = { '\0' };
        istream.read(magic, md5b::magic_length);

		if(strcmp(md5b::magic, magic) != 0)
		{
			throw std::exception();
		}

        istream.read(magic, md5b::magic_length);

		if(strcmp(md5b::animation_magic, magic) != 0)
		{
			throw std::exception();
		}

		//version
		int32_t version = 0;
		istream.read((char*)&version, sizeof(version));

		if(version != md5b::animation_version)
		{
			throw std::exception();
		}

		//name
		std::getline(istream, name, '\0');

		//frames per second
		istream.read((char*)&frames_per_second, sizeof(frames_per_second));

		//bone count
		istream.read((char*)&bone_count, sizeof(bone_count));

		//bones
		std::vector<bone_t> bones;
		bones.resize(bone_count);

		for(auto& bone : bones)
		{
			std::string bone_name;
			std::getline(istream, bone_name, '\0');
			bone.hash = hash_t(bone_name);
			istream.read((char*)&bone.parent_index, sizeof(bone.parent_index));
			istream.read((char*)&bone.flags, sizeof(bone.flags));
			istream.read((char*)&bone.data_start_index, sizeof(bone.data_start_index));
		}

		//frame count
		istream.read((char*)&frame_count, sizeof(frame_count));

		//frame bounds
		std::vector<aabb3_t> frame_bounds;
		frame_bounds.resize(frame_count);
		istream.read((char*)frame_bounds.data(), sizeof(frame_bounds[0]) * frame_count);

		//base bone frames
		std::vector<bone_frame_t> base_bone_frames;
		base_bone_frames.resize(bone_count);

		for(auto& base_bone_frame : base_bone_frames)
		{
			istream.read((char*)&base_bone_frame.position, sizeof(base_bone_frame.position));
			istream.read((char*)&base_bone_frame.orientation.x, sizeof(base_bone_frame.orientation.x));
			istream.read((char*)&base_bone_frame.orientation.y, sizeof(base_bone_frame.orientation.y));
			istream.read((char*)&base_bone_frame.orientation.z, sizeof(base_bone_frame.orientation.z));

			md5b::compute_quaternion_w(base_bone_frame.orientation);
		}

		//frame data count
		uint32_t frame_data_count;
		istream.read((char*)&frame_data_count, sizeof(frame_data_count));

		//frame data
		std::vector<float32_t> frame_data;
		frame_data.resize(frame_data_count * frame_count);
		istream.read((char*)frame_data.data(), sizeof(frame_data[0]) * frame_data.size());

		//build frame skeletons
		frame_skeletons.resize(frame_count);

		for(size_t i = 0; i < frame_skeletons.size(); ++i)
		{
			auto& skeleton = frame_skeletons[i];

			skeleton.bones.resize(bones.size());
			skeleton.bone_matrices.resize(bones.size());
			skeleton.aabb = frame_bounds[i];

			for(size_t j = 0; j < bones.size(); ++j)
			{
				auto k = 0;

				auto& bone = bones[j];
				auto& skeleton_bone = skeleton.bones[j];
				skeleton_bone.parent_index = bone.parent_index;
				skeleton_bone.position = base_bone_frames[j].position;
				skeleton_bone.orientation = base_bone_frames[j].orientation;

				auto frame_data_start_index = (frame_data_count * i) + bone.data_start_index;

				if (bone.flags & bone_t::flags_t::position_x)
				{
					skeleton_bone.position.x = frame_data[frame_data_start_index + k++];
				}

				if(bone.flags & bone_t::flags_t::position_y)
				{
					skeleton_bone.position.z = -frame_data[frame_data_start_index + k++];
				}

				if(bone.flags & bone_t::flags_t::position_z)
				{
					skeleton_bone.position.y = frame_data[frame_data_start_index + k++];
				}

				if(bone.flags & bone_t::flags_t::orientation_x)
				{
					skeleton_bone.orientation.x = frame_data[frame_data_start_index + k++];
				}

				if(bone.flags & bone_t::flags_t::orientation_y)
				{
					skeleton_bone.orientation.z = -frame_data[frame_data_start_index + k++];
				}

				if(bone.flags & bone_t::flags_t::orientation_z)
				{
					skeleton_bone.orientation.y = frame_data[frame_data_start_index + k++];
				}

				md5b::compute_quaternion_w(skeleton_bone.orientation);

				if(skeleton_bone.parent_index != md5b::bone_null_index)
				{
					auto& parent_skeleton_bone = skeleton.bones[skeleton_bone.parent_index];
					auto rotated_position = parent_skeleton_bone.orientation * skeleton_bone.position;

					skeleton_bone.position = parent_skeleton_bone.position + rotated_position;
					skeleton_bone.orientation = glm::normalize(parent_skeleton_bone.orientation * skeleton_bone.orientation);
				}
			}
		}
	}
};
