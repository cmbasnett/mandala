#pragma once

// std
#include <string>

// boost
#include <boost\preprocessor\stringize.hpp>

#define NAGA_GAME_COMPONENT_DEFINE_COMPONENT_NAME(name)\
    static const char* component_name;\
    \
    virtual std::string get_type_name() const\
    {\
        return std::string(component_name);\
    }