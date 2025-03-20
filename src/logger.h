#pragma once

#include "defines.h"

#include <stdio.h>
#include <stdarg.h>

#define LOGGER_ENABLE_WARNING 1
#define LOGGER_ENABLE_INFO 1
#define LOGGER_ENABLE_DEBUG 1
#define LOGGER_ENABLE_TRACE 1

enum {
    LOG_ERROR = 0,
    LOG_WARNING = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3,
    LOG_TRACE = 4
};

void log_message(u32 msg_type, const char* msg, ...) {

    char output[16384];
    va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(output, 16384, msg, arg_ptr);
    va_end(arg_ptr);

    switch(msg_type) {
        case LOG_ERROR:
            printf("\e[0;31m[ERROR]: %s\e[0m\n", output);
            break;
        case LOG_WARNING:
            printf("\e[0;33m[WARN]: %s\e[0m\n", output);
            break;
        case LOG_INFO:
            printf("\e[0;32m[INFO] : %s\e[0m\n", output);
            break;
        case LOG_DEBUG:
            printf("\e[0;34m[DEBUG]: %s\e[0m\n", output);
            break;
        case LOG_TRACE:
            printf("[TRACE]: %s\n", output);
            break;
    }
}
#define LOGGER_ENABLE_WARNING 1
#define LOGGER_ENABLE_INFO 1
#define LOGGER_ENABLE_DEBUG 1
#define LOGGER_ENABLE_TRACE 1

#define LERROR(message, ...); log_message(LOG_ERROR, message, ##__VA_ARGS__)

#if LOGGER_ENABLE_WARNING == 1
#define LWARN(message, ...); log_message(LOG_WARNING, message, ##__VA_ARGS__)
#else
#define LWARN(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_INFO == 1
#define LINFO(message, ...); log_message(LOG_INFO, message, ##__VA_ARGS__)
#else
#define LINFO(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_DEBUG== 1
#define LDEBUG(message, ...); log_message(LOG_DEBUG, message, ##__VA_ARGS__)
#else
#define LDEBUG(message, ...) //do nothing
#endif

#if LOGGER_ENABLE_TRACE== 1
#define LTRACE(message, ...); log_message(LOG_TRACE, message, ##__VA_ARGS__)
#else
#define LTRACE(message, ...) //do nothing
#endif
