//naga
#include "game_object_collection.hpp"
#include "game_object.hpp"

namespace naga
{
    void game_object_collection::add(type& game_object)
    {
        if (game_object->get_scene() != nullptr)
        {
            throw std::invalid_argument("cannot add a game object that is already in a scene");
        }

        objects.push_back(game_object);

        game_object->scene = shared_from_this();
    }

    void game_object_collection::erase(type& game_object)
    {
        if (game_object->get_scene() != shared_from_this())
        {
            throw std::exception("")
        }
    }
}