#pragma once

//naga
#include "pose.hpp"
#include "model_skeleton.hpp"
#include "game_component.hpp"
#include "sphere.hpp"

//boost
#include <boost\shared_ptr.hpp>

namespace naga
{
    struct model;
    struct model_animation;
    struct material_instance;
    struct camera_params;

    struct model_component : game_component
    {
        void tick(f32 dt);
        void render(const camera_params& camera_params, const vec3& light_location) const;

        void play(const hash& animation_hash);

        const boost::shared_ptr<model>& get_model() const { return model; }
        pose3 get_bone_pose(const hash& bone_hash) const;
        const aabb3& get_aabb() const { return aabb; }
        const sphere& get_sphere() const { return sphere; }

        void set_model(const boost::shared_ptr<model>& model);

    private:
        boost::shared_ptr<model> model;
        f32 t = 0.0f;
        std::vector<mat4> bone_matrices;
        model_skeleton skeleton;
        boost::shared_ptr<model_animation> animation;
        std::vector<boost::shared_ptr<material_instance>> mesh_materials;
        aabb3 aabb;
        sphere sphere;
    };
}
