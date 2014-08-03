#pragma once

//mandala
#include "types.hpp"

//boost
#include <boost\preprocessor.hpp>

#define DECLARE_DATA_MEMBER1(R,TYPES,INDEX,NAME) \
	BOOST_PP_SEQ_ELEM(INDEX, TYPES) NAME;

#define DEFINE_ACCESSOR1(R,TYPES,INDEX,NAME) \
	BOOST_PP_SEQ_ELEM(INDEX, TYPES) NAME() { return NAME; }

#define MY_MACRO1(TYPES,NAMES) \
	BOOST_PP_SEQ_FOR_EACH_I(DECLARE_DATA_MEMBER1, TYPES, NAMES) \
public: \
	//BOOST_PP_SEQ_FOR_EACH_I(DEFINE_ACCESSOR1, TYPES, NAMES)

struct A
{
	MY_MACRO1((mandala::vec3_t)(mandala::vec2_t), (position)(texcoord))
};

namespace mandala
{
};
