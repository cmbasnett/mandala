//std
#include <sstream>

//naga
#include "model.hpp"
#include "model_component.hpp"
#include "model_animation.hpp"
#include "collision.hpp"
#include "resource_manager.hpp"
#include "material_instance.hpp"
#include "debug_renderer.hpp"
#include "game_object.hpp"
#include "camera_params.hpp"
#include "bisect.hpp"

//glm
#include <glm\glm.hpp>

namespace naga
{
    void ModelComponent::on_tick(f32 dt)
    {
		if (animation != nullptr)
		{
			t += dt;

			// TODO: extract this to a function to get the frame index
			auto frame_count = static_cast<f32>(animation->frame_count);
			auto seconds_per_frame = 1.0f / animation->frames_per_second;
			auto frame_0_index = static_cast<size_t>(glm::floor(glm::mod(t / seconds_per_frame, frame_count)));
			auto frame_1_index = (frame_0_index + 1) % animation->frame_count;
			auto interpolate_t = glm::mod(t, seconds_per_frame) / seconds_per_frame;
			auto& frame_skeleton_0 = animation->frame_skeletons[frame_0_index];
			auto& frame_skeleton_1 = animation->frame_skeletons[frame_1_index];

			skeleton.interpolate(frame_skeleton_0, frame_skeleton_1, interpolate_t);
		}

        //TODO: the AABB is not calculated if there is no animation
        for (size_t i = 0; i < skeleton.bones.size(); ++i)
        {
            bone_matrices[i] = skeleton.bone_matrices[i] * model->get_bones()[i].inverse_bind_pose_matrix;
        }

		aabb = skeleton.aabb << get_world_pose().to_matrix();

        sphere.origin = aabb.center();
        sphere.radius = glm::length(aabb.extents());
    }

	void ModelComponent::on_render(CameraParameters& camera_parameters)
    {
        if (model == nullptr)
        {
            throw std::exception();
        }

		// TODO: problem here is that the frustum/sphere/aabb are not 
		if (intersects(camera_parameters.frustum, sphere) == IntersectType::DISJOINT ||
			intersects(camera_parameters.frustum, aabb) == IntersectType::DISJOINT)
        {
            //skeleton aabb does not intersect camera frustum
            return;
        }
        
        // TODO: need to figure out a better way to feed light data into the shaders
        vec3 light_location;

		const auto world_matrix = get_world_pose().to_matrix();
		auto view_projection_matrix = camera_parameters.projection_matrix * camera_parameters.view_matrix;

		model->render(camera_parameters.location, world_matrix, view_projection_matrix, bone_matrices, light_location, mesh_materials);

#if defined (DEBUG)
        render_aabb(mat4(1.0f), view_projection_matrix, aabb, vec4(1, 0, 0, 1));
		render_sphere(mat4(1.0f), view_projection_matrix, sphere, vec4(1));

		for (size_t i = 0; i < model->get_bones().size(); ++i)
		{
			if (skeleton.bones[i].parent_index != 255)
			{
				const auto& a = skeleton.bones[i].pose.location;
				const auto& b = skeleton.bones[skeleton.bones[i].parent_index].pose.location;
				render_line_loop(world_matrix, view_projection_matrix, std::vector<vec3>({ a, b }), vec4(0.5f));
			}

			render_axes(world_matrix * skeleton.bones[i].pose.to_matrix(), view_projection_matrix);
		}
#endif
    }
    
	Pose3 ModelComponent::get_bone_pose(const std::string& bone_name) const
    {
		auto bone_index = model->get_bone_index(bone_name);

        if (!bone_index)
        {
            std::ostringstream oss;
            oss << "model contains no bone " << bone_name;
            throw std::invalid_argument(oss.str().c_str());
        }

        return get_world_pose() * skeleton.bones[*bone_index].pose;
    }

	void ModelComponent::set_model(const boost::shared_ptr<Model>& model)
    {
        this->model = model;

        skeleton.bones.resize(model->get_bones().size());
        skeleton.bone_matrices.resize(model->get_bones().size());

        for (size_t i = 0; i < skeleton.bones.size(); ++i)
        {
            skeleton.bone_matrices[i] = model->get_bones()[i].bind_pose_matrix;
        }

        bone_matrices.resize(skeleton.bones.size());

        for (size_t i = 0; i < skeleton.bones.size(); ++i)
        {
            bone_matrices[i] = skeleton.bone_matrices[i] * model->get_bones()[i].inverse_bind_pose_matrix;
        }

        for (const auto& mesh : model->get_meshes())
        {
			mesh_materials.push_back(boost::make_shared<MaterialInstance>(mesh->material));
        }
    }

	void ModelComponent::play(const std::string& animation_name)
    {
		animation = resources.get<ModelAnimation>(animation_name);
    }

	void pause();
}
