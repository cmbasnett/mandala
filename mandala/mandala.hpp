#pragma once

#if defined(_WIN32) || defined(WIN32)
#define MANDALA_WINDOWS
#elif defined(__APPLE__)
#define MANDALA_APPLE
#include "TargetConditionals.h"
#if defined(TARGET_OS_IPHONE)
#define MANDALA_IOS
#elif defined(TARGET_IPHONE_SIMULATOR)
#define MANDALA_IOS_SIMULATOR
#elif defined(TARGET_OS_MAC)
#define MANDALA_OSX
#endif
#endif

#if defined(MANDALA_WINDOWS) || defined(MANDALA_OSX)
#define MANDALA_PC
#endif
