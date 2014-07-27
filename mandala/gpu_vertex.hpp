#pragma once

//mandala
#include "types.hpp"

//boost
#include <boost\preprocessor.hpp>

#define SEQ (w) (x) (y) (z)

#define MACRO(R, DATA, ELEM) BOOST_PP_CAT(ELEM, DATA);

#define CREATE_ATTRIBUTE(TYPE,NAME,...) TYPE NAME;

#define CREATE_GPU_VERTEX(NAME,...) struct NAME\
{\
    typedef NAME type;\
}

CREATE_GPU_VERTEX(vertex_t, size_t a = 0;);

namespace mandala
{
    void abc()
    {
        vertex_t v;
    }
};
