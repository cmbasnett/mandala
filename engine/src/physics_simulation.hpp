#pragma once

//naga
#include "types.hpp"

//boost
#include <boost\shared_ptr.hpp>

class btMultiBodyDynamicsWorld;
struct btDbvtBroadphase;
class btCollisionDispatcher;
class btDefaultCollisionConfiguration;
class btMultiBodyConstraintSolver;

namespace naga
{
    struct rigid_body_component;

    struct physics_simulation
    {
        physics_simulation();

        void step(f32 dt);

        void set_gravity(const vec3& gravity);
        vec3 get_gravity() const;

        void add_rigid_body(const boost::shared_ptr<rigid_body_component>& rigid_body);
        void remove_rigid_body(const boost::shared_ptr<rigid_body_component>& rigid_body);

    private:
        btMultiBodyDynamicsWorld* dynamics_world;
        btDbvtBroadphase* broadphase_interface;
        btCollisionDispatcher* collision_dispatcher;
        btDefaultCollisionConfiguration* collision_configuration;
        btMultiBodyConstraintSolver* multi_body_constraint_solver;
    };
}
