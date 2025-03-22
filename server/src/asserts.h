#pragma once

#include "defines.h"
#include "logger.h"

#define ASSERTIONS_ENABLED

#ifdef ASSERTIONS_ENABLED

void on_assertion_failure(const char* expr, const char* msg, const char* file, i32 line) {
    log_message(LOG_ERROR, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expr, msg, file, line);
}

#define _ASSERT(expr)                                           \
    {                                                           \
        if(expr) {                                              \
        } else {                                                \
            on_assertion_failure(#expr, "", __FILE__, __LINE__);\
            __builtin_trap();                                   \
        }                                                       \
    }

#define _ASSERT_MSG(expr, message)                                   \
    {                                                                \
        if(expr) {                                                   \
        } else {                                                     \
            on_assertion_failure(#expr, message, __FILE__, __LINE__);\
            __builtin_trap();                                        \
        }                                                            \
    }

#else
#define _ASSERT(expr)               //do nothing if assertions aren't enabled
#define _ASSERT_MSG(expr, message)  //do nothing if assertions aren't enabled
#endif
