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
		const boost::shared_ptr<model_t> model;

		void tick(float32_t dt);
		void render(const camera_t& camera, const vec3_t& light_location) const;

        void play(const hash_t& animation_hash);

		const pose3 get_bone_pose(const hash_t& bone_hash) const;

    private:
        float32_t t = 0.0f;
        std::vector<mat4_t> bone_matrices;
		model_skeleton_t skeleton;
        boost::shared_ptr<model_animation_t> animation;
        std::vector<boost::shared_ptr<material_instance_t>> mesh_materials;
        aabb3_t aabb;
	};
}
