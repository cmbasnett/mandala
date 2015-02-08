//std
#include <fstream>

//boost
#include <boost/iostreams/read.hpp>

//mandala
#include "model_animation.hpp"

namespace mandala
{
	model_animation_t::model_animation_t(std::istream& istream)
	{
		//magic
        char magic[md5b::magic_length + 1] = { '\0' };
        istream.read(magic, md5b::magic_length);

		if(strcmp(md5b::animation_magic, magic) != 0)
		{
			throw std::exception();
		}

		//version
		int32_t version = 0;
        istream.read(reinterpret_cast<char*>(&version), sizeof(version));

		if(version != md5b::animation_version)
		{
			throw std::exception();
        }

        //frames per second
        istream.read(reinterpret_cast<char*>(&frames_per_second), sizeof(frames_per_second));

		//bone count
        istream.read(reinterpret_cast<char*>(&bone_count), sizeof(bone_count));

		//bones
		std::vector<bone_t> bones;
		bones.resize(bone_count);

		for(auto& bone : bones)
		{
			std::string bone_name;
			std::getline(istream, bone_name, '\0');
			bone.hash = hash_t(bone_name);
            istream.read(reinterpret_cast<char*>(&bone.parent_index), sizeof(bone.parent_index));
            istream.read(reinterpret_cast<char*>(&bone.flags), sizeof(bone.flags));
            istream.read(reinterpret_cast<char*>(&bone.data_start_index), sizeof(bone.data_start_index));
		}

		//frame count
        istream.read(reinterpret_cast<char*>(&frame_count), sizeof(frame_count));

		//frame bounds
		std::vector<aabb3_t> frame_bounds;
		frame_bounds.resize(frame_count);

        for (auto& frame_bound : frame_bounds)
        {
            istream.read(reinterpret_cast<char*>(&frame_bound.min.x), sizeof(frame_bound.min.x));
            istream.read(reinterpret_cast<char*>(&frame_bound.min.y), sizeof(frame_bound.min.y));
            istream.read(reinterpret_cast<char*>(&frame_bound.min.z), sizeof(frame_bound.min.z));
            istream.read(reinterpret_cast<char*>(&frame_bound.max.x), sizeof(frame_bound.max.x));
            istream.read(reinterpret_cast<char*>(&frame_bound.max.y), sizeof(frame_bound.max.y));
            istream.read(reinterpret_cast<char*>(&frame_bound.max.z), sizeof(frame_bound.max.z));
        }

		//base bone frames
		std::vector<pose3> base_bone_frames;
		base_bone_frames.resize(bone_count);

		for(auto& base_bone_frame : base_bone_frames)
		{
            istream.read(reinterpret_cast<char*>(&base_bone_frame.location.x), sizeof(base_bone_frame.location.x));
            istream.read(reinterpret_cast<char*>(&base_bone_frame.location.y), sizeof(base_bone_frame.location.y));
            istream.read(reinterpret_cast<char*>(&base_bone_frame.location.z), sizeof(base_bone_frame.location.z));
            istream.read(reinterpret_cast<char*>(&base_bone_frame.rotation.x), sizeof(base_bone_frame.rotation.x));
            istream.read(reinterpret_cast<char*>(&base_bone_frame.rotation.y), sizeof(base_bone_frame.rotation.y));
            istream.read(reinterpret_cast<char*>(&base_bone_frame.rotation.z), sizeof(base_bone_frame.rotation.z));

			md5b::compute_quaternion_w(base_bone_frame.rotation);
		}

		//frame data count
		uint32_t frame_data_count;
        istream.read(reinterpret_cast<char*>(&frame_data_count), sizeof(frame_data_count));

		//frame data
		std::vector<float32_t> frame_data;
		frame_data.resize(frame_data_count * frame_count);
        istream.read(reinterpret_cast<char*>(frame_data.data()), sizeof(frame_data[0]) * frame_data.size());

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

				const auto& bone = bones[j];
				auto& skeleton_bone = skeleton.bones[j];
				skeleton_bone.parent_index = bone.parent_index;
				skeleton_bone.pose.location = base_bone_frames[j].location;
				skeleton_bone.pose.rotation = base_bone_frames[j].rotation;

				auto frame_data_start_index = (frame_data_count * i) + bone.data_start_index;

				if (bone.flags & bone_t::flags_t::location_x)
				{
					skeleton_bone.pose.location.x = frame_data[frame_data_start_index + k++];
				}

				if(bone.flags & bone_t::flags_t::location_y)
				{
					skeleton_bone.pose.location.z = -frame_data[frame_data_start_index + k++];
				}

				if(bone.flags & bone_t::flags_t::location_z)
				{
					skeleton_bone.pose.location.y = frame_data[frame_data_start_index + k++];
				}

				if(bone.flags & bone_t::flags_t::rotation_x)
				{
					skeleton_bone.pose.rotation.x = frame_data[frame_data_start_index + k++];
				}

				if(bone.flags & bone_t::flags_t::rotation_y)
				{
					skeleton_bone.pose.rotation.z = -frame_data[frame_data_start_index + k++];
				}

				if(bone.flags & bone_t::flags_t::rotation_z)
				{
					skeleton_bone.pose.rotation.y = frame_data[frame_data_start_index + k++];
				}

				md5b::compute_quaternion_w(skeleton_bone.pose.rotation);

				if(skeleton_bone.parent_index != md5b::bone_null_index)
				{
					auto& parent_skeleton_bone = skeleton.bones[skeleton_bone.parent_index];
					auto rotated_location = parent_skeleton_bone.pose.rotation * skeleton_bone.pose.location;

					skeleton_bone.pose.location = parent_skeleton_bone.pose.location + rotated_location;
					skeleton_bone.pose.rotation = glm::normalize(parent_skeleton_bone.pose.rotation * skeleton_bone.pose.rotation);
				}
			}
		}
	}
}
