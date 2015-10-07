//mandala
#include "sprite_animation.hpp"
#include "bisect.hpp"

//boost
#include <boost\property_tree\json_parser.hpp>

namespace mandala
{
    sprite_animation::sprite_animation(std::istream& istream)
    {
        using namespace boost::property_tree;
        using namespace boost::property_tree::json_parser;

        ptree _ptree;

        read_json(istream, _ptree);

        _ptree.get<f32>("duration");
    }

    const sprite_ref& sprite_animation::get_sprite_at_time(const time_type& time) const
    {
        return frames[bisect_left(frames, time, 0, frames.size() - 1)].sprite;
    }
}
