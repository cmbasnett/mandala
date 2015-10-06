#pragma once

//mandala
#include "pose.hpp"
#include "model_skeleton.hpp"
#include "camera.hpp"

//boost
#include <boost\shared_ptr.hpp>

namespace mandala
{
    struct model_t;
    struct model_animation_t;
    struct material_instance_t;

    struct model_instance_t
    {
        model_instance_t(boost::shared_ptr<model_t> model);
        model_instance_t(const hash_t& model_hash);

        pose3 pose;

        void tick(float32_t dt);
        void render(const camera_params& camera_params, const vec3_t& light_location) const;

        void play(const hash_t& animation_hash);

        const boost::shared_ptr<model_t>& get_model() const { return model; }
        const pose3& get_bone_pose(const hash_t& bone_hash) const;
        const aabb3_t& get_aabb() const { return aabb; }
        const sphere_t& get_sphere() const { return sphere; }

        void set_model(const boost::shared_ptr<model_t>& model);

    private:
        boost::shared_ptr<model_t> model;
        float32_t t = 0.0f;
        std::vector<mat4_t> bone_matrices;
        model_skeleton_t skeleton;
        boost::shared_ptr<model_animation_t> animation;
        std::vector<boost::shared_ptr<material_instance_t>> mesh_materials;
        aabb3_t aabb;
        sphere_t sphere;
    };
}
