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
	model_t::model_t(std::shared_ptr<model_info_t> model) :
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
        animation_controller.tick(dt);

        //TODO: figure out where to put this
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
}
