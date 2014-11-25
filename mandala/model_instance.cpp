//mandala
#include "model_instance.hpp"
#include "model.hpp"
#include "model_animation.hpp"
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
		animation_controller.tick(dt);

		//TODO: figure out where to put this
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
		//if(intersects(skeleton.aabb, camera.frustum) == intersect_type_e::disjoint)
		//{
			//skeleton aabb does not intersect camera frustum
			//return;
		//}

		auto view_projection_matrix = camera.projection_matrix * camera.view_matrix;

		model->render(camera.position, world_matrix, view_projection_matrix, bone_matrices, light_position);
	}
	
	const model_skeleton_t::bone_t& model_instance_t::get_bone(const hash_t& bone_name_hash) const
	{
		if (model->bone_indices.find(bone_name_hash) == model->bone_indices.end())
		{
			throw std::exception();
		}

		auto bone_index = model->bone_indices.at(bone_name_hash);

		return skeleton.bones[bone_index];
	}
}
