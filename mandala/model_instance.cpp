//mandala
#include "model_instance.hpp"
#include "model.hpp"
#include "animation.hpp"
#include "collision.hpp"

//glm
#include <glm\glm.hpp>

namespace mandala
{
	model_instance_t::model_instance_t(std::shared_ptr<model_t> model_) :
		model(model_)
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

	void model_instance_t::tick(float32_t dt)
	{
		//TODO: replace with animation controller class
		t += dt;
		
		if(animation != nullptr)
		{
			auto frame_count = static_cast<float32_t>(animation->frame_count);
            auto seconds_per_frame = 1.0f / animation->frames_per_second;
            auto frame_0_index = static_cast<size_t>(glm::floor(glm::mod(t / seconds_per_frame, frame_count)));
            auto frame_1_index = (frame_0_index + 1) % animation->frame_count;
            auto interpolate_t = glm::mod(t, seconds_per_frame) / seconds_per_frame;
            auto& frame_skeleton_0 = animation->frame_skeletons[frame_0_index];
            auto& frame_skeleton_1 = animation->frame_skeletons[frame_1_index];

			skeleton_t::interpolate(skeleton, frame_skeleton_0, frame_skeleton_1, interpolate_t);
		}

		bone_matrices.resize(skeleton.bones.size());

		for(size_t i = 0; i < skeleton.bones.size(); ++i)
		{
			bone_matrices[i] = skeleton.bone_matrices[i] * model->bones[i].inverse_bind_pose_matrix;
		}
	}

	void model_instance_t::render(const camera_t& camera, const vec3_t& light_position) const
	{
		if (model == nullptr)
		{
			throw std::exception();
		}

		//TODO: take instance transformation into consideration
		if(intersects(skeleton.aabb, camera.frustum) == intersect_type_e::disjoint)
		{
			//skeleton aabb does not intersect camera frustum
			//return;
		}

		auto view_projection_matrix = camera.projection_matrix * camera.view_matrix;

		model->render(camera.position, world_matrix, view_projection_matrix, bone_matrices, light_position);
	}
	
	const skeleton_t::bone_t& model_instance_t::get_bone(const hash_t& bone_name_hash) const
	{
		if (model->bone_indices.find(bone_name_hash) == model->bone_indices.end())
		{
			throw std::exception();
		}

		auto bone_index = model->bone_indices.at(bone_name_hash);

		return skeleton.bones[bone_index];
	}
};
