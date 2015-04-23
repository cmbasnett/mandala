//std
#include <sstream>

//mandala
#include "model.hpp"
#include "model_info.hpp"
#include "model_animation.hpp"
#include "collision.hpp"
#include "resource_mgr.hpp"

//glm
#include <glm\glm.hpp>

namespace mandala
{
	model_t::model_t(boost::shared_ptr<model_info_t> model) :
		model(model)
	{
		//TODO: kind of ham-fisted, find a better way to do this?
		skeleton.bones.resize(model->bones.size());
		skeleton.bone_matrices.resize(model->bones.size());

		for(size_t i = 0; i < skeleton.bones.size(); ++i)
		{
			skeleton.bone_matrices[i] = model->bones[i].bind_pose_matrix;
		}

		bone_matrices.resize(skeleton.bones.size());

		for(size_t i = 0; i < skeleton.bones.size(); ++i)
		{
			bone_matrices[i] = skeleton.bone_matrices[i] * model->bones[i].inverse_bind_pose_matrix;
		}
    }

    model_t::model_t(const hash_t& model_hash) :
        model_t(resources.get<model_info_t>(model_hash))
    {
    }

    void model_t::tick(float32_t dt)
    {
        if (animation != nullptr)
        {
            t += dt;

            auto frame_count = static_cast<float32_t>(animation->frame_count);
            auto seconds_per_frame = 1.0f / animation->frames_per_second;
            auto frame_0_index = static_cast<size_t>(glm::floor(glm::mod(t / seconds_per_frame, frame_count)));
            auto frame_1_index = (frame_0_index + 1) % animation->frame_count;
            auto interpolate_t = glm::mod(t, seconds_per_frame) / seconds_per_frame;
            auto& frame_skeleton_0 = animation->frame_skeletons[frame_0_index];
            auto& frame_skeleton_1 = animation->frame_skeletons[frame_1_index];

            model_skeleton_t::interpolate(skeleton, frame_skeleton_0, frame_skeleton_1, interpolate_t);
        }

        //TODO: this shouldn't be necessary?
        bone_matrices.resize(skeleton.bones.size());

        for (size_t i = 0; i < skeleton.bones.size(); ++i)
        {
            bone_matrices[i] = skeleton.bone_matrices[i] * model->bones[i].inverse_bind_pose_matrix;
        }
    }

	void model_t::render(const camera_t& camera, const vec3_t& light_location) const
	{
		if (model == nullptr)
		{
			throw std::exception();
		}

		//TODO: take instance transformation into consideration
		//if(intersects(skeleton.aabb, camera.frustum) == intersect_type_e::disjoint)
		//{
			//skeleton aabb does not intersect camera frustum
			//return;
		//}

        auto view_projection_matrix = camera.get_projection_matrix() * camera.get_view_matrix();

		model->render(camera.location, pose.to_matrix(), view_projection_matrix, bone_matrices, light_location);
	}
	
	const pose3 model_t::get_bone_pose(const hash_t& bone_hash) const
	{
		auto bone_index = model->get_bone_index(bone_hash);

        if (!bone_index)
        {
            std::ostringstream oss;
            oss << "model contains no bone " << bone_hash.get_value();
            throw std::invalid_argument(oss.str().c_str());
        }

        return skeleton.bones[*bone_index].pose;
	}

    void model_t::play(const hash_t& animation_hash)
    {
        animation = resources.get<model_animation_t>(animation_hash);
    }
}
