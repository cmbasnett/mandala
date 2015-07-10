//mandala
#include "sprite_animation.hpp"

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

        _ptree.get<float32_t>("duration");
    }

    const sprite_ref_t& sprite_animation_t::get_sprite_at_time(const time_type& time) const
    {
        assert(!frames.empty());

        if (time.count() <= 0)
        {
            return frames.begin()->sprite;
        }
        else if (time >= _duration)
        {
            return frames.rbegin()->sprite;
        }
        else
        {
            return frames.begin()->sprite;
        }
    }
}
