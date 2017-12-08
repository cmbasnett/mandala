#pragma once

//std
#include <vector>
#include <set>

//boost
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

//naga
#include "octtree.hpp"
#include "game_object_collection.hpp"
#include "trace_result.hpp"

namespace naga
{
    struct GameObject;
	struct FrameBuffer;
	struct InputEvent;
    struct PhysicsSimulation;
    struct bsp;

	struct Scene : boost::enable_shared_from_this<Scene>
    {
		Scene();

		const std::vector<boost::shared_ptr<GameObject>>& get_game_objects() const { return game_objects; }

        void tick(f32 dt);
		void render(const boost::shared_ptr<FrameBuffer>& frame_buffer, const boost::shared_ptr<GameObject>& camera) const;
		void on_input_event(InputEvent& input_event);

        boost::shared_ptr<GameObject> create_game_object();
		void remove_game_object(const boost::shared_ptr<GameObject>& game_object);

		const boost::shared_ptr<PhysicsSimulation>& get_physics() const { return physics; }

		TraceResult trace(const vec3& start, const vec3& end) const;

    private:
		friend struct GameObject;

		std::vector<boost::shared_ptr<GameObject>> game_objects;

		boost::shared_ptr<PhysicsSimulation> physics;
        OctTree octtree;
        boost::shared_ptr<bsp> bsp;
    };
}