#pragma once

// std
#include <memory>

class btMultiBodyDynamicsWorld;
struct btDbvtBroadphase;
class btCollisionDispatcher;
class btDefaultCollisionConfiguration;
class btMultiBodyConstraintSolver;

namespace naga
{
    struct physics_simulation
    {
        physics_simulation();

    private:
        btMultiBodyDynamicsWorld* dynamics_world;
        btDbvtBroadphase* broadphase_interface;
        btCollisionDispatcher* collision_dispatcher;
        btDefaultCollisionConfiguration* collision_configuration;
        btMultiBodyConstraintSolver* multi_body_constraint_solver;
    };
}
