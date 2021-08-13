#pragma once

#include <cstdio>

#ifndef LOG_ALWAYS
#   define LOG_ALWAYS(...) printf(__VA_ARGS__)
#endif

#ifndef LOG_ERROR
#   define LOG_ERROR(...) fprintf(stderr, "error: " __VA_ARGS__)
#endif

#ifndef LOG_DEBUG
#   ifdef NDEBUG
#       define LOG_DEBUG(...)
#   else
#       define LOG_DEBUG(...) printf(__VA_ARGS__)
#   endif
#endif
