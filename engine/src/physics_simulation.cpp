// naga
#include "physics_simulation.hpp"

// bullet
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics\Featherstone\btMultiBodyDynamicsWorld.h"
#include "BulletDynamics\Featherstone\btMultiBodyConstraintSolver.h"

namespace naga
{
    physics_simulation::physics_simulation()
    {
        collision_configuration = new btDefaultCollisionConfiguration();
        collision_dispatcher = new btCollisionDispatcher(collision_configuration);
        broadphase_interface = new btDbvtBroadphase();
        multi_body_constraint_solver = new btMultiBodyConstraintSolver();
        dynamics_world = new btMultiBodyDynamicsWorld(
            collision_dispatcher,
            broadphase_interface,
            multi_body_constraint_solver,
            collision_configuration);
    }
}