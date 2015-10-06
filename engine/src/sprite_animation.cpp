//mandala
#include "sprite_animation.hpp"
#include "bisect.hpp"

//boost
#include <boost\property_tree\json_parser.hpp>

namespace mandala
{
    sprite_animation_t::sprite_animation_t(std::istream& istream)
    {
        using namespace boost::property_tree;
        using namespace boost::property_tree::json_parser;

        ptree _ptree;

        read_json(istream, _ptree);

        _ptree.get<f32>("duration");
    }

    const sprite_ref_t& sprite_animation_t::get_sprite_at_time(const time_type& time) const
    {
        return frames[bisect_left(frames, time, 0, frames.size() - 1)].sprite;
    }
}
