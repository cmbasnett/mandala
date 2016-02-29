#pragma once

#include <iterator>
#include <vector>

#include <boost\shared_ptr.hpp>

namespace naga
{
    struct game_object;

    struct game_object_collection
    {
        typedef boost::shared_ptr<game_object> type;

        void add(type& game_object);
        void erase(type& game_object);
        void find(size_t id);
        void find(type& game_object);

    private:
        std::vector<type> objects;
        std::set<size_t> ids;
    };
}