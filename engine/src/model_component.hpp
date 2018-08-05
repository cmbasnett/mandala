#pragma once

//naga
#include "pose.hpp"
#include "model_skeleton.hpp"
#include "scene_component.hpp"
#include "sphere.hpp"

//boost
#include <boost\shared_ptr.hpp>

namespace naga
{
	struct Model;
    struct ModelAnimation;
	struct MaterialInstance;
	struct CameraParameters;

	struct ModelAnimationController
	{

	};

    struct ModelComponent : SceneComponent
    {
        void on_tick(f32 dt) override;
		void on_render(CameraParameters& camera_parameters) override;

		// TODO: make some sort of animation controller
        void play(const std::string& animation_name);

		const boost::shared_ptr<Model>& get_model() const { return model; }
        Pose3 get_bone_pose(const std::string& bone_name) const;
		const AABB3& get_aabb() const { return aabb; }
        const Sphere& get_sphere() const { return sphere; }

		void set_model(const boost::shared_ptr<Model>& model);

    private:
        boost::shared_ptr<Model> model;
        f32 t = 0.0f;
        std::vector<mat4> bone_matrices;
        ModelSkeleton skeleton;
        boost::shared_ptr<ModelAnimation> animation;
        std::vector<boost::shared_ptr<MaterialInstance>> mesh_materials;
		AABB3 aabb;
		Sphere sphere;
		ModelAnimationController animation_controller;
    };
}
