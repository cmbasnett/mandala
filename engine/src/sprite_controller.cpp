#include "sprite_controller.hpp"
#include "sprite_animation.hpp"

namespace mandala
{
    void sprite_controller::clear()
    {
        operations.clear();
    }

    void sprite_controller::pause()
    {
        is_playing = false;
    }

    void sprite_controller::play()
    {
        is_playing = true;
    }

    void sprite_controller::queue(const boost::shared_ptr<sprite_animation_t>& animation)
    {
        operation operation;
        operation.type = operation::type::ANIMATION;
        operation.animation = animation;
        operation.duration = animation->get_duration();

        operations.push_back(operation);
    }

    void sprite_controller::wait(float32_t duration)
    {
        operation operation;
        operation.type = operation::type::WAIT;
        operation.duration = duration;

        operations.push_back(operation);
    }

    void sprite_controller::tick(float32_t dt)
    {
        if (operations.empty())
        {
            // no operation in queue, do nothing
            return;
        }

        auto& operation = operations.front();

        // increment elapsed time
        elapsed_time += dt;

        // check if operation has expired and find the next operation to work on
        while (elapsed_time > operation.duration)
        {
            //TODO: we need to set the sprite as we loop through these

            elapsed_time -= operation.duration;

            operations.pop_front();

            if (operations.empty())
            {
                return;
            }

            operation = operations.front();
        }

        switch (operation.type)
        {
        case operation::type::ANIMATION:
            operation.animation->get_sprite_at_time(elapsed_time);
            break;
        case operation::type::FRAME:

            break;
        case operation::type::WAIT:
            //do nothing
            break;
        }
    }
}