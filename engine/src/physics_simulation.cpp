// naga
#include "physics_simulation.hpp"
#include "rigid_body_component.hpp"

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

    void physics_simulation::step(f32 dt)
    {
        dynamics_world->stepSimulation(dt);

        auto manifold_count = dynamics_world->getDispatcher()->getNumManifolds();

        for (i32 i = 0; i < manifold_count; ++i)
        {
            auto contact_manifold = dynamics_world->getDispatcher()->getManifoldByIndexInternal(i);
            auto body0 = contact_manifold->getBody0();
            auto body1 = contact_manifold->getBody1();

            auto contact_count = contact_manifold->getNumContacts();

            for (i32 j = 0; j < contact_count; ++j)
            {
                auto contact_point = contact_manifold->getContactPoint(j);

                body0->getUserPointer();
            }
        }
    }

    void physics_simulation::set_gravity(const vec3& gravity)
    {
        dynamics_world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
    }

    vec3 physics_simulation::get_gravity() const
    {
        const auto gravity = dynamics_world->getGravity();

        return vec3(gravity.getX(), gravity.getY(), gravity.getZ());
    }

    void physics_simulation::add_rigid_body(const boost::shared_ptr<rigid_body_component>& rigid_body)
    {
        dynamics_world->addRigidBody(rigid_body->get_ptr());
    }

    void physics_simulation::remove_rigid_body(const boost::shared_ptr<rigid_body_component>& rigid_body)
    {
        dynamics_world->removeRigidBody(rigid_body->get_ptr());
    }
}