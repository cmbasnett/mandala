#pragma once

#if defined(_WIN32) || defined(WIN32)
#define MANDALA_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#define MANDALA_OSX
#elif defined(__ANDROID__) && defined(__ANDROID_API__)
#include <android/api-level.h>
#define MANDALA_ANDROID
#elif defined(__gnu_linux__)
#define MANDALA_LINUX
#endif

#if defined(MANDALA_WINDOWS) || defined(MANDALA_OSX) || defined(MANDALA_LINUX)
#define MANDALA_PC
#endif