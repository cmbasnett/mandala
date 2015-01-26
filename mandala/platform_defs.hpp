#pragma once

#if defined(_WIN32) || defined(WIN32)
#define MANDALA_WINDOWS
#elif defined(__APPLE__)
#define MANDALA_OSX
#endif

#if defined(MANDALA_WINDOWS) || defined(MANDALA_OSX)
#define MANDALA_PC
#endif
