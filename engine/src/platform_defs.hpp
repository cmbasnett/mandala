#pragma once

#if defined(_WIN32) || defined(WIN32)
#define NAGA_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#define NAGA_OSX
#elif defined(__ANDROID__) && defined(__ANDROID_API__)
#include <android/api-level.h>
#define NAGA_ANDROID
#elif defined(__gnu_linux__)
#define NAGA_LINUX
#endif

#if defined(NAGA_WINDOWS) || defined(NAGA_OSX) || defined(NAGA_LINUX)
#define NAGA_PC
#endif