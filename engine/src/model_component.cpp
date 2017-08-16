//std
#include <sstream>

//naga
#include "model.hpp"
#include "model_component.hpp"
#include "model_animation.hpp"
#include "collision.hpp"
#include "resource_mgr.hpp"
#include "material_instance.hpp"
#include "line_renderer.hpp"
#include "game_object.hpp"
#include "camera_params.hpp"

//glm
#include <glm\glm.hpp>

namespace naga
{
    void model_component::on_tick(f32 dt)
    {
        if (animation != nullptr)
        {
            t += dt;

            auto frame_count = static_cast<f32>(animation->frame_count);
            auto seconds_per_frame = 1.0f / animation->frames_per_second;
            auto frame_0_index = static_cast<size_t>(glm::floor(glm::mod(t / seconds_per_frame, frame_count)));
            auto frame_1_index = (frame_0_index + 1) % animation->frame_count;
            auto interpolate_t = glm::mod(t, seconds_per_frame) / seconds_per_frame;
            auto& frame_skeleton_0 = animation->frame_skeletons[frame_0_index];
            auto& frame_skeleton_1 = animation->frame_skeletons[frame_1_index];

            model_skeleton::interpolate(skeleton, frame_skeleton_0, frame_skeleton_1, interpolate_t);
        } 

        //TODO: the AABB is not calculated if there is no animation

        for (size_t i = 0; i < skeleton.bones.size(); ++i)
        {
            bone_matrices[i] = skeleton.bone_matrices[i] * model->get_bones()[i].inverse_bind_pose_matrix;
        }

        aabb = skeleton.aabb << get_owner()->pose.to_matrix();

        sphere.origin = aabb.center();
        sphere.radius = glm::length(aabb.extents());
    }

    void model_component::on_render(camera_params& camera_params)
    {
        if (model == nullptr)
        {
            throw std::exception();
        }

        if (intersects(camera_params.frustum, sphere) == intersect_type_e::DISJOINT ||
            intersects(camera_params.frustum, aabb) == intersect_type_e::DISJOINT)
        {
            //skeleton aabb does not intersect camera frustum
            return;
        }
        
        // TODO: need to figure out a better way to feed light data into the shaders
        vec3 light_location;

        const auto world_matrix = get_owner()->pose.to_matrix();
        auto view_projection_matrix = camera_params.projection_matrix * camera_params.view_matrix;

        model->render(camera_params.location, world_matrix, view_projection_matrix, bone_matrices, light_location, mesh_materials);

#if defined (DEBUG)
        render_aabb(world_matrix, view_projection_matrix, skeleton.aabb, vec4(1, 0, 0, 1));
		render_sphere(world_matrix, view_projection_matrix, sphere, vec4(1, 0, 1, 1));

		for (size_t i = 0; i < model->get_bones().size(); ++i)
		{
			if (skeleton.bones[i].parent_index != 255)
			{
				const auto& a = skeleton.bones[i].pose.location;
				const auto& b = skeleton.bones[skeleton.bones[i].parent_index].pose.location;
				render_line_loop(world_matrix, view_projection_matrix, std::vector<vec3>({ a, b }), vec4(0.5f));
			}

			render_sphere(glm::translate(skeleton.bones[i].pose.location) * world_matrix, view_projection_matrix, naga::sphere(vec3(), 0.5f), vec4(1));
		}
#endif
    }
    
    pose3 model_component::get_bone_pose(const std::string& bone_name) const
    {
		auto bone_index = model->get_bone_index(bone_name);

        if (!bone_index)
        {
            std::ostringstream oss;
            oss << "model contains no bone " << bone_name;
            throw std::invalid_argument(oss.str().c_str());
        }

        return get_owner()->pose * skeleton.bones[*bone_index].pose;
    }

    void model_component::set_model(const boost::shared_ptr<naga::model>& model)
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
            mesh_materials.push_back(boost::make_shared<material_instance>(mesh->material));
        }
    }

    void model_component::play(const std::string& animation_name)
    {
		animation = resources.get<model_animation>(animation_name);
    }
}
