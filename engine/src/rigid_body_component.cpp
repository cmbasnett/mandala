//naga
#include "rigid_body_component.hpp"
#include "game_object.hpp"
#include "scene.hpp"
#include "physics_simulation.hpp"

//bullet
#include "btBulletDynamicsCommon.h"

namespace naga
{
    const char* rigid_body_component::component_name = "RigidBodyComponent";

    rigid_body_component::rigid_body_component()
    {
        auto mass = 1.0f;
        auto interia = btVector3(0, 0, 0);

        auto ground_shape = new btBoxShape(btVector3(btScalar(50.0), btScalar(50.0), btScalar(50.0)));

        if (mass != 0.0f)
        {
            ground_shape->calculateLocalInertia(mass, interia);
        }

        auto motion_state = new btDefaultMotionState();

        btRigidBody::btRigidBodyConstructionInfo construction_info(mass, motion_state, ground_shape, interia);

        ptr = new btRigidBody(construction_info);
        ptr->setUserPointer(this);
    }

    void rigid_body_component::on_tick(f32 dt)
    {
        // TODO: perhaps some sort of movement reponse might be more applicable,
        // otherwise we're going to be doing a lot of unnecessary pose
        // recalculations.

        get_owner()->pose = get_pose();
    }

    void rigid_body_component::on_create()
    {
        get_owner()->get_scene()->get_physics()->add_rigid_body(shared_from_this());
    }

    void rigid_body_component::on_destroy()
    {
        get_owner()->get_scene()->get_physics()->remove_rigid_body(shared_from_this());
    }

    pose3 rigid_body_component::get_pose() const
    {
        const auto& world_transform = ptr->getWorldTransform();
        const auto& location = world_transform.getOrigin();
        const auto& rotation = world_transform.getRotation();

        pose3 pose;
        pose.location.x = location.x();
        pose.location.y = location.y();
        pose.location.z = location.z();
        pose.rotation.x = rotation.x();
        pose.rotation.y = rotation.y();
        pose.rotation.z = rotation.z();
        pose.rotation.w = rotation.w();

        return pose;
    }

    f32 rigid_body_component::get_mass() const
    {
        if (ptr->getInvMass() == 0.0f)
        {
            return 0.0f;
        }

        return 1.0f / ptr->getInvMass();
    }

    //https://studiofreya.com/game-maker/bullet-physics/bullet-physics-how-to-change-body-mass/
    void rigid_body_component::set_mass(f32 mass)
    {
        get_owner()->get_scene()->get_physics()->remove_rigid_body(shared_from_this());

        btVector3 inertia;
        ptr->getCollisionShape()->calculateLocalInertia(mass, inertia);
        ptr->setMassProps(mass, inertia);

        get_owner()->get_scene()->get_physics()->add_rigid_body(shared_from_this());
    }

    vec3 rigid_body_component::get_center_of_mass() const
    {
        auto& com = ptr->getCenterOfMassPosition();

        return vec3(com.x(), com.y(), com.z());
    }

    void rigid_body_component::set_center_of_mass(const vec3& center_of_mass)
    {
        btTransform xform;
        xform.setOrigin(btVector3(center_of_mass.x, center_of_mass.y, center_of_mass.z));

        ptr->setCenterOfMassTransform(xform);
    }

    aabb3 rigid_body_component::get_aabb() const
    {
        btVector3 min, max;
        ptr->getAabb(min, max);

        return aabb3(vec3(min.x(), min.y(), min.z()), vec3(max.x(), max.y(), max.z()));
    }

    void rigid_body_component::set_collision_shape(btCollisionShape* collision_shape)
    {
        ptr->setCollisionShape(collision_shape);
    }
}