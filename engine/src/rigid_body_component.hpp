#pragma once

//boost
#include <boost\enable_shared_from_this.hpp>

//naga
#include "game_component.hpp"
#include "pose.hpp"
#include "aabb.hpp"

class btRigidBody;
class btCollisionShape;

namespace naga
{
    struct rigid_body_component : game_component, boost::enable_shared_from_this<rigid_body_component>
    {
        static const char* component_name;

        rigid_body_component();

        virtual void on_tick(f32 dt) override;
        virtual void on_create() override;;
        virtual void on_destroy() override;;

        pose3 get_pose() const;
        f32 get_mass() const;
        vec3 get_center_of_mass() const;
        aabb3 get_aabb() const;

        void set_pose(const pose3& pose);
        void set_mass(f32 mass);
        void set_center_of_mass(const vec3& center_of_mass);

        //TODO: not 
        void set_collision_shape(btCollisionShape* collision_shape);

        btRigidBody* get_ptr() const { return ptr; }

    private:
        btRigidBody* ptr;
    };
}
