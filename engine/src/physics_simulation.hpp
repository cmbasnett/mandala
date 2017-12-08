#pragma once

//boost
#include <boost\shared_ptr.hpp>

//naga
#include "types.hpp"
#include "trace_result.hpp"

class btMultiBodyDynamicsWorld;
struct btDbvtBroadphase;
class btCollisionDispatcher;
class btDefaultCollisionConfiguration;
class btMultiBodyConstraintSolver;

namespace naga
{
    struct RigidBodyComponent;

    struct PhysicsSimulation
    {
		PhysicsSimulation();

        void step(f32 dt);

        void set_gravity(const vec3& gravity);
        vec3 get_gravity() const;

		void add_rigid_body(const boost::shared_ptr<RigidBodyComponent>& rigid_body);
		void remove_rigid_body(const boost::shared_ptr<RigidBodyComponent>& rigid_body);

		TraceResult trace(const vec3& start, const vec3& end) const;

    private:
        btMultiBodyDynamicsWorld* dynamics_world;
        btDbvtBroadphase* broadphase_interface;
        btCollisionDispatcher* collision_dispatcher;
        btDefaultCollisionConfiguration* collision_configuration;
        btMultiBodyConstraintSolver* multi_body_constraint_solver;
    };
}
