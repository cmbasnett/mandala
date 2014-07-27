#pragma once

#define FOR_EACH_1(what, x, ...) what(x)

#define FOR_EACH_2(what, x, ...) \
    what(x) \
    FOR_EACH_1(what, __VA_ARGS__)

#define FOR_EACH_3(what, x, ...) \
    what(x) \
    FOR_EACH_2(what, __VA_ARGS__)