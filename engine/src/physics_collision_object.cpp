//naga
#include "physics_collision_object.hpp"

//bullet
#include <BulletCollision\CollisionDispatch\btCollisionObject.h>

namespace naga
{
    physics_collision_object::physics_collision_object()
    {
        ptr = new btCollisionObject();
        ptr->setUserPointer(this);
    }
}