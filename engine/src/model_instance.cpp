//std
#include <sstream>

//mandala
#include "model.hpp"
#include "model_instance.hpp"
#include "model_animation.hpp"
#include "collision.hpp"
#include "resource_mgr.hpp"
#include "material_instance.hpp"
#include "line_renderer.hpp"

//glm
#include <glm\glm.hpp>

namespace mandala
{
    model_instance_t::model_instance_t(boost::shared_ptr<model_t> model)
    {
        set_model(model);
    }

    model_instance_t::model_instance_t(const hash_t& model_hash) :
        model_instance_t(resources.get<model_t>(model_hash))
    {
    }

    void model_instance_t::tick(float32_t dt)
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

        //TODO: the AABB is not calculated if there is no animation

        for (size_t i = 0; i < skeleton.bones.size(); ++i)
        {
            bone_matrices[i] = skeleton.bone_matrices[i] * model->get_bones()[i].inverse_bind_pose_matrix;
        }

        aabb = skeleton.aabb << pose.to_matrix();

        sphere.origin = aabb.center();
        sphere.radius = glm::length(aabb.extents());
    }

    void model_instance_t::render(const camera_params& camera_params, const vec3_t& light_location) const
    {
        if (model == nullptr)
        {
            throw std::exception();
        }

        if (intersects(camera_params.frustum, sphere) == intersect_type_e::DISJOINT ||
            intersects(camera_params.frustum, aabb) == intersect_type_e::DISJOINT)
        {
            //skeleton aabb does not intersect camera frustum
            //return;
        }

        auto view_projection_matrix = camera_params.projection_matrix * camera_params.view_matrix;

        model->render(camera_params.location, pose.to_matrix(), view_projection_matrix, bone_matrices, light_location, mesh_materials);

#if defined (DEBUG)
        render_aabb(pose.to_matrix(), view_projection_matrix, skeleton.aabb, rgba_type(1, 0, 0, 1));
        render_aabb(mat4_t(), view_projection_matrix, aabb, rgba_type(0, 1, 0, 1));
        render_sphere(mat4_t(), view_projection_matrix, sphere);
#endif
    }
    
    const pose3& model_instance_t::get_bone_pose(const hash_t& bone_hash) const
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

    void model_instance_t::set_model(const boost::shared_ptr<model_t>& model)
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
            mesh_materials.push_back(boost::make_shared<material_instance_t>(mesh->material));
        }
    }

    void model_instance_t::play(const hash_t& animation_hash)
    {
        animation = resources.get<model_animation_t>(animation_hash);
    }
}
