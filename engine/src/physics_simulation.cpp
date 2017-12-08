// naga
#include "physics_simulation.hpp"
#include "rigid_body_component.hpp"

// bullet
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics\Featherstone\btMultiBodyDynamicsWorld.h"
#include "BulletDynamics\Featherstone\btMultiBodyConstraintSolver.h"

namespace naga
{
	PhysicsSimulation::PhysicsSimulation()
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

	void PhysicsSimulation::step(f32 dt)
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

	void PhysicsSimulation::set_gravity(const vec3& gravity)
    {
        dynamics_world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
    }

	vec3 PhysicsSimulation::get_gravity() const
    {
        const auto gravity = dynamics_world->getGravity();

        return vec3(gravity.getX(), gravity.getY(), gravity.getZ());
    }

	void PhysicsSimulation::add_rigid_body(const boost::shared_ptr<RigidBodyComponent>& rigid_body)
    {
        dynamics_world->addRigidBody(rigid_body->get_ptr());
    }

	void PhysicsSimulation::remove_rigid_body(const boost::shared_ptr<RigidBodyComponent>& rigid_body)
    {
        dynamics_world->removeRigidBody(rigid_body->get_ptr());
    }

	TraceResult PhysicsSimulation::trace(const vec3& start, const vec3& end) const
	{
		TraceResult trace_result;
		auto rayFromWorld = btVector3(start.x, start.y, start.z);
		auto rayToWorld = btVector3(end.x, end.y, end.z);

		btCollisionWorld::ClosestRayResultCallback rayResultCallback(rayFromWorld, rayToWorld);

		dynamics_world->rayTest(rayFromWorld, rayToWorld, rayResultCallback);

		if (rayResultCallback.hasHit())
		{
			trace_result.did_hit = true;
			trace_result.location.x = rayResultCallback.m_hitPointWorld.x();
			trace_result.location.y = rayResultCallback.m_hitPointWorld.y();
			trace_result.location.z = rayResultCallback.m_hitPointWorld.z();
			trace_result.normal.x = rayResultCallback.m_hitNormalWorld.x();
			trace_result.normal.y = rayResultCallback.m_hitNormalWorld.y();
			trace_result.normal.z = rayResultCallback.m_hitNormalWorld.z();
		}

		return trace_result;
	}
}