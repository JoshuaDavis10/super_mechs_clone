#pragma once

#include "defines.h"
#include "logger.h"

#include <stdlib.h> //for debug break?

#define ASSERTIONS_ENABLED

#ifdef ASSERTIONS_ENABLED

void on_assertion_failure(const char* expr, const char* msg, const char* file, i32 line);

#define SM_ASSERT(expr)                                         \
    {                                                           \
        if(expr) {                                              \
        } else {                                                \
            on_assertion_failure(#expr, "", __FILE__, __LINE__);\
            __debugbreak();                                      \
        }                                                       \
    }

#define SM_ASSERT_MSG(expr, message)                                 \
    {                                                                \
        if(expr) {                                                   \
        } else {                                                     \
            on_assertion_failure(#expr, message, __FILE__, __LINE__);\
            __debugbreak();                                           \
        }                                                            \
    }

#else
#define SM_ASSERT(expr)
#define SM_ASSERT_MSG(expr, message)
#endif
