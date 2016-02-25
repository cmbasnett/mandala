#pragma once

class btCollisionObject;

namespace naga
{
    struct physics_collision_object
    {
        physics_collision_object();

        const btCollisionObject* const get_ptr() const { return ptr; }

    private:
        btCollisionObject* ptr;
    };
}