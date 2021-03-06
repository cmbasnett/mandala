//std
#include <fstream>

//boost
#include <boost/iostreams/read.hpp>

//naga
#include "model_animation.hpp"
#include "io.hpp"

#define MD5A_MAGIC_LENGTH       (4)
#define MD5A_MAGIC              (std::array<char, MD5A_MAGIC_LENGTH> { { 'M', 'D', '5', 'A' } })
#define MD5A_VERSION            (1)
#define MD5A_BONE_INDEX_NULL    (255)

namespace naga
{
	ModelAnimation::ModelAnimation(std::istream& istream)
    {
        //magic
        std::array<char, 4> magic;
        read(istream, magic);

        if(magic != MD5A_MAGIC)
        {
            throw std::exception();
        }

        //version
        i32 version = 0;
        read(istream, version);

        if(version != MD5A_VERSION)
        {
            throw std::exception();
        }

        //frames per second
        read(istream, frames_per_second);

        //bone count
        read(istream, bone_count);

        //bones
        std::vector<Bone> bones;
        bones.resize(bone_count);

        for(auto& bone : bones)
        {
            std::getline(istream, bone.name, '\0');
            read(istream, bone.parent_index);
            read(istream, bone.flags);
            read(istream, bone.data_start_index);
        }

        //frame count
        read(istream, frame_count);

        //frame bounds
        std::vector<AABB3> frame_bounds;
        frame_bounds.resize(frame_count);

        for (auto& frame_bound : frame_bounds)
        {
            read(istream, frame_bound.min.x);
            read(istream, frame_bound.min.y);
            read(istream, frame_bound.min.z);
            read(istream, frame_bound.max.x);
            read(istream, frame_bound.max.y);
            read(istream, frame_bound.max.z);
        }

        //base bone frames
        std::vector<Pose3> base_bone_frames;
        base_bone_frames.resize(bone_count);

        for(auto& base_bone_frame : base_bone_frames)
        {
            read(istream, base_bone_frame.location.x);
            read(istream, base_bone_frame.location.y);
            read(istream, base_bone_frame.location.z);
            read(istream, base_bone_frame.rotation.x);
            read(istream, base_bone_frame.rotation.y);
            read(istream, base_bone_frame.rotation.z);

            md5b::compute_quaternion_w(base_bone_frame.rotation);
        }

        //frame data count
        u32 frame_data_count;
        read(istream, frame_data_count);

        //frame data
        std::vector<f32> frame_data;
        read(istream, frame_data, frame_data_count * frame_count);

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
                skeleton_bone.pose = base_bone_frames[j];

                auto frame_data_start_index = (frame_data_count * i) + bone.data_start_index;

                if (bone.flags & Bone::Flags::LOCATION_X)
                {
                    skeleton_bone.pose.location.x = frame_data[frame_data_start_index + k++];
                }

                if(bone.flags & Bone::Flags::LOCATION_Y)
                {
                    skeleton_bone.pose.location.z = -frame_data[frame_data_start_index + k++];
                }

                if(bone.flags & Bone::Flags::LOCATION_Z)
                {
                    skeleton_bone.pose.location.y = frame_data[frame_data_start_index + k++];
                }

                if(bone.flags & Bone::Flags::ROTATION_X)
                {
                    skeleton_bone.pose.rotation.x = frame_data[frame_data_start_index + k++];
                }

                if(bone.flags & Bone::Flags::ROTATION_Y)
                {
                    skeleton_bone.pose.rotation.z = -frame_data[frame_data_start_index + k++];
                }

                if(bone.flags & Bone::Flags::ROTATION_Z)
                {
                    skeleton_bone.pose.rotation.y = frame_data[frame_data_start_index + k++];
                }

                md5b::compute_quaternion_w(skeleton_bone.pose.rotation);

                if(skeleton_bone.parent_index != MD5A_BONE_INDEX_NULL)
                {
                    const auto& parent_skeleton_bone = skeleton.bones[skeleton_bone.parent_index];
                    auto rotated_location = parent_skeleton_bone.pose.rotation * skeleton_bone.pose.location;

                    skeleton_bone.pose.location = parent_skeleton_bone.pose.location + rotated_location;
                    skeleton_bone.pose.rotation = glm::normalize(parent_skeleton_bone.pose.rotation * skeleton_bone.pose.rotation);
                }
            }
        }
    }
}